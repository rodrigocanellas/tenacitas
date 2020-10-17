#ifndef TENACITAS_CONCURRENT_RUNNER_H
#define TENACITAS_CONCURRENT_RUNNER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include <concurrent/internal/log.h>
#include <concurrent/internal/worker_wrapper.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_time, typename t_result,
          typename... t_params>
struct runner_t {

  typedef std::function<void(std::thread::id)> timeout_callback;

  typedef std::function<t_result(t_params...)> worker;

  runner_t() = default;

  runner_t(
      t_time p_timeout, worker p_worker,
      timeout_callback p_timeout_callback = [](std::thread::id) -> void {})
      : m_timeout(p_timeout), m_work_wrapper(p_worker),
        m_timeout_callback(p_timeout_callback) {
    start();
  }

  ~runner_t() { stop(); }

  void start() {
    if (!m_work_wrapper) {
      throw std::runtime_error("work function not defined");
    }

    if (m_stopped) {
      concurrent_debug(m_log, "starting");
      m_stopped = false;
      m_thread = concurrent::thread([this]() -> void { loop(); });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

  void stop() {
    if (!m_stopped) {
      concurrent_debug(m_log, "stopping");
      m_stopped = true;
      m_cond_exec.notify_one();
      m_cond_wait.notify_one();
      m_thread.join();
    }
  }

  inline t_time get_timeout() const { return m_timeout; }

  inline worker get_worker() const { return m_work_wrapper.get_worker(); }

  inline timeout_callback get_timeout_callback() { return m_timeout_callback; }

  auto operator()(t_params... p_params) {

    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return m_work_wrapper.get_result_not_ok();
    }

    m_work_wrapper.set_params(p_params...);

    //    m_params = std::make_tuple(p_params...);
    //    concurrent_debug(m_log, "parameters provided = ", m_params);

    m_is_timeout = false;

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "timeout: ", m_thread.get_id(),
                      " did not finish in time");
      timeout_callback_thread();
      m_is_timeout = true;
      return m_work_wrapper.get_result_not_ok();
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return m_work_wrapper.get_result_not_ok();
    }

    concurrent_debug(m_log, "worker finished on time");
    return m_work_wrapper.get_result_ok();
  }

private:
  typedef work_wrapper_t<t_result, t_params...> work_wrapper;

private:
  void timeout_callback_thread() {
    std::thread _timeout_thread(
        [this]() -> void { m_timeout_callback(m_thread.get_id()); });
    //    _timeout_thread.detach();
    _timeout_thread.join();
  }

  void loop() {
    while (true) {

      concurrent_debug(m_log, "waiting for work");

      std::unique_lock<std::mutex> _lock(m_mutex_exec);
      m_cond_exec.wait(_lock);

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        m_cond_wait.notify_one();
        break;
      }

      concurrent_debug(m_log, "work to do!");
      //      m_worker_result = std::apply(m_worker, std::move(m_params));

      m_work_wrapper();

      if (!m_is_timeout) {

        if (m_stopped) {
          concurrent_debug(m_log, "stopped while working");
          m_cond_wait.notify_one();
          break;
        }

        concurrent_debug(m_log, "notifying that work is done");
        m_cond_wait.notify_one();
      } else {
        concurrent_debug(m_log, "timeout");
        break;
      }
    }
  }

private:
  t_time m_timeout;
  work_wrapper m_work_wrapper;
  timeout_callback m_timeout_callback;

  t_log m_log{"concurrent::runner"};
  bool m_stopped{true};
  bool m_is_timeout{false};

  std::mutex m_mutex_operation;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_wait;
  concurrent::thread m_thread;
  std::mutex m_mutex_exec;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_RUNNER_H

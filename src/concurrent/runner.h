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
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_time, typename t_result, typename t_worker,
          typename... t_params>
struct runner_t {

  typedef std::function<void(std::thread::id)> timeout_callback;

  runner_t(
      t_time p_timeout, t_worker p_worker,
      timeout_callback p_timeout_callback = [](std::thread::id) -> void {})
      : m_timeout(p_timeout), m_worker(p_worker),
        m_timeout_callback(p_timeout_callback) {
    start();
  }

  ~runner_t() { stop(); }

  void start() {
    if (m_stopped) {
      concurrent_debug(m_log, "starting");
      m_stopped = false;
      m_thread = concurrent::thread([this]() -> void { loop(); });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      //      m_provider_executer.start();
    }
  }

  void stop() {
    if (!m_stopped) {
      concurrent_debug(m_log, "stopping");
      //      m_provider_executer.stop();
      m_stopped = true;
      m_cond_exec.notify_one();
      m_cond_wait.notify_one();
      m_thread.join();
    }
  }

  inline t_time get_timeout() const { return m_timeout; }

  auto operator()(t_params... p_params) {

    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return not_worked::result();
    }

    m_params = std::make_tuple(p_params...);
    concurrent_debug(m_log, "parameters provided = ", m_params);

    m_is_timeout = false;

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      //      timeout_callback_thread();
      m_is_timeout = true;
      return not_worked::result();
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return not_worked::result();
    }

    concurrent_debug(m_log, "worker finished on time");
    return m_work_wrapper.result();
  }

  auto operator()(std::function<std::tuple<t_params...>()> p_provider) {

    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    typedef std::function<std::tuple<t_params...>()> provider;

    typedef runner_t<t_log, std::chrono::seconds, std::tuple<t_params...>,
                     provider>
        provider_runner;

    provider_runner _provider_runner(std::chrono::seconds(1), p_provider);

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return not_worked::result();
    }

    auto _maybe = _provider_runner();
    if (!_maybe) {
      return not_worked::result();
    }
    m_params = _maybe.value();
    concurrent_debug(m_log, "parameters provided = ", m_params);

    m_is_timeout = false;

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      timeout_callback_thread();
      m_is_timeout = true;
      return not_worked::result();
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return not_worked::result();
    }

    concurrent_debug(m_log, "worker finished on time");
    return m_work_wrapper.result();
  }

private:
  struct work_with_return {

    inline void operator()(t_worker p_worker,
                           std::tuple<t_params...> p_params) {
      m_worker_result = std::apply(p_worker, std::move(p_params));
    }

    inline std::optional<t_result> result() { return m_worker_result; }

    std::optional<t_result> m_worker_result;
  };

  struct work_with_no_return {

    inline void operator()(t_worker p_worker,
                           std::tuple<t_params...> p_params) {
      std::apply(p_worker, std::move(p_params));
    }

    void result() {}
  };

  typedef typename std::conditional<std::is_void<t_result>::value,
                                    work_with_no_return, work_with_return>::type
      work_wrapper;

  struct not_worked_void {
    static void result() {}
  };

  struct not_worked_result {
    static std::optional<t_result> result() { return {}; }
  };

  typedef
      typename std::conditional<std::is_void<t_result>::value, not_worked_void,
                                not_worked_result>::type not_worked;

private:
  void timeout_callback_thread() {
    std::thread _timeout_thread(
        [this]() -> void { m_timeout_callback(m_thread.get_id()); });
    _timeout_thread.detach();
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

      m_work_wrapper(m_worker, m_params);

      if (!m_is_timeout) {

        if (m_stopped) {
          concurrent_debug(m_log, "stopped while working");
          m_cond_wait.notify_one();
          break;
        }

        concurrent_debug(m_log, "notifying that work is done");
        m_cond_wait.notify_one();
      }
    }
  }

private:
  t_time m_timeout;
  t_worker m_worker;
  t_log m_log{"runner_t"};
  bool m_stopped{true};
  timeout_callback m_timeout_callback;

  work_wrapper m_work_wrapper;

  std::mutex m_mutex_operation;
  std::tuple<t_params...> m_params;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_wait;
  //  std::optional<t_result> m_worker_result;
  //  worker_result m_worker_result;
  concurrent::thread m_thread;
  std::mutex m_mutex_exec;
  bool m_is_timeout{false};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_RUNNER_H

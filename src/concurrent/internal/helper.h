#ifndef TENACITAS_CONCURRENT_HELPER_H
#define TENACITAS_CONCURRENT_HELPER_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include <concurrent/thread.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

typedef std::function<void(std::thread::id)> timeout_callback;

void timeout_callback_thread(timeout_callback p_timeout_callback,
                             std::thread::id &p_id) {
  std::thread _timeout_thread(
      [p_timeout_callback, p_id]() -> void { p_timeout_callback(p_id); });
  _timeout_thread.detach();
}

template <typename t_log, typename t_worker, typename t_worker_result,
          typename t_worker_timeout, typename t_provider_executer,
          typename t_worker_params>
struct helper_t {

  ~helper_t() {
    concurrent_debug(m_log, "destructor");
    stop();
  }

  void start() {
    if (m_stopped) {
      concurrent_debug(m_log, "starting");
      m_stopped = false;
      m_thread = concurrent::thread([this]() -> void { loop(); });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      m_provider_executer.start();
    }
  }

  void stop() {
    if (!m_stopped) {
      concurrent_debug(m_log, "stopping");
      m_provider_executer.stop();
      m_stopped = true;
      m_cond_exec.notify_one();
      m_cond_wait.notify_one();
      m_thread.join();
    }
  }

  std::optional<t_worker_result> operator()() {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    concurrent_debug(m_log, "calling provider");
    std::optional<t_worker_params> _provider_result = m_provider_executer();
    if (!_provider_result) {
      concurrent_info(m_log, "provider did not return data");
      return {};
    }

    m_params = _provider_result.value();
    concurrent_debug(m_log, "provider returned = ", m_params);

    m_is_timeout = false;

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      m_is_timeout = true;
      timeout_callback_thread(m_timeout_callback, m_thread.get_id());
      return {};
    }

    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return {};
    }

    if (m_worker_result) {
      t_worker_result _result(m_worker_result.value());
      concurrent_debug(m_log, "worker finished on time: result = ", _result);
      return {_result};
    }

    return {};
  }

private:
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
      m_worker_result = std::apply(m_worker, std::move(m_params));

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
  t_worker m_worker;
  t_worker_timeout m_timeout;
  t_provider_executer m_provider_executer;
  timeout_callback m_timeout_callback;
  bool m_stopped{true};
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  concurrent::thread m_thread;
  std::mutex m_mutex_operation;
  std::optional<t_worker_result> m_worker_result;
  t_worker_params m_params;
  bool m_is_timeout{false};
  t_log m_log{"concurrent::executer<t_result, t_params...>"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // HELPER_H

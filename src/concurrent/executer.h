#ifndef TENACITAS_CONCURRENT_EXECUTER_H
#define TENACITAS_CONCURRENT_EXECUTER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <tuple>
#include <utility>

#include <concurrent/internal/log.h>
#include <concurrent/thread.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_params... are the types of the parameters for the work function
///
template <typename t_time, typename t_log, typename... t_params>
struct executer_t {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef std::function<bool(t_params...)> worker;

  executer_t(worker p_worker, t_time p_timeout)
      : m_worker(p_worker), m_timeout(p_timeout) {
    start();
  }

  executer_t(const executer_t &) = delete;
  executer_t(executer_t &&) = delete;

  executer_t &operator=(const executer_t &) = delete;
  executer_t &operator=(executer_t &&) = delete;

  ~executer_t() {
    concurrent_debug(m_log, "destructor");
    stop();
  }

  void start() {
    concurrent_debug(m_log, "starting");
    m_stopped = false;
    m_thread = concurrent::thread([this]() -> void { loop(); });
  }

  void stop() {
    concurrent_debug(m_log, "stoping");
    m_stopped = true;
    m_cond_exec.notify_one();
    m_cond_wait.notify_one();
    m_thread.join();
  }

  bool operator()(t_params... p_params) {
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return false;
    }

    m_params = std::make_tuple(p_params...);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_waiting = true;
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout, [this]() -> bool {
          return m_stopped || m_done;
        })) {
      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        m_waiting = false;
        return false;
      }
      concurrent_debug(m_log, "worker finished on time: result = ", m_result);
      return m_result;

    } else {
      concurrent_warn(m_log, "worker did not finish in time");
      m_waiting = false;
      return false;
    }
  }

private:
  void loop() {
    while (true) {
      concurrent_debug(m_log, "waiting notification to work");

      std::unique_lock<std::mutex> _lock(m_mutex_exec);
      m_cond_exec.wait(_lock, [this]() -> bool { return m_stopped; });

      concurrent_debug(m_log, "notification arrived");

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        if (m_waiting) {
          concurrent_debug(m_log, "result was still being expected");
          _lock.unlock();
          m_cond_wait.notify_one();
        }
        break;
      }

      concurrent_debug(m_log, "starting to work");
      m_done = false;
      m_result = std::apply(m_worker, std::move(m_params));

      if (m_stopped) {
        concurrent_debug(m_log, "stopped while working");
        _lock.unlock();
        m_cond_wait.notify_one();
        break;
      }

      if (!m_waiting) {
        concurrent_debug(m_log, "no one is waiting anymnore for the result");
        _lock.unlock();
      } else {
        concurrent_debug(m_log, "notifying that work is done");
        m_done = true;
        _lock.unlock();
        m_cond_wait.notify_one();
      }
    }
  }

private:
  worker m_worker;
  t_time m_timeout;
  bool m_result{true};
  bool m_waiting{true};
  bool m_done{false};
  std::tuple<t_params...> m_params;
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  bool m_stopped{true};
  concurrent::thread m_thread;
  t_log m_log{"concurrent::executer"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // WORK_H

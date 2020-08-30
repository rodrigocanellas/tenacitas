#ifndef TENACITAS_CONCURRENT_EXECUTER_H
#define TENACITAS_CONCURRENT_EXECUTER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <condition_variable>
#include <functional>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>

#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>

///// \brief copies a tuple fields into a string
// template <class TupType, size_t... I>
// std::string to_str(const TupType &_tup, std::index_sequence<I...>) {
//  std::stringstream _stream;
//  _stream << "(";
//  (..., (_stream << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
//  _stream << ")";
//  return _stream.str();
//}

///// \brief copies one tuple field into a string
// template <class... T> std::string to_str(const std::tuple<T...> &_tup) {
//  return to_str(_tup, std::make_index_sequence<sizeof...(T)>());
//}

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
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
  typedef typename traits_t<t_params...>::worker worker;

  typedef typename traits_t<t_params...>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker, provider p_provider, t_time p_timeout)
      : m_worker(p_worker), m_provider(p_provider), m_timeout(p_timeout) {
    start();
  }

  executer_t(const executer_t &) = delete;
  executer_t(executer_t &&) = delete;

  executer_t &operator=(const executer_t &) = delete;
  executer_t &operator=(executer_t &&) = delete;

  void *operator new(size_t) = delete;
  void *operator new[](size_t) = delete;

  ~executer_t() {
    concurrent_debug(m_log, "destructor");
    stop();
  }

  void start() {
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

  inline bool is_stopped() const { return m_stopped; }

  inline bool is_started() const { return !m_stopped; }

  bool operator()() {
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return false;
    }

    concurrent_debug(m_log, "calling provider");
    std::optional<std::tuple<t_params...>> _provider_result = m_provider();
    if (!_provider_result) {
      concurrent_info(m_log, "provider did not return data");
      return false;
    }

    m_params = _provider_result.value();
    concurrent_debug(m_log, "provider returned = ", m_params);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "worker did not finish in time");
      return false;
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return false;
    }
    concurrent_debug(m_log, "worker finished on time: result = ", m_result);
    return m_result;
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
      m_result = std::apply(m_worker, std::move(m_params));

      if (m_stopped) {
        concurrent_debug(m_log, "stopped while working");
        m_cond_wait.notify_one();
        break;
      }

      concurrent_debug(m_log, "notifying that work is done");
      m_cond_wait.notify_one();
    }
  }

private:
  worker m_worker;
  provider m_provider;
  t_time m_timeout;
  bool m_result{true};
  std::tuple<t_params...> m_params;
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  bool m_stopped{true};
  concurrent::thread m_thread;
  t_log m_log{"concurrent::executer"};
};

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
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
template <typename t_time, typename t_log>
struct executer_t<t_time, t_log, void> {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef typename traits_t<void>::worker worker;

  typedef typename traits_t<void>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker, t_time p_timeout)
      : m_worker(p_worker), m_timeout(p_timeout) {
    start();
  }

  executer_t(const executer_t &) = delete;
  executer_t(executer_t &&) = delete;

  executer_t &operator=(const executer_t &) = delete;
  executer_t &operator=(executer_t &&) = delete;

  void *operator new(size_t) = delete;
  void *operator new[](size_t) = delete;

  ~executer_t() {
    concurrent_debug(m_log, "destructor");
    stop();
  }

  void start() {
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

  inline bool is_stopped() const { return m_stopped; }

  inline bool is_started() const { return !m_stopped; }

  bool operator()() {
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return false;
    }

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "worker did not finish in time");
      return false;
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return false;
    }
    concurrent_debug(m_log, "worker finished on time: result = ", m_result);
    return m_result;
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
      m_result = m_worker();

      if (m_stopped) {
        concurrent_debug(m_log, "stopped while working");
        m_cond_wait.notify_one();
        break;
      }

      concurrent_debug(m_log, "notifying that work is done");
      m_cond_wait.notify_one();
    }
  }

private:
  worker m_worker;
  t_time m_timeout;
  bool m_result{true};
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

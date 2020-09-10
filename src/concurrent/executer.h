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

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_time, typename t_result,
          typename... t_params>
struct executer_t;

template <typename t_log, typename t_time, typename t_result, typename t_param>
struct executer_t<t_log, t_time, t_result, t_param>;

template <typename t_log, typename t_time, typename t_result>
struct executer_t<t_log, t_time, t_result, void>;

template <typename t_log, typename t_time, typename... t_params>
struct executer_t<t_log, t_time, void, t_params...>;

template <typename t_log, typename t_time, typename t_param>
struct executer_t<t_log, t_time, void, t_param>;

template <typename t_log, typename t_time>
struct executer_t<t_log, t_time, void, void>;

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// This executer executes a work function that returns a result, and expects
/// parameters
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_result is the type of object that the work function returns
///
/// \tparam t_params... are the types of the parameters for the work function
///
template <typename t_log, typename t_time, typename t_result,
          typename... t_params>
struct executer_t {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef typename traits_t<t_result, t_params...>::worker worker;

  typedef typename traits_t<t_result, t_params...>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker, t_time p_timeout, provider p_provider)
      : m_worker(p_worker), m_timeout(p_timeout),
        m_provider_executer(p_provider, std::chrono::seconds(1)) {
    //    m_provider(p_provider) {
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

  inline t_time get_timeout() const { return m_timeout; }

  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

  inline worker get_worker() const { return m_worker; }

  std::optional<t_result> operator()(t_params... p_params) {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    m_params = std::make_tuple(p_params...);
    concurrent_debug(m_log, "parameters provided = ", m_params);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return {};
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return {};
    }
    if (m_worker_result) {
      t_result _result(m_worker_result.value());
      concurrent_debug(m_log, "worker finished on time: result = ", _result);
      return {_result};
    }
    return {};
  }

  std::optional<t_result> operator()() {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    concurrent_debug(m_log, "calling provider");
    std::optional<std::tuple<t_params...>> _provider_result =
        m_provider_executer();
    if (!_provider_result) {
      concurrent_info(m_log, "provider did not return data");
      return {};
    }

    m_params = _provider_result.value();
    concurrent_debug(m_log, "provider returned = ", m_params);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return {};
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return {};
    }
    if (m_worker_result) {
      t_result _result(m_worker_result.value());
      concurrent_debug(m_log, "worker finished on time: result = ", _result);
      return {_result};
    }
    return {};
  }

private:
  typedef executer_t<t_log, std::chrono::seconds, std::tuple<t_params...>, void>
      provider_executer;

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
  provider_executer m_provider_executer;
  bool m_stopped{true};
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  concurrent::thread m_thread;
  std::mutex m_mutex_operation;
  std::optional<t_result> m_worker_result;
  std::tuple<t_params...> m_params;
  t_log m_log{"concurrent::executer<t_result, t_params...>"};
};

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// This executer executes a work function that returns a result, and expects
/// one parameter
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_result is the type of object that the work function returns
///
/// \tparam t_params is the type of the parameter for the work function
///
template <typename t_log, typename t_time, typename t_result, typename t_param>
struct executer_t<t_log, t_time, t_result, t_param> {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef typename traits_t<t_result, t_param>::worker worker;

  typedef typename traits_t<t_result, t_param>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker, t_time p_timeout, provider p_provider)
      : m_worker(p_worker), m_timeout(p_timeout),
        m_provider_executer(p_provider, std::chrono::seconds(1)) {
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

  inline t_time get_timeout() const { return m_timeout; }

  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

  inline worker get_worker() const { return m_worker; }

  std::optional<t_result> operator()(t_param &&p_param) {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    m_param = std::move(p_param);
    concurrent_debug(m_log, "parameters provided = ", m_param);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return {};
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return {};
    }
    if (m_worker_result) {
      t_result _result(m_worker_result.value());
      concurrent_debug(m_log, "worker finished on time: result = ", _result);
      return {_result};
    }
    return {};
  }

  std::optional<t_result> operator()() {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    concurrent_debug(m_log, "calling provider");
    std::optional<t_param> _provider_result = m_provider_executer();
    if (!_provider_result) {
      concurrent_info(m_log, "provider did not return data");
      return {};
    }

    m_param = std::move(_provider_result.value());
    concurrent_debug(m_log, "provider returned = ", m_param);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return {};
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return {};
    }
    if (m_worker_result) {
      t_result _result(m_worker_result.value());
      concurrent_debug(m_log, "worker finished on time: result = ", _result);
      return {_result};
    }
    return {};
  }

private:
  typedef executer_t<t_log, std::chrono::seconds, t_param, void>
      provider_executer;

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
      m_worker_result = m_worker(std::move(m_param));

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
  provider_executer m_provider_executer;
  bool m_stopped{true};
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  concurrent::thread m_thread;
  std::mutex m_mutex_operation;
  std::optional<t_result> m_worker_result;
  t_param m_param;
  t_log m_log{"concurrent::executer<t_result, t_param>"};
};

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// This executer executes a work function that returns a result, but requires
/// no parameters
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_result is the type of object that the work function returns
///
template <typename t_log, typename t_time, typename t_result>
struct executer_t<t_log, t_time, t_result, void> {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef typename traits_t<t_result, void>::worker worker;

  //  typedef typename traits_t<t_result, void>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker,
             t_time p_timeout /*, provider p_provider = []() {}*/)
      : m_worker(p_worker), m_timeout(p_timeout) {
    //    provider _provider = p_provider;
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

  inline t_time get_timeout() const { return m_timeout; }

  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

  inline worker get_worker() const { return m_worker; }

  std::optional<t_result> operator()() {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return {};
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return {};
    }
    if (m_worker_result) {
      t_result _result(m_worker_result.value());
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
      m_worker_result = m_worker();

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
  bool m_stopped{true};
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  concurrent::thread m_thread;
  std::mutex m_mutex_operation;
  std::optional<t_result> m_worker_result;
  t_log m_log{"concurrent::executer<t_result, void>"};
};

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// This executer executes a work function that does not return an object, and
/// expects parameters
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_params... are the types of the parameters for the work function
///
template <typename t_log, typename t_time, typename... t_params>
struct executer_t<t_log, t_time, void, t_params...> {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef typename traits_t<void, t_params...>::worker worker;

  typedef typename traits_t<void, t_params...>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker, t_time p_timeout, provider p_provider)
      : m_worker(p_worker), m_timeout(p_timeout),
        m_provider_executer(p_provider, std::chrono::seconds(1)) {
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

  inline t_time get_timeout() const { return m_timeout; }

  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

  inline worker get_worker() const { return m_worker; }

  void operator()(t_params... p_params) {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return;
    }

    m_params = std::make_tuple(p_params...);
    concurrent_debug(m_log, "parameters provided = ", m_params);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return;
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return;
    }

    concurrent_debug(m_log, "worker finished on time");
  }

  void operator()() {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return;
    }

    concurrent_debug(m_log, "calling provider");
    std::optional<std::tuple<t_params...>> _provider_result =
        m_provider_executer();
    if (!_provider_result) {
      concurrent_info(m_log, "provider did not return data");
      return;
    }

    m_params = _provider_result.value();
    concurrent_debug(m_log, "provider returned = ", m_params);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return;
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return;
    }

    concurrent_debug(m_log, "worker finished on time");
  }

private:
  typedef executer_t<t_log, std::chrono::seconds, std::tuple<t_params...>, void>
      provider_executer;

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
      std::apply(m_worker, std::move(m_params));

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
  provider_executer m_provider_executer;
  bool m_stopped{true};
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  concurrent::thread m_thread;
  std::mutex m_mutex_operation;
  std::tuple<t_params...> m_params;
  t_log m_log{"concurrent::executer<void, t_params>"};
};

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// This executer executes a work function that does not return an object, and
/// expects parameters
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
/// \tparam t_param is the type of the parameter for the work function
///
template <typename t_log, typename t_time, typename t_param>
struct executer_t<t_log, t_time, void, t_param> {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef typename traits_t<void, t_param>::worker worker;

  typedef typename traits_t<void, t_param>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker, t_time p_timeout, provider p_provider)
      : m_worker(p_worker), m_timeout(p_timeout),
        m_provider_executer(p_provider, std::chrono::seconds(1)) {
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

  inline t_time get_timeout() const { return m_timeout; }

  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

  inline worker get_worker() const { return m_worker; }

  void operator()(t_param &&p_param) {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return;
    }

    m_param = std::move(p_param);
    concurrent_debug(m_log, "parameters provided = ", m_param);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return;
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return;
    }

    concurrent_debug(m_log, "worker finished on time");
  }

  void operator()() {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return;
    }

    concurrent_debug(m_log, "calling provider");
    std::optional<t_param> _provider_result = m_provider_executer();
    if (!_provider_result) {
      concurrent_info(m_log, "provider did not return data");
      return;
    }

    m_param = std::move(_provider_result.value());
    concurrent_debug(m_log, "provider returned = ", m_param);

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return;
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return;
    }

    concurrent_debug(m_log, "worker finished on time");
  }

private:
  typedef executer_t<t_log, std::chrono::seconds, t_param, void>
      provider_executer;

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
      m_worker(std::move(m_param));

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
  provider_executer m_provider_executer;
  bool m_stopped{true};
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  concurrent::thread m_thread;
  std::mutex m_mutex_operation;
  t_param m_param;
  t_log m_log{"concurrent::executer<void, t_param>"};
};

/// \brief executer_t allows timeout control of a work function without
/// starting a new thread for each execution
///
/// This executer executes a work function that does not return an object, and
/// expects no parameters
///
/// It can only be used as a data member or as a local object, as copy
/// constructor, copy assignment, move constructor, move assignment, and \p
/// new operators are deleted. This is to avoid the need for locking the entire
/// execution of the work function
///
/// \tparam t_log must implement:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
template <typename t_log, typename t_time>
struct executer_t<t_log, t_time, void, void> {

  /// \brief work function that will be executed, and its time of execution
  /// controlled
  typedef typename traits_t<void, void>::worker worker;

  //  typedef typename traits_t<void, void>::provider provider;

  /// \brief constructor
  executer_t(worker p_worker,
             t_time p_timeout /*, provider p_provider = []() {}*/)
      : m_worker(p_worker), m_timeout(p_timeout) {
    //    provider _provider = p_provider;
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

  inline t_time get_timeout() const { return m_timeout; }

  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

  inline worker get_worker() const { return m_worker; }

  void operator()() {
    std::lock_guard<std::mutex> _lock_operation(m_mutex_operation);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, "executer is stopped; call 'start()' first");
      return;
    }

    concurrent_debug(m_log, "notifying there is work to be done ");
    m_cond_exec.notify_one();

    concurrent_debug(m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(m_mutex_wait);
    if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
      concurrent_warn(m_log, "TIMEOUT: worker did not finish in time");
      return;
    }
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return;
    }
    concurrent_debug(m_log, "worker finished on time");
    return;
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
      m_worker();

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
  bool m_stopped{true};
  std::condition_variable m_cond_wait;
  std::mutex m_mutex_wait;
  std::condition_variable m_cond_exec;
  std::mutex m_mutex_exec;
  concurrent::thread m_thread;
  std::mutex m_mutex_operation;
  t_log m_log{"concurrent::executer<void, void>"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_EXECUTER_H

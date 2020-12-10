#ifndef TENACITAS_CONCURRENT_EXECUTER_H
#define TENACITAS_CONCURRENT_EXECUTER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include <concurrent/internal/constants.h>
#include <concurrent/internal/function_wrapper.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/time_unit.h>
#include <concurrent/timeout_callback.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

enum class timeout_control : char { yes = 'y', no = 'n' };

template <typename t_log> struct executer_base_t {

  executer_base_t(const executer_base_t &) = delete;
  executer_base_t(executer_base_t &&) = delete;
  executer_base_t &operator=(const executer_base_t &) = delete;
  executer_base_t &operator=(executer_base_t &&) = delete;

  /// \brief destructor
  ///
  /// Stops the execution of the single created thread for the execution of the
  /// function asynchronously
  virtual ~executer_base_t() {
    concurrent_debug(m_log, "entering destructor");
    stop();
    concurrent_debug(m_log, "leaving destructor");
  }

  /// \brief Stars the single thread that will execute the function
  /// asynchronously
  void start() {
    if (m_stopped) {
      concurrent_debug(m_log, "starting");
      m_stopped = false;
      m_thread = concurrent::thread([this]() -> void { loop(); });
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

  /// \brief Stops the execution of the single created thread for the execution
  /// of the function asynchronously
  void stop() {
    if (!m_stopped) {
      concurrent_debug(m_log, "stopping");
      m_stopped = true;
      concurrent_debug(m_log, "m_stopped = true");
      m_cond_exec.notify_one();
      concurrent_debug(m_log, "m_cond_exec.notify_one()");
      m_cond_wait.notify_one();
      concurrent_debug(m_log, "m_cond_wait.notify_one()");
      m_thread.join();
      concurrent_debug(m_log, "m_thread.join()");
    } else {
      concurrent_debug(m_log, "not stopping because it was already stopped");
    }
  }

protected:
  executer_base_t() = default;

  virtual void loop() = 0;

protected:
  /// \brief Logger object
  t_log m_log{"concurrent::executer"};

  /// \brief The loop initiates stopped
  bool m_stopped{true};

  /// \brief Controls the execution of the execution loop
  std::mutex m_mutex_function;

  /// \brief Controls the execution of the execution loop
  std::condition_variable m_cond_exec;

  /// \brief Controls the waiting of the execution loop
  std::mutex m_mutex_wait;

  /// \brief Controls the waiting of the execution loop
  std::condition_variable m_cond_wait;

  /// \brief Single thread used to execute the \p function over and over
  /// asynchronously
  concurrent::thread m_thread;

  /// \brief Controls the execution of the thread
  std::mutex m_mutex_exec;
};

template <typename t_log, timeout_control, typename t_result,
          typename... t_params>
struct executer_t;

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::no, t_result, t_params...>;

//// ########## 2 ##########
// template <typename t_log, typename t_result, typename t_params>
// struct executer_t<t_log, timeout_control::no, t_result, t_params>;

// ########## 3 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, timeout_control::no, t_result, void>;

// ########## 4 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, timeout_control::no, void, t_params...>;

// ########## 5 ##########
template <typename t_log, typename t_params>
struct executer_t<t_log, timeout_control::no, void, t_params>;

// ########## 6 ##########
template <typename t_log>
struct executer_t<t_log, timeout_control::no, void, void>;

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::no, t_result, t_params...>
    : public executer_base_t<t_log> {
  typedef std::function<t_result(t_params...)> function;

  executer_t(function p_function) : m_function(p_function) {}

  std::optional<t_result> operator()(t_params... p_params) {
    std::lock_guard<std::mutex> _lock_function(this->m_mutex_function);
    concurrent_debug(this->m_log, "operator()()");

    if (this->m_stopped) {
      concurrent_warn(this->m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    m_params = {p_params...};

    concurrent_debug(this->m_log, "notifying there is work to be done");
    this->m_cond_exec.notify_one();

    concurrent_debug(this->m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(this->m_mutex_wait);
    this->m_cond_wait.wait(_lock);

    concurrent_debug(this->m_log, "work was done or 'executer' was stopped");
    if (this->m_stopped) {
      concurrent_debug(this->m_log, "stopped");
      return {};
    }

    concurrent_debug(this->m_log, "function done");
    return m_result;
  }

private:
  void loop() override {
    while (true) {

      concurrent_debug(this->m_log, "waiting for work");

      std::unique_lock<std::mutex> _lock(this->m_mutex_exec);
      this->m_cond_exec.wait(_lock);

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        this->m_cond_wait.notify_one();
        concurrent_debug(this->m_log, "about to break the loop");
        break;
      }

      concurrent_debug(this->m_log, "function to execute!");

      m_result = std::apply(m_function, std::move(m_params));
      concurrent_debug(m_log, "function returned ", m_result);

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped while in function");
        this->m_cond_wait.notify_one();
        break;
      }

      concurrent_debug(this->m_log, "notifying that work is done");
      this->m_cond_wait.notify_one();
      concurrent_debug(this->m_log, "notification sent");
    }

    concurrent_debug(this->m_log, "leaving the loop");
  }

private:
  function m_function;

  std::tuple<t_params...> m_params;

  t_result m_result;

  t_log m_log{"executer"};
};

//// ########## 2 ##########
// template <typename t_log, typename t_result, typename t_params>
// struct executer_t<t_log, timeout_control::no, t_result, t_params> {
//  typedef std::function<t_result(t_params)> function;

//  executer_t() = delete;
//  executer_t(const executer_t &) = delete;
//  executer_t(executer_t &&) = delete;
//  executer_t &operator=(const executer_t &) = delete;
//  executer_t &operator=(executer_t &&) = delete;

//  executer_t(function p_function) : m_function(p_function) {}

//  t_result operator()(t_params p_params) { return m_function(p_params); }

// private:
//  function m_function;

//  t_log m_log{"executer"};
//};

// ########## 3 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, timeout_control::no, t_result, void>
    : public executer_base_t<t_log> {
  typedef std::function<t_result()> function;

  executer_t(function p_function) : m_function(p_function) {}

  std::optional<t_result> operator()() {
    std::lock_guard<std::mutex> _lock_function(this->m_mutex_function);
    concurrent_debug(this->m_log, "operator()()");

    if (this->m_stopped) {
      concurrent_warn(this->m_log, "executer is stopped; call 'start()' first");
      return {};
    }

    concurrent_debug(this->m_log, "notifying there is work to be done");
    this->m_cond_exec.notify_one();

    concurrent_debug(this->m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(this->m_mutex_wait);
    this->m_cond_wait.wait(_lock);

    concurrent_debug(this->m_log, "work was done or 'executer' was stopped");
    if (this->m_stopped) {
      concurrent_debug(this->m_log, "stopped");
      return {};
    }

    concurrent_debug(this->m_log, "function done");
    return m_result;
  }

private:
  void loop() override {
    while (true) {

      concurrent_debug(this->m_log, "waiting for work");

      std::unique_lock<std::mutex> _lock(this->m_mutex_exec);
      this->m_cond_exec.wait(_lock);

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        this->m_cond_wait.notify_one();
        concurrent_debug(this->m_log, "about to break the loop");
        break;
      }

      concurrent_debug(this->m_log, "function to execute!");

      m_result = m_function();

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped while in function");
        this->m_cond_wait.notify_one();
        break;
      }

      concurrent_debug(this->m_log, "notifying that work is done");
      this->m_cond_wait.notify_one();
      concurrent_debug(this->m_log, "notification sent");
    }

    concurrent_debug(this->m_log, "leaving the loop");
  }

private:
  function m_function;

  t_result m_result;

  t_log m_log{"executer"};
};

// ########## 4 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, timeout_control::no, void, t_params...> {
  typedef std::function<void(t_params...)> function;

  executer_t() = delete;
  executer_t(const executer_t &) = delete;
  executer_t(executer_t &&) = delete;
  executer_t &operator=(const executer_t &) = delete;
  executer_t &operator=(executer_t &&) = delete;

  executer_t(function p_function) : m_function(p_function) {}

  void operator()(t_params... p_params) { return m_function(p_params...); }

private:
  function m_function;

  t_log m_log{"executer"};
};

// ########## 5 ##########
template <typename t_log, typename t_params>
struct executer_t<t_log, timeout_control::no, void, t_params> {
  typedef std::function<void(t_params)> function;

  executer_t() = delete;
  executer_t(const executer_t &) = delete;
  executer_t(executer_t &&) = delete;
  executer_t &operator=(const executer_t &) = delete;
  executer_t &operator=(executer_t &&) = delete;

  executer_t(function p_function) : m_function(p_function) {}

  void operator()(t_params p_params) { return m_function(p_params); }

private:
  function m_function;

  t_log m_log{"executer"};
};

// ########## 6 ##########
template <typename t_log>
struct executer_t<t_log, timeout_control::no, void, void> {
  typedef std::function<void()> function;

  executer_t() = delete;
  executer_t(const executer_t &) = delete;
  executer_t(executer_t &&) = delete;
  executer_t &operator=(const executer_t &) = delete;
  executer_t &operator=(executer_t &&) = delete;

  executer_t(function p_function) : m_function(p_function) {}

  void operator()() { return m_function(); }

private:
  function m_function;

  t_log m_log{"executer"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_EXECUTER_H

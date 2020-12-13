#ifndef TENACITAS_CONCURRENT_EXECUTER_H
#define TENACITAS_CONCURRENT_EXECUTER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <list>
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

template <typename t_log> struct executer_no_timeout_base_t {

  executer_no_timeout_base_t(const executer_no_timeout_base_t &) = delete;
  executer_no_timeout_base_t(executer_no_timeout_base_t &&) = delete;
  executer_no_timeout_base_t &
  operator=(const executer_no_timeout_base_t &) = delete;
  executer_no_timeout_base_t &operator=(executer_no_timeout_base_t &&) = delete;

  /// \brief destructor
  ///
  /// Stops the execution of the single created thread for the execution of the
  /// function asynchronously
  virtual ~executer_no_timeout_base_t() {
    concurrent_debug(m_log, "entering destructor");
    stop();
    concurrent_debug(m_log, "leaving destructor");
  }

protected:
  struct thread_controler {};

protected:
  executer_no_timeout_base_t(std::function<void()> p_function)
      : m_function(p_function) {}

  /// \brief Stars the single thread that will execute the function
  /// asynchronously
  void start() {
    if (m_stopped) {
      concurrent_debug(m_log, "starting");
      m_stopped = false;
      m_thread = concurrent::thread([this]() -> void { looping(); });
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

  template <typename t_result>
  t_result call_no_timeout(std::function<t_result()> p_ok,
                           std::function<t_result()> p_not_ok,
                           std::function<void()> p_save_params) {
    std::lock_guard<std::mutex> _lock_function(this->m_mutex_function);
    concurrent_debug(this->m_log, "operator()()");

    if (this->m_stopped) {
      concurrent_warn(this->m_log, "executer is stopped; call 'start()' first");
      return p_not_ok();
    }

    p_save_params();

    concurrent_debug(this->m_log, "notifying there is work to be done");
    this->m_cond_exec.notify_one();

    concurrent_debug(this->m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(this->m_mutex_wait);
    this->m_cond_wait.wait(_lock);

    concurrent_debug(this->m_log, "work was done or 'executer' was stopped");
    if (this->m_stopped) {
      concurrent_debug(this->m_log, "stopped");
      return p_not_ok();
    }

    concurrent_debug(this->m_log, "function done");
    return p_ok();
  }

  virtual void looping() {
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

      m_function();

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

protected:
  std::function<void()> m_function;

  /// \brief Logger object
  t_log m_log{"executer"};

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

template <typename t_log>
struct executer_with_timeout_base_t : executer_no_timeout_base_t<t_log> {

  virtual ~executer_with_timeout_base_t() {}

protected:
  template <typename t_time>
  executer_with_timeout_base_t(std::function<void()> p_function,
                               t_time p_timeout,
                               timeout_callback p_timeout_callback)
      : executer_no_timeout_base_t<t_log>(p_function),
        m_timeout(to_timeout(p_timeout)),
        m_timeout_callback(p_timeout_callback) {}

  template <typename t_result>
  t_result call_with_timeout(std::function<t_result()> p_ok,
                             std::function<t_result()> p_not_ok,
                             std::function<void()> p_save_params) {
    std::lock_guard<std::mutex> _lock_function(this->m_mutex_function);
    concurrent_debug(this->m_log, "operator()()");

    if (this->m_stopped) {
      concurrent_warn(this->m_log, "executer is stopped; call 'start()' first");
      return p_not_ok();
    }

    if (!m_function_returned) {
      this->start();
    }

    p_save_params();

    this->m_is_timeout = false;

    concurrent_debug(this->m_log, "notifying there is work to be done");
    this->m_cond_exec.notify_one();

    concurrent_debug(this->m_log, "waiting for work to be done");
    std::unique_lock<std::mutex> _lock(this->m_mutex_wait);
    if (this->m_cond_wait.wait_for(_lock, this->m_timeout) ==
        std::cv_status::timeout) {
      concurrent_warn(this->m_log, "timeout!");
      this->m_is_timeout = true;
      this->timeout_callback_thread();
      return p_not_ok();
    }

    concurrent_debug(this->m_log, "work was done or 'executer' was stopped");
    if (this->m_stopped) {
      concurrent_debug(this->m_log, "stopped");
      return p_not_ok();
    }

    concurrent_debug(this->m_log, "function done");
    return p_ok();
  }

  void looping() override {
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

      if (!this->m_is_timeout) {
        concurrent_debug(this->m_log, "function to execute!");

        this->m_function();

        if (this->m_stopped) {
          concurrent_debug(this->m_log, "stopped while in function");
          this->m_cond_wait.notify_one();
          break;
        }

        m_function_returned = true;

        concurrent_debug(this->m_log, "notifying that work is done");
        this->m_cond_wait.notify_one();
        concurrent_debug(this->m_log, "notification sent");
      }
    }

    concurrent_debug(this->m_log, "leaving the loop");
  }

  /// \brief Thread to execute the function that is called if the \p function
  /// times out
  void timeout_callback_thread() {
    std::thread::id _id{std::this_thread::get_id()};
    m_timeout_thread =
        std::thread([this, _id]() -> void { m_timeout_callback(_id); });
    m_timeout_thread.detach();
  }

protected:
  timeout m_timeout;

  bool m_is_timeout{false};

  timeout_callback m_timeout_callback;

  std::thread m_timeout_thread;

  bool m_function_returned{false};
};

template <typename t_log, timeout_control, typename t_result,
          typename... t_params>
struct executer_t;

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::no, t_result, t_params...>;

// ########## 2 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, timeout_control::no, t_result, void>;

// ########## 3 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, timeout_control::no, void, t_params...>;

// ########## 4 ##########
template <typename t_log>
struct executer_t<t_log, timeout_control::no, void, void>;

// ########## 5 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::yes, t_result, t_params...>;

// ########## 6 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, timeout_control::yes, t_result, void>;

// ########## 7 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, timeout_control::yes, void, t_params...>;

// ########## 8 ##########
template <typename t_log>
struct executer_t<t_log, timeout_control::yes, void, void>;

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::no, t_result, t_params...>
    : public executer_no_timeout_base_t<t_log> {

  typedef std::function<t_result(t_params...)> function;

  executer_t(function p_function)
      : executer_no_timeout_base_t<t_log>([this, p_function]() {
          m_result = std::apply(p_function, std::move(m_params));
        }) {
    this->start();
  }

  std::optional<t_result> operator()(t_params... p_params) {

    auto ok = [this]() -> std::optional<t_result> { return m_result; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = [this, p_params...]() -> void {
      m_params = {p_params...};
    };

    return this->template call_no_timeout<std::optional<t_result>>(ok, not_ok,
                                                                   save_params);
  }

private:
  std::tuple<t_params...> m_params;

  t_result m_result;
};

// ########## 2 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, timeout_control::no, t_result, void>
    : public executer_no_timeout_base_t<t_log> {

  typedef std::function<t_result(void)> function;

  executer_t(function p_function)
      : executer_no_timeout_base_t<t_log>(
            [this, p_function]() { m_result = p_function(); }) {
    this->start();
  }

  std::optional<t_result> operator()() {

    auto ok = [this]() -> std::optional<t_result> { return m_result; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = []() -> void {};

    return this->template call_no_timeout<std::optional<t_result>>(ok, not_ok,
                                                                   save_params);
  }

private:
  t_result m_result;
};

// ########## 3 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, timeout_control::no, void, t_params...>
    : public executer_no_timeout_base_t<t_log> {

  typedef std::function<void(t_params...)> function;

  executer_t(function p_function)
      : executer_no_timeout_base_t<t_log>([this, p_function]() {
          std::apply(p_function, std::move(m_params));
        }) {
    this->start();
  }

  void operator()(t_params... p_params) {
    auto ok = [this]() -> void {};
    auto not_ok = []() -> void {};
    auto save_params = [this, p_params...]() -> void {
      m_params = {p_params...};
    };

    return this->template call_no_timeout<void>(ok, not_ok, save_params);
  }

private:
  std::tuple<t_params...> m_params;
};

// ########## 4 ##########
template <typename t_log>
struct executer_t<t_log, timeout_control::no, void, void>
    : public executer_no_timeout_base_t<t_log> {

  typedef std::function<void()> function;

  executer_t(function p_function)
      : executer_no_timeout_base_t<t_log>(
            [this, p_function]() { p_function(); }) {
    this->start();
  }

  void operator()() {
    auto ok = [this]() -> void {};
    auto not_ok = []() -> void {};
    auto save_params = []() -> void {};

    return this->template call_no_timeout<void>(ok, not_ok, save_params);
  }
};

// ########## 5 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t<t_log, timeout_control::yes, t_result, t_params...>
    : public executer_with_timeout_base_t<t_log> {

  typedef std::function<t_result(t_params...)> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_with_timeout_base_t<t_log>(
            [this, p_function]() -> void {
              m_result = std::apply(p_function, std::move(m_params));
            },
            p_timeout, p_timeout_callback) {
    this->start();
  }

  std::optional<t_result> operator()(t_params... p_params) {
    auto ok = [this]() -> std::optional<t_result> { return m_result; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = [this, p_params...]() -> void {
      m_params = {p_params...};
    };

    return this->template call_with_timeout<std::optional<t_result>>(
        ok, not_ok, save_params);
  }

private:
  std::tuple<t_params...> m_params;

  t_result m_result;
};

// ########## 6 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, timeout_control::yes, t_result, void>
    : public executer_with_timeout_base_t<t_log> {

  typedef std::function<t_result()> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_with_timeout_base_t<t_log>(
            [this, p_function]() -> void { m_result = p_function(); },
            p_timeout, p_timeout_callback) {
    this->start();
  }

  std::optional<t_result> operator()() {

    auto ok = [this]() -> std::optional<t_result> { return {m_result}; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = []() -> void {};

    this->template call_with_timeout<std::optional<t_result>>(ok, not_ok,
                                                              save_params);
  }

private:
  t_result m_result;
};

// ########## 7 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, timeout_control::yes, void, t_params...>
    : public executer_with_timeout_base_t<t_log> {

  typedef std::function<void(t_params...)> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_with_timeout_base_t<t_log>(
            [this, p_function]() -> void {
              std::apply(p_function, std::move(m_params));
            },
            p_timeout, p_timeout_callback) {
    this->start();
  }

  void operator()(t_params... p_params) {
    this->template call_with_timeout<void>(
        []() -> void { return; }, []() -> void { return; },
        [this, p_params...]() -> void { m_params = {p_params...}; });
  }

private:
  std::tuple<t_params...> m_params;
};

// ########## 8 ##########
template <typename t_log>
struct executer_t<t_log, timeout_control::yes, void, void>
    : public executer_with_timeout_base_t<t_log> {

  typedef std::function<void()> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_with_timeout_base_t<t_log>(
            [this, p_function]() -> void { p_function(); }, p_timeout,
            p_timeout_callback) {
    this->start();
  }

  void operator()() {
    auto ok = []() -> void {};
    auto not_ok = []() -> void {};
    auto save_params = []() -> void {};

    this->template call_with_timeout<void>(ok, not_ok, save_params);
  }
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_EXECUTER_H

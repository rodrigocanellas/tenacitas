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

#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/time_unit.h>
#include <concurrent/timeout_callback.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Base class to allow a function to be executed, with timeout control,
/// but not starting a thread for each execution
template <typename t_log> struct executer_base_t {

  virtual ~executer_base_t() {
    concurrent_debug(m_log, "entering destructor");
    stop();
    concurrent_debug(m_log, "leaving destructor");
  }

protected:
  template <typename t_time>
  executer_base_t(std::function<void()> p_function, t_time p_timeout,
                  timeout_callback p_timeout_callback)
      : m_function(p_function), m_timeout(to_timeout(p_timeout)),
        m_timeout_callback(p_timeout_callback) {
    concurrent_debug(m_log, "calling start from constructor");
    start();
  }

  /// \brief Stars the single thread that will execute the function
  /// asynchronously
  void start() {
    if (m_stopped) {
      concurrent_debug(m_log, "starting");
      m_stopped = false;
      concurrent_debug(m_log, "about to push a new 'loop'");
      m_loops.push_front({this});

      m_loop = m_loops.begin();
      m_loop->start();
      concurrent_debug(m_log, "new 'loop' pushed, ", &(*m_loop));
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

  /// \brief Stops the execution of the single created thread for the execution
  /// of the function asynchronously
  void stop() {
    if (!m_stopped) {
      concurrent_debug(this->m_log, "m_stopped = true");
      m_stopped = true;
      concurrent_debug(this->m_log, "m_cond_exec.notify_one()");
      m_cond_exec.notify_all();
      concurrent_debug(this->m_log, "m_cond_wait.notify_one()");
      m_cond_wait.notify_one();
    } else {
      concurrent_debug(this->m_log,
                       "not stopping because it was already stopped");
    }
  }

  inline void set_log_debug_level() { m_log.set_debug_level(); }
  inline void set_log_info_level() { m_log.set_info_level(); }
  inline void set_log_level() { m_log.set_warn_level(); }

  template <typename t_result>
  t_result call(std::function<t_result()> p_ok,
                std::function<t_result()> p_not_ok,
                std::function<void()> p_save_params) {
    std::lock_guard<std::mutex> _lock_function(m_mutex_function);
    concurrent_debug(m_log, "operator()()");

    if (m_stopped) {
      concurrent_error(m_log, "executer is stopped; call 'start()' first");
      return p_not_ok();
    }

    p_save_params();

    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return p_not_ok();
    }

    concurrent_debug(m_log, "notifying there is work to be done");
    m_cond_exec.notify_all();

    concurrent_debug(m_log, "waiting ", m_timeout.count(),
                     " microsecs for work");
    {
      std::unique_lock<std::mutex> _lock(m_mutex_wait);
      if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
        concurrent_warn(m_log, "timeout!");
        timeout_callback_thread(m_loop->id());
        m_loop->abandon();
        m_stopped = true;
        start();

        return p_not_ok();
      }
    }

    concurrent_debug(m_log, "work was done or 'executer' was stopped");
    if (m_stopped) {
      concurrent_debug(m_log, "stopped");
      return p_not_ok();
    }

    concurrent_debug(m_log, "function done");
    return p_ok();
  }

private:
  /// \brief Loop that will be executed asyncrhonously
  struct loop {
    loop() = default;
    loop(executer_base_t<t_log> *p_owner) : m_owner(p_owner) {
      //      m_log.set_debug();
      concurrent_debug(m_log, "m_owner = ", m_owner);
    }
    loop(const loop &) = delete;
    loop(loop &&p_loop) noexcept : m_owner(p_loop.m_owner) {
      concurrent_debug(m_log, "move constructor m_owner = ", m_owner);
    }

    loop &operator=(const loop &) = delete;
    loop &operator=(loop &&p_loop) = delete;

    inline std::thread::id id() const { return m_thread.get_id(); }

    inline ~loop() {
      concurrent_debug(m_log, "destructor before, m_owner = ", m_owner);
      if (m_owner == nullptr) {
        concurrent_debug(m_log, "m_owner is null, leaving destructor");
        return;
      }

      if (m_thread.joinable()) {
        concurrent_debug(m_log, "joining");
        m_owner->m_cond_exec.notify_one();
        m_thread.join();
      }
      m_owner = nullptr;
      concurrent_debug(m_log, "destructor after, m_owner = ", m_owner);
    }

    inline void abandon() {
      m_thread.detach();
      concurrent_debug(m_log, "abandoning, m_owner = ", m_owner);
      m_owner = nullptr;
    }

    void start() {
      m_thread = std::thread([this]() -> void { (*this)(); });
    }

    void operator()() {
      while (true) {

        if (!m_owner) {
          concurrent_debug(m_log, "owner is null");
          break;
        }

        if ((m_owner) && (m_owner->m_stopped)) {
          concurrent_debug(m_log, "stopped ");
          break;
        }

        if (m_owner) {
          std::unique_lock<std::mutex> _lock(m_owner->m_mutex_exec);
          if (m_owner) {
            concurrent_debug(m_log, "waiting for work");
            m_owner->m_cond_exec.wait(_lock);
            concurrent_debug(m_log, "not waiting for work anymore");
          }
        }

        if ((m_owner) && (m_owner->m_stopped)) {
          concurrent_debug(m_log, "stopped");
          break;
        }

        if (m_owner) {
          concurrent_debug(m_log, "executing");
          m_owner->m_function();
        }

        if ((m_owner) && (m_owner->m_stopped)) {
          concurrent_debug(m_log, "stopped");
          break;
        }

        if (m_owner) {
          concurrent_debug(m_log, "notifying");
          m_owner->m_cond_wait.notify_one();
        }
      }

      concurrent_debug(m_log, "leaving the loop");
    }

  private:
    executer_base_t<t_log> *m_owner{nullptr};

    std::thread m_thread;

    t_log m_log{"concurrent::executer::loop"};
  };

private:
  /// \brief Thread to execute the function that is called if the \p function
  /// times out
  void timeout_callback_thread(std::thread::id p_id) {
    m_timeout_thread =
        std::thread([this, p_id]() -> void { m_timeout_callback(p_id); });
    m_timeout_thread.detach();
  }

private:
  std::function<void()> m_function;

  timeout m_timeout;

  timeout_callback m_timeout_callback;

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

  /// \brief Controls the execution of the thread
  std::mutex m_mutex_exec;

  std::thread m_timeout_thread;

  std::list<loop> m_loops;

  typename std::list<loop>::iterator m_loop;

  /// \brief Single thread used to execute the \p function over and over
  /// asynchronously
  std::thread m_thread;
};

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t;

// ########## 2 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, t_result, void>;

// ########## 3 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, void, t_params...>;

// ########## 4 ##########
template <typename t_log> struct executer_t<t_log, void, void>;

// ########## 1 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t : public executer_base_t<t_log> {

  typedef std::function<t_result(t_params...)> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_worker]() -> void {
              m_result = std::apply(p_worker, std::move(m_params));
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

    return this->template call<std::optional<t_result>>(ok, not_ok,
                                                        save_params);
  }

private:
  std::tuple<t_params...> m_params;

  t_result m_result;
};

// ########## 2 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, t_result, void> : public executer_base_t<t_log> {

  typedef std::function<t_result()> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_worker]() -> void { m_result = p_worker(); }, p_timeout,
            p_timeout_callback) {
    this->start();
  }

  std::optional<t_result> operator()() {

    auto ok = [this]() -> std::optional<t_result> { return {m_result}; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = []() -> void {};

    return this->template call<std::optional<t_result>>(ok, not_ok,
                                                        save_params);
  }

private:
  t_result m_result;
};

// ########## 3 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, void, t_params...> : public executer_base_t<t_log> {

  typedef std::function<void(t_params...)> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_worker]() -> void {
              std::apply(p_worker, std::move(m_params));
            },
            p_timeout, p_timeout_callback) {
    this->start();
  }

  void operator()(t_params... p_params) {
    this->template call<void>(
        []() -> void { return; }, []() -> void { return; },
        [this, p_params...]() -> void { m_params = {p_params...}; });
  }

private:
  std::tuple<t_params...> m_params;
};

// ########## 4 ##########
template <typename t_log>
struct executer_t<t_log, void, void> : public executer_base_t<t_log> {

  typedef std::function<void()> worker;

  template <typename t_time>
  executer_t(worker p_worker, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>([this, p_worker]() -> void { p_worker(); },
                               p_timeout, p_timeout_callback) {
    this->start();
  }

  void operator()() {
    auto ok = []() -> void {};
    auto not_ok = []() -> void {};
    auto save_params = []() -> void {};

    this->template call<void>(ok, not_ok, save_params);
  }
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_EXECUTER_H

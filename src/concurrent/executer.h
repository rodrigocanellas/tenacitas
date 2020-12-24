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
    concurrent_debug(m_log, this, " - entering destructor");
    stop();
    concurrent_debug(m_log, this, " - leaving destructor");
  }

protected:
  template <typename t_time>
  executer_base_t(std::function<void()> p_function, t_time p_timeout,
                  timeout_callback p_timeout_callback)
      : m_function(p_function), m_timeout(to_timeout(p_timeout)),
        m_timeout_callback(p_timeout_callback) {
    concurrent_debug(m_log, this, " - calling start from constructor");
    start();
  }

  /// \brief Stars the single thread that will execute the function
  /// asynchronously
  void start() {
    if (m_stopped) {
      concurrent_debug(m_log, this, " - starting");
      m_stopped = false;
      concurrent_debug(m_log, this, " - about to push a new 'loop'");
      m_loops.push_front({this});

      m_loop = m_loops.begin();
      m_loop->start();
      concurrent_debug(m_log, this, " - new 'loop' pushed, ", &(*m_loop));
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

  /// \brief Stops the execution of the single created thread for the execution
  /// of the function asynchronously
  void stop() {
    if (!m_stopped) {
      concurrent_debug(this->m_log, this, " - m_stopped = true");
      m_stopped = true;
      concurrent_debug(this->m_log, this, " - m_cond_exec.notify_one()");
      m_cond_exec.notify_all();
      concurrent_debug(this->m_log, this, " - m_cond_wait.notify_one()");
      m_cond_wait.notify_one();
    } else {
      concurrent_debug(this->m_log, this,
                       " - not stopping because it was already stopped");
    }
  }

  template <typename t_result>
  t_result call(std::function<t_result()> p_ok,
                std::function<t_result()> p_not_ok,
                std::function<void()> p_save_params) {
    std::lock_guard<std::mutex> _lock_function(m_mutex_function);
    concurrent_debug(m_log, this, " - operator()()");

    if (m_stopped) {
      concurrent_warn(m_log, this,
                      " - executer is stopped; call 'start()' first");
      return p_not_ok();
    }

    p_save_params();

    if (m_stopped) {
      concurrent_warn(m_log, this, " - stopped");
      return p_not_ok();
    }

    concurrent_debug(m_log, this, " - notifying there is work to be done");
    m_cond_exec.notify_all();

    concurrent_debug(m_log, this, " - waiting ", m_timeout.count(),
                     " microsecs for work");
    {
      std::unique_lock<std::mutex> _lock(m_mutex_wait);
      if (m_cond_wait.wait_for(_lock, m_timeout) == std::cv_status::timeout) {
        concurrent_warn(m_log, this, " - timeout!");
        timeout_callback_thread(m_loop->id());
        m_loop->abandon();
        m_stopped = true;
        start();

        return p_not_ok();
      }
    }

    concurrent_debug(m_log, this, " - work was done or 'executer' was stopped");
    if (m_stopped) {
      concurrent_debug(m_log, this, " - stopped");
      return p_not_ok();
    }

    concurrent_debug(m_log, this, " - function done");
    return p_ok();
  }

private:
  /// \brief Loop that will be executed asyncrhonously
  struct loop {
    loop() = default;
    loop(executer_base_t<t_log> *p_owner) : m_owner(p_owner) {
      concurrent_debug(m_log, this, " - m_owner = ", m_owner);
      //      m_thread = std::thread([this]() -> void { (*this)(); });
    }
    loop(const loop &) = delete;
    loop(loop &&p_loop) noexcept : m_owner(p_loop.m_owner) {
      concurrent_debug(m_log, this, " - move constructor m_owner = ", m_owner);
    }

    loop &operator=(const loop &) = delete;
    loop &operator=(loop &&p_loop) = delete;

    inline std::thread::id id() const { return m_thread.get_id(); }

    inline ~loop() {
      concurrent_debug(m_log, this,
                       " - destructor before, m_owner = ", m_owner);
      if (m_owner == nullptr) {
        concurrent_warn(m_log, this, " - m_owner is null, leaving destructor");
        return;
      }

      if (m_thread.joinable()) {
        concurrent_debug(m_log, this, " - joining");
        m_owner->m_cond_exec.notify_one();
        m_thread.join();
      }
      m_owner = nullptr;
      concurrent_debug(m_log, this, " - destructor after, m_owner = ", m_owner);
    }

    inline void abandon() {
      m_thread.detach();
      concurrent_debug(m_log, this, " - abandoning, m_owner = ", m_owner);
      m_owner = nullptr;
    }

    void start() {
      m_thread = std::thread([this]() -> void { (*this)(); });
    }

    void operator()() {
      while (true) {
        if (m_owner == nullptr) {
          concurrent_debug(m_log, this, " - m_owner = ", m_owner);
          break;
        }

        if (m_owner->m_stopped) {
          concurrent_debug(m_log, this, " - stopped ");
          break;
        }
        concurrent_debug(m_log, this,
                         " - waiting for work, m_owner = ", m_owner);

        {
          std::unique_lock<std::mutex> _lock(m_owner->m_mutex_exec);
          concurrent_debug(m_log, this,
                           " - m_cond_exec unlocked, m_owner = ", m_owner);
          if (!m_owner) {
            concurrent_error(m_log, this, " - no owner, but it should have");
            break;
          }
          concurrent_debug(m_log, this, " - waiting...");
          m_owner->m_cond_exec.wait(_lock);

          concurrent_debug(m_log, this, " - not waiting for work anymore");
        }
        if (!m_owner) {
          concurrent_warn(
              m_log, this,
              " - not waiting for work anymore, but m_owner is null");
        }

        if (m_owner->m_stopped) {
          concurrent_debug(m_log, this, " - stopped");
          break;
        }

        concurrent_debug(m_log, this, " - function to execute!");

        if (m_owner) {
          m_owner->m_function();
        } else {
          concurrent_warn(m_log, this,
                          " - could not call function as m_owener = ", m_owner);
        }
        concurrent_debug(m_log, this,
                         " - function returned, m_owner = ", m_owner);

        if (m_owner == nullptr) {
          concurrent_debug(m_log, this, " - abandoning");
          break;
        }

        if (m_owner->m_stopped) {
          concurrent_debug(m_log, this, " - stopped");
          break;
        }

        concurrent_debug(m_log, this, " - notifying");
        if (m_owner) {
          m_owner->m_cond_wait.notify_one();
        } else {
          concurrent_warn(m_log, this,
                          " - could not notify because m_owner = ", m_owner);
        }
      }

      concurrent_debug(m_log, this, " - leaving the loop");
    }

  private:
    executer_base_t<t_log> *m_owner{nullptr};

    std::thread m_thread;

    t_log m_log{"loop"};
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

  /// \brief Controls the execution of the thread
  std::mutex m_mutex_exec;

  std::thread m_timeout_thread;

  std::list<loop> m_loops;

  typename std::list<loop>::iterator m_loop;

  /// \brief Single thread used to execute the \p function over and over
  /// asynchronously
  std::thread m_thread;
};

// ########## 5 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t;

// ########## 6 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, t_result, void>;

// ########## 7 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, void, t_params...>;

// ########## 8 ##########
template <typename t_log> struct executer_t<t_log, void, void>;

// ########## 5 ##########
template <typename t_log, typename t_result, typename... t_params>
struct executer_t : public executer_base_t<t_log> {

  typedef std::function<t_result(t_params...)> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
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

    return this->template call<std::optional<t_result>>(ok, not_ok,
                                                        save_params);
  }

private:
  std::tuple<t_params...> m_params;

  t_result m_result;
};

// ########## 6 ##########
template <typename t_log, typename t_result>
struct executer_t<t_log, t_result, void> : public executer_base_t<t_log> {

  typedef std::function<t_result()> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_function]() -> void { m_result = p_function(); },
            p_timeout, p_timeout_callback) {
    this->start();
  }

  std::optional<t_result> operator()() {

    auto ok = [this]() -> std::optional<t_result> { return {m_result}; };
    auto not_ok = []() -> std::optional<t_result> { return {}; };
    auto save_params = []() -> void {};

    this->template call<std::optional<t_result>>(ok, not_ok, save_params);
  }

private:
  t_result m_result;
};

// ########## 7 ##########
template <typename t_log, typename... t_params>
struct executer_t<t_log, void, t_params...> : public executer_base_t<t_log> {

  typedef std::function<void(t_params...)> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>(
            [this, p_function]() -> void {
              std::apply(p_function, std::move(m_params));
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

// ########## 8 ##########
template <typename t_log>
struct executer_t<t_log, void, void> : public executer_base_t<t_log> {

  typedef std::function<void()> function;

  template <typename t_time>
  executer_t(function p_function, t_time p_timeout,
             timeout_callback p_timeout_callback)
      : executer_base_t<t_log>([this, p_function]() -> void { p_function(); },
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

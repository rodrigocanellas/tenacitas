#ifndef TENACITAS_CONCURRENT_ASYNC_LOOP_H
#define TENACITAS_CONCURRENT_ASYNC_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>

#include <concurrent/breaker.h>
#include <concurrent/internal/constants.h>
#include <concurrent/internal/log.h>
#include <concurrent/runner.h>
#include <concurrent/thread.h>
#include <concurrent/timeout_callback.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief
template <typename t_log> struct async_loop_base_t {

  typedef runner_t<t_log, bool> breaker_executer;

  /// \brief copy constructor not allowed
  async_loop_base_t(const async_loop_base_t &) = delete;

  /// \brief async_loop move constructor
  async_loop_base_t(async_loop_base_t &&p_async) = delete;

  /// \brief copy assignment not allowed
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;

  /// \brief move assignment
  async_loop_base_t &operator=(async_loop_base_t &&p_async) = delete;

  inline virtual ~async_loop_base_t() {
    concurrent_debug(m_log, "destructor");
    stop();
    concurrent_info(m_log, "leaving destructor");
  }

  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief retrieves the timeout for the Work function
  ///
  /// \return the timeout
  inline std::chrono::nanoseconds get_timeout() const { return m_timeout; }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
    m_timeout = std::chrono::duration_cast<std::chrono::nanoseconds>(p_timeout);
  }

  /// \brief run starts the loop asynchronously
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  void start() {
    if (!m_stopped) {
      concurrent_info(m_log, this,
                      "not starting the loop because it is already running");
      return;
    }
    concurrent_debug(m_log, "starting the loop");
    std::lock_guard<std::mutex> _lock(m_mutex);

    m_thread = concurrent::thread([this]() -> void { loop(); });
  }

  /// \brief stops the loop
  void stop() {
    if (m_stopped) {
      concurrent_warn(m_log, "not stopping because it was not running");
      return;
    }

    concurrent_debug(m_log, "marking to stop");
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_stopped = true;
    m_thread.join();
  }

protected:
  async_loop_base_t(std::chrono::nanoseconds p_timeout)
      : m_timeout(p_timeout) {}

  async_loop_base_t() = default;

  virtual void loop() = 0;

protected:
  std::chrono::nanoseconds m_timeout{infinite_timeout};

  /// \brief m_thread is the thread where the \p loop will run
  concurrent::thread m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;

  bool m_stopped{true};

  t_log m_log{"concurrent::async_loop"};

  static constexpr std::chrono::milliseconds m_provider_timeout{300};
  //  static constexpr std::chrono::milliseconds m_breaker_timeout{300};
};

template <typename t_log, bool use_breaker, typename... t_params>
struct async_loop_t;

// ############### 1 ########################################################
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, true, t_params...>;

// ############### 2 ########################################################
template <typename t_log, typename t_param>
struct async_loop_t<t_log, true, t_param>;

// ############### 3 ########################################################
template <typename t_log> struct async_loop_t<t_log, true>;

// ############### 4 ########################################################
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, false, t_params...>;

// ############### 5 ########################################################
template <typename t_log, typename t_param>
struct async_loop_t<t_log, false, t_param>;

// ############### 6 ########################################################
template <typename t_log> struct async_loop_t<t_log, false>;

// ############### 1 ########################################################
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, true, t_params...>
    : public async_loop_base_t<t_log> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_params...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief constructor
  /// This constructor must be used when \p t_params... is not \p void, and
  /// therefore, a Provider function must be defined
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  ///
  /// \param p_provider instance of the function that will provide \p
  /// t_params..., if available. If \p t_params... is \p void, this parameter
  /// assumes a default value of a \p void returning function
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      provider p_provider, timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker(p_breaker) {}

  inline async_loop_t(worker p_worker, breaker p_breaker, provider p_provider)
      : async_loop_base_t<t_log>(), m_work_executer(p_worker),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker(p_breaker) {}

  inline worker get_worker() const { return m_work_executer.get_worker(); }

private:
  typedef runner_t<t_log, std::optional<std::tuple<t_params...>>>
      provider_executer;

  typedef runner_t<t_log, void, t_params...> work_executer;

protected:
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_work_executer.start();
    m_provider_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          m_provider_executer();

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(this->m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided(
            _maybe_executed.value());

        if (_maybe_provided) {
          std::tuple<t_params...> _provided(std::move(*_maybe_provided));
          concurrent_debug(this->m_log, "provider provided: ", _provided);

          std::apply(m_work_executer, std::move(_provided));
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(this->m_log, "about to call breaker");
      if (m_breaker()) {
        concurrent_debug(this->m_log, "stopped by breaker");
        this->m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
  }

private:
  work_executer m_work_executer;

  provider_executer m_provider_executer;

  breaker m_breaker;
};

// ############### 2 ########################################################
template <typename t_log, typename t_param>
struct async_loop_t<t_log, true, t_param> : public async_loop_base_t<t_log> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_param)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<t_param>()> provider;

  /// \brief constructor
  /// This constructor must be used when \p t_params... is not \p void, and
  /// therefore, a Provider function must be defined
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  ///
  /// \param p_provider instance of the function that will provide \p
  /// t_params..., if available. If \p t_params... is \p void, this parameter
  /// assumes a default value of a \p void returning function
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      provider p_provider, timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker(p_breaker) {}

  inline async_loop_t(worker p_worker, breaker p_breaker, provider p_provider)
      : async_loop_base_t<t_log>(), m_work_executer(p_worker),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker(p_breaker) {}

  inline worker get_worker() const { return m_work_executer.get_worker(); }

private:
  typedef runner_t<t_log, std::optional<t_param>> provider_executer;

  typedef runner_t<t_log, void, t_param> work_executer;

private:
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_work_executer.start();
    m_provider_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      std::optional<std::optional<t_param>> _maybe_executed =
          m_provider_executer();

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(this->m_log, "provider executed");
        std::optional<t_param> _maybe_provided = _maybe_executed.value();

        if (_maybe_provided) {
          t_param _provided = std::move(*_maybe_provided);
          concurrent_debug(this->m_log, "provider provided: ", _provided);

          m_work_executer(_provided);
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(this->m_log, "about to call breaker");
      if (m_breaker()) {

        concurrent_debug(this->m_log, "stopped by breaker");
        this->m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
  }

private:
  work_executer m_work_executer;

  provider_executer m_provider_executer;

  breaker m_breaker;
};

// ############### 3 ########################################################
/// \brief
///
/// \example 25 async_loop_example
template <typename t_log>
struct async_loop_t<t_log, true> : public async_loop_base_t<t_log> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void()> worker;

  /// \brief constructor
  /// This constructor must be used when \p t_params... is not \p void, and
  /// therefore, a Provider function must be defined
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  ///
  /// \param p_provider instance of the function that will provide \p
  /// t_params..., if available. If \p t_params... is \p void, this parameter
  /// assumes a default value of a \p void returning function
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_breaker(p_breaker) {}

  inline async_loop_t(worker p_worker, breaker p_breaker)
      : async_loop_base_t<t_log>(), m_work_executer(p_worker),
        m_breaker(p_breaker) {}

  inline worker get_worker() const { return m_work_executer.get_worker(); }

private:
  typedef runner_t<t_log, void> work_executer;

private:
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_work_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      m_work_executer();

      concurrent_debug(this->m_log, "about to call breaker");
      if (m_breaker()) {
        concurrent_debug(this->m_log, "stopped by breaker");
        this->m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
  }

private:
  work_executer m_work_executer;

  breaker m_breaker;
};

// ############### 4 ########################################################
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, false, t_params...>
    : public async_loop_base_t<t_log> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_params...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief constructor
  /// This constructor must be used when \p t_params... is not \p void, and
  /// therefore, a Provider function must be defined
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  ///
  /// \param p_provider instance of the function that will provide \p
  /// t_params..., if available. If \p t_params... is \p void, this parameter
  /// assumes a default value of a \p void returning function
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  inline async_loop_t(worker p_worker, provider p_provider)
      : async_loop_base_t<t_log>(), m_work_executer(p_worker),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  inline worker get_worker() const { return m_work_executer.get_worker(); }

private:
  typedef runner_t<t_log, std::optional<std::tuple<t_params...>>>
      provider_executer;

  typedef runner_t<t_log, void, t_params...> work_executer;

private:
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_work_executer.start();
    m_provider_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          m_provider_executer();

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(this->m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = std::move(*_maybe_provided);

          concurrent_debug(this->m_log, "provider provided: ", _provided);

          std::apply(m_work_executer, _provided);
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
  }

private:
  work_executer m_work_executer;

  provider_executer m_provider_executer;
};

// ############### 5 ########################################################
template <typename t_log, typename t_param>
struct async_loop_t<t_log, false, t_param> : public async_loop_base_t<t_log> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_param)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<t_param>()> provider;

  /// \brief constructor
  /// This constructor must be used when \p t_params... is not \p void, and
  /// therefore, a Provider function must be defined
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  ///
  /// \param p_provider instance of the function that will provide \p
  /// t_params..., if available. If \p t_params... is \p void, this parameter
  /// assumes a default value of a \p void returning function
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  inline async_loop_t(worker p_worker, provider p_provider)
      : async_loop_base_t<t_log>(), m_work_executer(p_worker),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  inline worker get_worker() const { return m_work_executer.get_worker(); }

private:
  typedef runner_t<t_log, std::optional<t_param>> provider_executer;

  typedef runner_t<t_log, void, t_param> work_executer;

private:
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting p it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_work_executer.start();
    m_provider_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      std::optional<std::optional<t_param>> _maybe_executed(
          m_provider_executer());

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(this->m_log, "provider executed");
        std::optional<t_param> _maybe_provided(_maybe_executed.value());

        if (_maybe_provided) {
          t_param _provided(_maybe_provided.value());
          concurrent_debug(this->m_log, "provider provided: ", _provided);

          m_work_executer(_provided);
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
  }

private:
  work_executer m_work_executer;

  provider_executer m_provider_executer;
};

// ############### 6 ########################################################
template <typename t_log>
struct async_loop_t<t_log, false> : public async_loop_base_t<t_log> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void()> worker;

  /// \brief constructor
  /// This constructor must be used when \p t_params... is not \p void, and
  /// therefore, a Provider function must be defined
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  ///
  /// \param p_provider instance of the function that will provide \p
  /// t_params..., if available. If \p t_params... is \p void, this parameter
  /// assumes a default value of a \p void returning function
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, worker p_worker,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback) {}

  inline async_loop_t(worker p_worker)
      : async_loop_base_t<t_log>(), m_work_executer(p_worker) {}

  inline worker get_worker() const { return m_work_executer.get_worker(); }

private:
  typedef runner_t<t_log, void> work_executer;

private:
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_work_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      m_work_executer();
    }
    m_work_executer.stop();
  }

private:
  work_executer m_work_executer;
};

} // namespace concurrent
} // namespace tenacitas

#endif

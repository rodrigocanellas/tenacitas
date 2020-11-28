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
#include <concurrent/executer.h>
#include <concurrent/internal/constants.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/timeout.h>
#include <concurrent/timeout_callback.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Base class to define a loop that run asynchronously
///
/// \tparam t_log
template <typename t_log> struct async_loop_base_t {

  /// \brief Defines a \p executer for a breaker function, allowing the breaker
  /// function to be executed asynchronously, and without creating a new thread
  /// for each execution
  typedef executer_t<t_log, bool> breaker_executer;

  /// \brief copy constructor not allowed
  async_loop_base_t(const async_loop_base_t &) = delete;

  /// \brief async_loop move constructor
  async_loop_base_t(async_loop_base_t &&p_async) = delete;

  /// \brief copy assignment not allowed
  async_loop_base_t &operator=(const async_loop_base_t &) = delete;

  /// \brief move assignment
  async_loop_base_t &operator=(async_loop_base_t &&p_async) = delete;

  inline virtual ~async_loop_base_t() { stop(); }

  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief retrieves the timeout for the operation
  ///
  /// \return the timeout
  inline timeout get_timeout() const { return m_timeout; }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
    m_timeout = to_timeout(p_timeout);
  }

  /// \brief starts the loop asynchronously, if it was not already started
  void start() {
    if (!m_stopped) {
      concurrent_info(m_log, this,
                      "not starting the loop because it is already running");
      return;
    }
    concurrent_debug(m_log, "starting the loop");
    std::lock_guard<std::mutex> _lock(m_mutex);

    //    m_thread = concurrent::thread([this]() -> void { loop(); });
    m_thread = concurrent::thread(&async_loop_base_t::loop, this);
  }

  /// \brief stops the loop, if it was running
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
  /// \brief Constructor base for sub-classes
  ///
  /// \param p_timeout is the amount of time a operation has to execute,
  /// before the called loses interest in its result
  async_loop_base_t(timeout p_timeout) : m_timeout(p_timeout) {}

  async_loop_base_t() = default;

  /// \brief Abstract method to be implemented in sub-classes
  virtual void loop() = 0;

protected:
  /// \brief Maximum time the operation has to execute
  timeout m_timeout{infinite_timeout};

  /// \brief m_thread is the thread where the \p loop will run
  concurrent::thread m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;

  /// \brief Indicates if the loop should stop
  bool m_stopped{true};

  /// \brief Logger for the class
  t_log m_log{"concurrent::async_loop"};

  /// \brief Time that the provider function has to execute
  static constexpr timeout m_provider_timeout{300};
};

/// \brief Base template class
///
/// \tparam t_log
///
/// \tparam use_breaker identifies if the loop uses a breaker function, which
/// allows another control if the loop should stop
///
/// \tparam t_params are the parameters that the operation needs
template <typename t_log, bool use_breaker, typename... t_params>
struct async_loop_t;

// ############### 1 ########################################################
/// \brief Specialization where the loop uses a breaker function, and the
/// operation takes parameters
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

  inline worker get_worker() const { return m_work_executer.get_operation(); }

private:
  typedef executer_t<t_log, std::optional<std::tuple<t_params...>>>
      provider_executer;

  typedef executer_t<t_log, void, t_params...> work_executer;

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
/// \brief Specialization where the loop uses a breaker function, and the
/// operation takes one parameter
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

  inline worker get_worker() const { return m_work_executer.get_operation(); }

private:
  typedef executer_t<t_log, std::optional<t_param>> provider_executer;

  typedef executer_t<t_log, void, t_param> work_executer;

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
/// \brief Specialization where the loop uses a breaker function, and the
/// operation takes no parameter
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

  inline worker get_worker() const { return m_work_executer.get_operation(); }

private:
  typedef executer_t<t_log, void> work_executer;

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
/// \brief Specialization where the loop does not use a breaker function, and
/// the operation takes parameters
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

  inline worker get_worker() const { return m_work_executer.get_operation(); }

private:
  typedef executer_t<t_log, std::optional<std::tuple<t_params...>>>
      provider_executer;

  typedef executer_t<t_log, void, t_params...> work_executer;

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
/// \brief Specialization where the loop does not use a breaker function, and
/// the operation takes one parameter
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

  inline worker get_worker() const { return m_work_executer.get_operation(); }

private:
  typedef executer_t<t_log, std::optional<t_param>> provider_executer;

  typedef executer_t<t_log, void, t_param> work_executer;

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
/// \brief Specialization where the loop does not use a breaker function, and
/// the operation takes no parameter
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

  inline worker get_worker() const { return m_work_executer.get_operation(); }

private:
  typedef executer_t<t_log, void> work_executer;

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

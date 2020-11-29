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

    m_thread = concurrent::thread([this]() -> void { loop(); });
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
  static constexpr std::chrono::milliseconds m_provider_timeout{300};
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
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the operation needs
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, true, t_params...>
    : public async_loop_base_t<t_log> {

  /// \brief type of operation that will be executed in a loop
  typedef std::function<void(t_params...)> operation;

  /// \brief type of function that provides data to the operation
  ///
  /// \return \p an optional tuple of objects needed by the operation
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief constructor
  ///
  /// \tparam t_time is the type of time used to define a timeout for the
  /// operation
  ///
  /// \param p_timeout is the timeout for the operation to execute
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_breaker is the function that can be used to stop the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  ///
  /// \param p_timeout_callback is the function to be called if the operation
  /// exceeds the timeout
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, operation p_operation,
                      breaker p_breaker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_operation_executer(p_timeout, p_operation, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker(p_breaker) {}

  /// \brief constructor
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_breaker is the function that can be used to stop the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  inline async_loop_t(operation p_operation, breaker p_breaker,
                      provider p_provider)
      : async_loop_base_t<t_log>(), m_operation_executer(p_operation),
        m_provider_executer(this->m_provider_timeout, p_provider),
        m_breaker(p_breaker) {}

  /// \brief Retrieves the operation executed in the loop
  inline operation get_operation() const {
    return m_operation_executer.get_operation();
  }

private:
  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the provider function
  typedef executer_t<t_log, std::optional<std::tuple<t_params...>>>
      provider_executer;

  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the operation
  typedef executer_t<t_log, void, t_params...> operation_executer;

protected:
  /// \brief Loop executed in another thread
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_operation_executer.start();
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

          std::apply(m_operation_executer, std::move(_provided));
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
    m_operation_executer.stop();
    m_provider_executer.stop();
  }

private:
  /// \brief Asynchronous executer of the operation
  operation_executer m_operation_executer;

  /// \brief Asynchronous executer of the provider
  provider_executer m_provider_executer;

  /// \brief Loop breaker function
  breaker m_breaker;
};

// ############### 2 ########################################################
/// \brief Specialization where the loop uses a breaker function, and the
/// operation takes one parameter
///
/// \tparam t_log
///
/// \tparam t_param is the parameter that the operation needs
template <typename t_log, typename t_param>
struct async_loop_t<t_log, true, t_param> : public async_loop_base_t<t_log> {

  /// \brief type of operation that will be executed in a loop
  typedef std::function<void(t_param)> operation;

  /// \brief type of function that provides data to the operation
  ///
  /// \return \p an optional tuple of objects needed by the operation
  typedef std::function<std::optional<t_param>()> provider;

  /// \brief constructor
  ///
  /// \tparam t_time is the type of time used to define a timeout for the
  /// operation
  ///
  /// \param p_timeout is the timeout for the operation to execute
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_breaker is the function that can be used to stop the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  ///
  /// \param p_timeout_callback is the function to be called if the operation
  /// exceeds the timeout
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, operation p_operation,
                      breaker p_breaker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_operation_executer(p_timeout, p_operation, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker(p_breaker) {}

  /// \brief constructor
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_breaker is the function that can be used to stop the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  inline async_loop_t(operation p_operation, breaker p_breaker,
                      provider p_provider)
      : async_loop_base_t<t_log>(), m_operation_executer(p_operation),
        m_provider_executer(this->m_provider_timeout, p_provider),
        m_breaker(p_breaker) {}

  /// \brief Retrieves the operation executed in the loop
  inline operation get_operation() const {
    return m_operation_executer.get_operation();
  }

private:
  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the provider function
  typedef executer_t<t_log, std::optional<t_param>> provider_executer;

  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the operation
  typedef executer_t<t_log, void, t_param> operation_executer;

private:
  /// \brief Loop executed in another thread
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_operation_executer.start();
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

          m_operation_executer(_provided);
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
    m_operation_executer.stop();
    m_provider_executer.stop();
  }

private:
  /// \brief Asynchronous executer of the operation
  operation_executer m_operation_executer;

  /// \brief Asynchronous executer of the provider
  provider_executer m_provider_executer;

  /// \brief Loop breaker function
  breaker m_breaker;
};

// ############### 3 ########################################################
/// \brief Specialization where the loop uses a breaker function, and the
/// operation takes no parameter
///
/// \tparam t_log
template <typename t_log>
struct async_loop_t<t_log, true> : public async_loop_base_t<t_log> {

  /// \brief type of operation that will be executed in a loop
  typedef std::function<void()> operation;

  /// \brief constructor
  ///
  /// \tparam t_time is the type of time used to define a timeout for the
  /// operation
  ///
  /// \param p_timeout is the timeout for the operation to execute
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_breaker is the function that can be used to stop the loop
  ///
  /// \param p_timeout_callback is the function to be called if the operation
  /// exceeds the timeout
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, operation p_operation,
                      breaker p_breaker, timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_operation_executer(p_timeout, p_operation, p_timeout_callback),
        m_breaker(p_breaker) {}

  /// \brief constructor
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_breaker is the function that can be used to stop the loop
  inline async_loop_t(operation p_operation, breaker p_breaker)
      : async_loop_base_t<t_log>(), m_operation_executer(p_operation),
        m_breaker(p_breaker) {}

  /// \brief Retrieves the operation executed in the loop
  inline operation get_operation() const {
    return m_operation_executer.get_operation();
  }

private:
  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the operation
  typedef executer_t<t_log, void> operation_executer;

private:
  /// \brief Loop executed in another thread
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_operation_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      m_operation_executer();

      concurrent_debug(this->m_log, "about to call breaker");
      if (m_breaker()) {
        concurrent_debug(this->m_log, "stopped by breaker");
        this->m_stopped = true;
        break;
      }
    }
    m_operation_executer.stop();
  }

private:
  /// \brief Asynchronous executer of the operation
  operation_executer m_operation_executer;

  /// \brief Asynchronous executer of the provider
  breaker m_breaker;
};

// ############### 4 ########################################################
/// \brief Specialization where the loop does not use a breaker function, and
/// the operation takes parameters
/// \tparam t_log
///
/// \tparam t_params are the parameters that the operation needs
template <typename t_log, typename... t_params>
struct async_loop_t<t_log, false, t_params...>
    : public async_loop_base_t<t_log> {

  /// \brief type of operation that will be executed in a loop
  typedef std::function<void(t_params...)> operation;

  /// \brief type of function that provides data to the operation
  ///
  /// \return \p an optional tuple of objects needed by the operation
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief constructor
  ///
  /// \tparam t_time is the type of time used to define a timeout for the
  /// operation
  ///
  /// \param p_timeout is the timeout for the operation to execute
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  ///
  /// \param p_timeout_callback is the function to be called if the operation
  /// exceeds the timeout
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, operation p_operation,
                      provider p_provider, timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_operation_executer(p_timeout, p_operation, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  /// \brief constructor
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  ///
  inline async_loop_t(operation p_operation, provider p_provider)
      : async_loop_base_t<t_log>(), m_operation_executer(p_operation),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  /// \brief Retrieves the operation executed in the loop
  inline operation get_operation() const {
    return m_operation_executer.get_operation();
  }

private:
  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the provider function
  typedef executer_t<t_log, std::optional<std::tuple<t_params...>>>
      provider_executer;

  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the operation
  typedef executer_t<t_log, void, t_params...> operation_executer;

private:
  /// \brief Loop executed in another thread
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_operation_executer.start();
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

          std::apply(m_operation_executer, _provided);
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }
    }
    m_operation_executer.stop();
    m_provider_executer.stop();
  }

private:
  /// \brief Asynchronous executer of the operation
  operation_executer m_operation_executer;

  /// \brief Asynchronous executer of the provider
  provider_executer m_provider_executer;
};

// ############### 5 ########################################################
/// \brief Specialization where the loop does not use a breaker function, and
/// the operation takes one parameter
///
/// \tparam t_log
///
/// \tparam t_param is the parameter that the operation needs
template <typename t_log, typename t_param>
struct async_loop_t<t_log, false, t_param> : public async_loop_base_t<t_log> {

  /// \brief type of operation that will be executed in a loop
  typedef std::function<void(t_param)> operation;

  /// \brief type of function that provides data to the operation
  ///
  /// \return \p an optional tuple of objects needed by the operation
  typedef std::function<std::optional<t_param>()> provider;

  /// \brief constructor
  ///
  /// \tparam t_time is the type of time used to define a timeout for the
  /// operation
  ///
  /// \param p_timeout is the timeout for the operation to execute
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  ///
  /// \param p_timeout_callback is the function to be called if the operation
  /// exceeds the timeout
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, operation p_operation,
                      provider p_provider, timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_operation_executer(p_timeout, p_operation, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  /// \brief constructor
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_provider provides the parameters required by the operation
  inline async_loop_t(operation p_operation, provider p_provider)
      : async_loop_base_t<t_log>(), m_operation_executer(p_operation),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

  /// \brief Retrieves the operation executed in the loop
  inline operation get_operation() const {
    return m_operation_executer.get_operation();
  }

private:
  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the provider function
  typedef executer_t<t_log, std::optional<t_param>> provider_executer;

  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the operation
  typedef executer_t<t_log, void, t_param> operation_executer;

private:
  /// \brief Loop executed in another thread
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting p it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_operation_executer.start();
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

          m_operation_executer(_provided);
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }
    }
    m_operation_executer.stop();
    m_provider_executer.stop();
  }

private:
  /// \brief Asynchronous executer of the operation
  operation_executer m_operation_executer;

  /// \brief Asynchronous executer of the provider
  provider_executer m_provider_executer;
};

// ############### 6 ########################################################
/// \brief Specialization where the loop does not use a breaker function, and
/// the operation takes no parameter
///
/// \tparam t_log
template <typename t_log>
struct async_loop_t<t_log, false> : public async_loop_base_t<t_log> {

  /// \brief type of operation that will be executed in a loop
  typedef std::function<void()> operation;

  /// \brief constructor
  ///
  /// \tparam t_time is the type of time used to define a timeout for the
  /// operation
  ///
  /// \param p_timeout is the timeout for the operation to execute
  ///
  /// \param p_operation is the operation to be executed in the loop
  ///
  /// \param p_timeout_callback is the function to be called if the operation
  /// exceeds the timeout
  template <typename t_time>
  inline async_loop_t(t_time p_timeout, operation p_operation,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log>(p_timeout),
        m_operation_executer(p_timeout, p_operation, p_timeout_callback) {}

  /// \brief constructor
  ///
  /// \param p_operation is the operation to be executed in the loop
  inline async_loop_t(operation p_operation)
      : async_loop_base_t<t_log>(), m_operation_executer(p_operation) {}

  /// \brief Retrieves the operation executed in the loop
  inline operation get_operation() const {
    return m_operation_executer.get_operation();
  }

private:
  /// \brief Asynchronous execution, without creating a new thread for each
  /// call, of the operation
  typedef executer_t<t_log, void> operation_executer;

private:
  /// \brief Loop executed in another thread
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_operation_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      m_operation_executer();
    }
    m_operation_executer.stop();
  }

private:
  /// \brief Asynchronous executer of the operation
  operation_executer m_operation_executer;
};

} // namespace concurrent
} // namespace tenacitas

#endif

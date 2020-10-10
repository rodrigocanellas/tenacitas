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

#include <concurrent/internal/log.h>
#include <concurrent/internal/loop_wrapper.h>
#include <concurrent/runner.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_time, bool use_breaker,
          typename... t_params>
struct async_loop_t;

template <typename t_log, typename t_time, typename t_param>
struct async_loop_t<t_log, t_time, true, t_param>;

template <typename t_log, typename t_time>
struct async_loop_t<t_log, t_time, true>;

template <typename t_log, typename t_time, typename... t_params>
struct async_loop_t<t_log, t_time, false, t_params...>;

template <typename t_log, typename t_time, typename t_param>
struct async_loop_t<t_log, t_time, false, t_param>;

template <typename t_log, typename t_time>
struct async_loop_t<t_log, t_time, false>;

/// \brief
typedef std::function<bool()> breaker;

/// \brief used to notify about timeout of \p worker
typedef std::function<void(std::thread::id)> timeout_callback;

/// \brief
template <typename t_log, typename t_time> struct async_loop_base_t {

  typedef runner_t<t_log, std::chrono::milliseconds, bool> breaker_executer;

  async_loop_base_t() = delete;

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

public:
  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief retrieves the timeout for the Work function
  ///
  /// \return the timeout
  inline t_time get_timeout() const { return m_timeout; }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

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
  async_loop_base_t(t_time p_timeout) : m_timeout(p_timeout) {}

  virtual void loop() = 0;

protected:
  t_time m_timeout;

  /// \brief m_thread is the thread where the \p loop will run
  concurrent::thread m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;

  bool m_stopped{true};

  t_log m_log{"concurrent::async_loop"};

  static constexpr std::chrono::milliseconds m_provider_timeout{300};
  static constexpr std::chrono::milliseconds m_breaker_timeout{300};
};

/// \brief async_loop implements a generic loop that is executed in a non
/// blocking way
///
/// A \p aync_loop needs a Worker function, that will execute a defined work at
/// each round of the loop; a Timeout to execute; a Breaker function, that
/// indicates when the loop should break; and a Provider function, that will
/// provide data for the Work function, if the Worker function expects
/// parameters.
///
/// \bWorker
/// The Worker function can take any number and type of parameter, but the
/// return type must \p std::conditional<bool>.
///
/// If the \p std::conditional<bool> does not return a value, the loop is
/// stopped.
///
/// If the \p std::conditional<bool> returns a value and it is \p true, it means
/// means that the Worker function will continue to work in the next loop. If it
/// is \p false, it means that it will no work anymore, and the loop must stop.
///
/// The Worker has this signature:
/// <code>std::function<std::optional<t_result>(t_params &&...)></code>.
///
/// If \p t_params... is \p void, the Worker has this signature
/// <code>std::function<std::optional<t_result>(void)></code>.
///
/// \bTimeout
/// If the Worker does not finish on time, \p std::conditional<bool> will not
/// return a value, causing the loop to stop.
///
/// \bProvider
/// The Provider function provides the \p t_params... parameters for the Worker
/// function, in the case \p t_params... is not \p void
///
/// The Provider function has this signature:
/// <code>std::function<std::optional<std::tuple<t_params...>>()></code>.
///
/// If \p std::conditional does not return a value, the Worker function will not
/// return a \p bool value, making the loop to stop
///
/// If \p t_params... is \p void, the Provider is not used, and the constructor
/// that does not take a Provider parameter must be used.
///
/// \pBreaker
/// The Breaker function allows the loop to be stopped, caused by a other code
/// than the Worker.
///
/// The Breaker function has this signature:
/// <code>std::function<bool()></code>
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control of the Worker
/// function
///
/// \tparam t_params..., if not \p void, are the types of the the parameters
/// expected by the Worker function the loop; it must be:
///    - default constructible
///    - move constructible
///

// ############### 1 ########################################################
template <typename t_log, typename t_time, typename... t_params>
struct async_loop_t<t_log, t_time, true, t_params...>
    : public async_loop_base_t<t_log, t_time> {

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
  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      provider p_provider, timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log, t_time>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker_executer(std::chrono::milliseconds(this->m_breaker_timeout),
                           p_breaker) {}

private:
  typedef runner_t<t_log, std::chrono::milliseconds,
                   std::optional<std::tuple<t_params...>>>
      provider_executer;

  typedef runner_t<t_log, t_time, void, t_params...> work_executer;

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
    m_breaker_executer.start();

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
          std::tuple<t_params...> _provided(_maybe_provided.value());
          concurrent_debug(this->m_log, "provider provided: ", _provided);

          std::apply(m_work_executer, std::move(_provided));
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(this->m_log, "about to call breaker");
      std::optional<bool> _maybe_break = m_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(this->m_log, "stopped by breaker");
        this->m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
    m_breaker_executer.stop();
  }

private:
  work_executer m_work_executer;

  provider_executer m_provider_executer;

  typename async_loop_base_t<t_log, t_time>::breaker_executer
      m_breaker_executer;
};

// ############### 2 ########################################################
template <typename t_log, typename t_time, typename t_param>
struct async_loop_t<t_log, t_time, true, t_param>
    : public async_loop_base_t<t_log, t_time> {

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
  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      provider p_provider, timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log, t_time>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider),
        m_breaker_executer(std::chrono::milliseconds(this->m_breaker_timeout),
                           p_breaker) {}

private:
  typedef runner_t<t_log, std::chrono::milliseconds, std::optional<t_param>>
      provider_executer;

  typedef runner_t<t_log, t_time, void, t_param> work_executer;

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
    m_breaker_executer.start();

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
          t_param _provided = _maybe_provided.value();
          concurrent_debug(this->m_log, "provider provided: ", _provided);

          m_work_executer(_provided);
          if (this->m_stopped) {
            concurrent_debug(this->m_log, "stopped!!");
            break;
          }
        }
      }

      concurrent_debug(this->m_log, "about to call breaker");
      std::optional<bool> _maybe_break = m_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(this->m_log, "stopped by breaker");
        this->m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
    m_breaker_executer.stop();
  }

private:
  work_executer m_work_executer;

  provider_executer m_provider_executer;

  typename async_loop_base_t<t_log, t_time>::breaker_executer
      m_breaker_executer;
};

// ############### 3 ########################################################

/// \brief
///
/// \example 25 async_loop_example
template <typename t_log, typename t_time>
struct async_loop_t<t_log, t_time, true>
    : public async_loop_base_t<t_log, t_time> {

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
  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log, t_time>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_breaker_executer(std::chrono::milliseconds(this->m_breaker_timeout),
                           p_breaker) {}

private:
  typedef runner_t<t_log, t_time, void> work_executer;

private:
  void loop() {
    if (!this->m_stopped) {
      concurrent_debug(this->m_log, "not starting because it is running");
      return;
    }

    concurrent_info(this->m_log, "starting");

    this->m_stopped = false;
    m_work_executer.start();
    m_breaker_executer.start();

    while (true) {

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      m_work_executer();

      concurrent_debug(this->m_log, "about to call breaker");
      std::optional<bool> _maybe_break = m_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(this->m_log, "stopped by breaker");
        this->m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
    m_breaker_executer.stop();
  }

private:
  work_executer m_work_executer;

  typename async_loop_base_t<t_log, t_time>::breaker_executer
      m_breaker_executer;
};

// ############### 4 ########################################################
template <typename t_log, typename t_time, typename... t_params>
struct async_loop_t<t_log, t_time, false, t_params...>
    : public async_loop_base_t<t_log, t_time> {

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
  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log, t_time>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

private:
  typedef runner_t<t_log, std::chrono::milliseconds,
                   std::optional<std::tuple<t_params...>>>
      provider_executer;

  typedef runner_t<t_log, t_time, void, t_params...> work_executer;

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
          auto _provided = _maybe_provided.value();
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
template <typename t_log, typename t_time, typename t_param>
struct async_loop_t<t_log, t_time, false, t_param>
    : public async_loop_base_t<t_log, t_time> {

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
  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log, t_time>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider_executer(std::chrono::milliseconds(this->m_provider_timeout),
                            p_provider) {}

private:
  typedef runner_t<t_log, std::chrono::milliseconds, std::optional<t_param>>
      provider_executer;

  typedef runner_t<t_log, t_time, void, t_param> work_executer;

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

      std::optional<std::optional<t_param>> _maybe_executed =
          m_provider_executer();

      if (this->m_stopped) {
        concurrent_debug(this->m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(this->m_log, "provider executed");
        std::optional<std::tuple<t_param>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(this->m_log, "provider provided: ", _provided);

          m_work_executer();
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
template <typename t_log, typename t_time>
struct async_loop_t<t_log, t_time, false>
    : public async_loop_base_t<t_log, t_time> {

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
  inline async_loop_t(t_time p_timeout, worker p_worker,
                      timeout_callback p_timeout_callback)
      : async_loop_base_t<t_log, t_time>(p_timeout),
        m_work_executer(p_timeout, p_worker, p_timeout_callback) {}

private:
  typedef runner_t<t_log, t_time, void> work_executer;

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

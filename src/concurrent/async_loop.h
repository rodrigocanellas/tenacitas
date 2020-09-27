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
#include <concurrent/runner.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

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
template <typename t_log, typename t_time, typename... t_params>
struct async_loop_t {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_params...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  typedef std::function<bool()> breaker;

  /// \brief used to notify about timeout of \p worker
  typedef std::function<void(std::thread::id)> timeout_callback;

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
      : m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider(p_provider),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            m_provider),
        m_breaker(p_breaker),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           m_breaker) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      provider p_provider)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}),
        m_provider(p_provider),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            p_provider),
        m_breaker(p_breaker),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           m_breaker) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_provider(p_provider),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            p_provider) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}),
        m_provider(p_provider),
        m_provider_executer(std::chrono::milliseconds(m_provider_timeout),
                            p_provider) {}

  /// \brief default constructor not allowed
  async_loop_t() = delete;

  /// \brief copy constructor not allowed
  async_loop_t(const async_loop_t &) = delete;

  /// \brief async_loop move constructor
  async_loop_t(async_loop_t &&p_async) = delete;

  /// \brief copy assignment not allowed
  async_loop_t &operator=(const async_loop_t &) = delete;

  /// \brief move assignment
  async_loop_t &operator=(async_loop_t &&p_async) = delete;

  /// \brief destructor stops the loop
  inline ~async_loop_t() {
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
  inline t_time get_timeout() const { return m_work_executer.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_timeout(t_time p_timeout) {
    m_work_executer.set_timeout(p_timeout);
  }

  /// \brief returns the Worker function
  inline worker get_worker() const { return m_work_executer.get_worker(); }

  /// \brief returns the Provide function
  inline provider get_provider() const {
    return m_provider_executer.get_worker();
  }

  inline timeout_callback get_timeout_callback() const {
    return m_work_executer.get_timeout_callback();
  }

  inline breaker get_breaker() const { return m_breaker_executer.get_worker(); }

  /// \brief Stops the loop, and starts it again
  ///
  /// \return \p true if could restart, or any other \p false otherwise
  inline void restart() {
    concurrent_debug(m_log, "restart");
    stop();
    start();
  }

  /// \brief run starts the loop asynchronously
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  void start() {

    if (!is_stopped()) {
      concurrent_info(m_log, this,
                      "not starting the loop because it is already running");
      return;
    }
    concurrent_debug(m_log, "starting the loop");

    if (m_breaker) {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_thread = thread([this]() -> void { loop_with_breaker(); });
    } else if (!m_breaker) {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_thread = thread([this]() -> void { loop_no_breaker(); });
    }
  }

  /// \brief stops the loop
  void stop() {
    if (is_stopped()) {
      concurrent_warn(m_log,
                      "not stopping the loop because it was not running");
      return;
    }

    concurrent_debug(m_log, "marking to stop");
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_stopped = true;
  }

private:
  typedef runner_t<t_log, t_time, void, t_params...> work_executer;

  typedef runner_t<t_log, std::chrono::milliseconds,
                   std::optional<std::tuple<t_params...>>>
      provider_executer;

  typedef runner_t<t_log, std::chrono::milliseconds, bool> breaker_executer;

private:
  /// \brief loop to be executed asyncronously
  void loop_with_breaker() {
    if (!is_stopped()) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    m_work_executer.start();
    m_provider_executer.start();
    m_breaker_executer.start();

    while (true) {

      if (is_stopped()) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          m_provider_executer();

      if (is_stopped()) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          std::apply(m_work_executer, _provided);
          if (is_stopped()) {
            concurrent_debug(m_log, "stopped");
            break;
          }
        }
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = m_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
    m_breaker_executer.stop();
  }

  void loop_no_breaker() {
    if (!is_stopped()) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    m_work_executer.start();
    m_provider_executer.start();

    while (true) {

      if (is_stopped()) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<std::optional<std::tuple<t_params...>>> _maybe_executed =
          m_provider_executer();

      if (is_stopped()) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (_maybe_executed) {
        concurrent_debug(m_log, "provider executed");
        std::optional<std::tuple<t_params...>> _maybe_provided =
            _maybe_executed.value();

        if (_maybe_provided) {
          auto _provided = _maybe_provided.value();
          concurrent_debug(m_log, "provider provided: ", _provided);

          std::apply(m_work_executer, _provided);
          if (is_stopped()) {
            concurrent_debug(m_log, "stopped");
            break;
          }
        }
      }
    }
    m_work_executer.stop();
    m_provider_executer.stop();
  }

private:
  t_log m_log{"concurrent::async_loop"};

  work_executer m_work_executer;

  provider m_provider;

  provider_executer m_provider_executer;

  breaker m_breaker;

  breaker_executer m_breaker_executer;

  bool m_stopped{true};

  /// \brief m_thread is the thread where the \p loop will run
  concurrent::thread m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;

  static constexpr std::chrono::milliseconds m_provider_timeout{300};
  static constexpr std::chrono::milliseconds m_breaker_timeout{100};
};

template <typename t_log, typename t_time> struct async_loop_t<t_log, t_time> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void()> worker;

  typedef std::function<bool()> breaker;

  /// \brief used to notify about timeout of \p worker
  typedef std::function<void(std::thread::id)> timeout_callback;

  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      timeout_callback p_timeout_callback)
      : m_work_executer(p_timeout, p_worker, p_timeout_callback),
        m_breaker(p_breaker),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           m_breaker) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}),
        m_breaker(p_breaker),
        m_breaker_executer(std::chrono::milliseconds(m_breaker_timeout),
                           m_breaker) {}

  inline async_loop_t(t_time p_timeout, worker p_worker,
                      timeout_callback p_timeout_callback)
      : m_work_executer(p_timeout, p_worker, p_timeout_callback) {}

  inline async_loop_t(t_time p_timeout, worker p_worker)
      : m_work_executer(p_timeout, p_worker, [](std::thread::id) -> void {}) {}

  /// \brief default constructor not allowed
  async_loop_t() = delete;

  /// \brief copy constructor not allowed
  async_loop_t(const async_loop_t &) = delete;

  /// \brief async_loop move constructor
  async_loop_t(async_loop_t &&p_async) = delete;

  /// \brief copy assignment not allowed
  async_loop_t &operator=(const async_loop_t &) = delete;

  /// \brief move assignment
  async_loop_t &operator=(async_loop_t &&p_async) = delete;

  /// \brief destructor stops the loop
  inline ~async_loop_t() {
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
  inline t_time get_timeout() const { return m_work_executer.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_timeout(t_time p_timeout) {
    m_work_executer.set_timeout(p_timeout);
  }

  /// \brief returns the Worker function
  inline worker get_worker() const { return m_work_executer.get_worker(); }

  inline timeout_callback get_timeout_callback() const {
    return m_work_executer.get_timeout_callback();
  }

  inline breaker get_breaker() const { return m_breaker_executer.get_worker(); }

  /// \brief Stops the loop, and starts it again
  ///
  /// \return \p true if could restart, or any other \p false otherwise
  inline void restart() {
    concurrent_debug(m_log, "restart");
    stop();
    start();
  }

  /// \brief run starts the loop asynchronously
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  void start() {

    if (!is_stopped()) {
      concurrent_info(m_log, this,
                      "not starting the loop because it is already running");
      return;
    }
    concurrent_debug(m_log, "starting the loop");

    if (m_breaker) {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_thread = thread([this]() -> void { loop_with_breaker(); });
    } else if (!m_breaker) {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_thread = thread([this]() -> void { loop_no_breaker(); });
    }
  }

  /// \brief stops the loop
  void stop() {
    if (is_stopped()) {
      concurrent_warn(m_log,
                      "not stopping the loop because it was not running");
      return;
    }

    concurrent_debug(m_log, "marking to stop");
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_stopped = true;
  }

private:
  typedef runner_t<t_log, t_time, void> work_executer;

  typedef runner_t<t_log, std::chrono::milliseconds, bool> breaker_executer;

private:
  void loop_with_breaker() {
    if (!is_stopped()) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    m_work_executer.start();
    m_breaker_executer.start();

    while (true) {

      if (is_stopped()) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      m_work_executer();

      if (is_stopped()) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      concurrent_debug(m_log, "about to call breaker");
      std::optional<bool> _maybe_break = m_breaker_executer();
      if ((_maybe_break) && (_maybe_break.value())) {
        concurrent_debug(m_log, "stopped by breaker");
        m_stopped = true;
        break;
      }
    }
    m_work_executer.stop();
    m_breaker_executer.stop();
  }

  /// \brief loop to be executed asyncronously
  void loop_no_breaker() {
    if (!is_stopped()) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    m_work_executer.start();

    while (true) {

      if (is_stopped()) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      m_work_executer();
    }
    m_work_executer.stop();
  }

private:
  t_log m_log{"concurrent::async_loop"};

  work_executer m_work_executer;

  breaker m_breaker;

  breaker_executer m_breaker_executer;

  bool m_stopped{true};

  /// \brief m_thread is the thread where the \p loop will run
  concurrent::thread m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;

  static constexpr std::chrono::milliseconds m_provider_timeout{300};
  static constexpr std::chrono::milliseconds m_breaker_timeout{100};
};

} // namespace concurrent
} // namespace tenacitas

#endif

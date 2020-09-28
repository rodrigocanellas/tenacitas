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

// ################ 1 #######################################################

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

  //  typedef typename std::conditional<
  //      std::tuple_size<std::tuple<t_params...>>::value != 0,
  //      std::function<std::optional<std::tuple<t_params...>>()>,
  //      dummy_provider>::type provider;

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
      : m_loop_wrapper(p_timeout, p_worker, p_breaker, p_provider,
                       p_timeout_callback) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      provider p_provider)
      : m_loop_wrapper(p_timeout, p_worker, p_breaker, p_provider) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider,
                      timeout_callback p_timeout_callback)
      : m_loop_wrapper(p_timeout, p_worker, p_provider, p_timeout_callback) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, provider p_provider)
      : m_loop_wrapper(p_timeout, p_worker, p_provider) {}

  // ######

  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
                      timeout_callback p_timeout_callback)
      : m_loop_wrapper(p_timeout, p_worker, p_breaker, p_timeout_callback) {}

  inline async_loop_t(t_time p_timeout, worker p_worker,
                      timeout_callback p_timeout_callback)
      : m_loop_wrapper(p_timeout, p_worker, p_timeout_callback) {}

  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker)
      : m_loop_wrapper(p_timeout, p_worker, p_breaker) {}

  inline async_loop_t(t_time p_timeout, worker p_worker)
      : m_loop_wrapper(p_timeout, p_worker) {}

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
  inline bool is_stopped() const { return m_loop_wrapper.is_stopped(); }

  /// \brief retrieves the timeout for the Work function
  ///
  /// \return the timeout
  inline t_time get_timeout() const { return m_loop_wrapper.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_timeout(t_time p_timeout) {
    m_loop_wrapper.set_timeout(p_timeout);
  }

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
  inline void start() { m_loop_wrapper.start(); }

  /// \brief stops the loop
  inline void stop() { m_loop_wrapper.stop(); }

private:
  typedef loop_wrapper_t<t_log, t_time, t_params...> loop_wrapper;

private:
  loop_wrapper m_loop_wrapper;

  t_log m_log{"concurrent::async_loop"};
};

// ################ 2 #######################################################

// template <typename t_log, typename t_time> struct async_loop_t<t_log, t_time>
// {

//  /// \brief worker is the type of work function, i.e., the function that will
//  /// be called in a loop in order to execute some work
//  typedef std::function<void()> worker;

//  typedef std::function<bool()> breaker;

//  /// \brief used to notify about timeout of \p worker
//  typedef std::function<void(std::thread::id)> timeout_callback;

//  /// \brief constructor
//  /// This constructor must be used when \p t_params... is not \p void, and
//  /// therefore, a Provider function must be defined
//  ///
//  /// \param p_worker instance of the function that will execute the defined
//  /// work
//  ///
//  /// \param p_breaker instance of the function that will indicate when the
//  loop
//  /// must stop
//  ///
//  /// \param p_timeout defines the amount of time the work function has to
//  /// execute
//  ///
//  /// \param p_provider instance of the function that will provide \p
//  /// t_params..., if available. If \p t_params... is \p void, this parameter
//  /// assumes a default value of a \p void returning function
//  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker,
//                      timeout_callback p_timeout_callback)
//      : m_loop_wrapper(p_timeout, p_worker, p_breaker, p_timeout_callback) {}

//  inline async_loop_t(t_time p_timeout, worker p_worker,
//                      timeout_callback p_timeout_callback)
//      : m_loop_wrapper(p_timeout, p_worker, p_timeout_callback) {}

//  inline async_loop_t(t_time p_timeout, worker p_worker, breaker p_breaker)
//      : m_loop_wrapper(p_timeout, p_worker, p_breaker) {}

//  inline async_loop_t(t_time p_timeout, worker p_worker)
//      : m_loop_wrapper(p_timeout, p_worker) {}

//  /// \brief default constructor not allowed
//  async_loop_t() = delete;

//  /// \brief copy constructor not allowed
//  async_loop_t(const async_loop_t &) = delete;

//  /// \brief async_loop move constructor
//  async_loop_t(async_loop_t &&p_async) = delete;

//  /// \brief copy assignment not allowed
//  async_loop_t &operator=(const async_loop_t &) = delete;

//  /// \brief move assignment
//  async_loop_t &operator=(async_loop_t &&p_async) = delete;

//  /// \brief destructor stops the loop
//  inline ~async_loop_t() {
//    concurrent_debug(m_log, "destructor");
//    stop();
//    concurrent_info(m_log, "leaving destructor");
//  }

//  /// \brief is_stopped
//  ///
//  /// \return \p true if the loop is not running; \p false othewise
//  inline bool is_stopped() const { return m_loop_wrapper.is_stopped(); }

//  /// \brief retrieves the timeout for the Work function
//  ///
//  /// \return the timeout
//  inline t_time get_timeout() const { return m_loop_wrapper.get_timeout(); }

//  /// \brief redefines the value of the timeout
//  ///
//  /// It does not restart the loop, it is necessary to call \p restart
//  inline void set_timeout(t_time p_timeout) {
//    m_loop_wrapper.set_timeout(p_timeout);
//  }

//  /// \brief Stops the loop, and starts it again
//  ///
//  /// \return \p true if could restart, or any other \p false otherwise
//  inline void restart() {
//    concurrent_debug(m_log, "restart");
//    stop();
//    start();
//  }

//  /// \brief run starts the loop asynchronously
//  ///
//  /// \param p_timeout amount of time that the loop will wait for \p p_work to
//  /// execute
//  ///
//  inline void start() { m_loop_wrapper.start(); }

//  /// \brief stops the loop
//  inline void stop() { m_loop_wrapper.stop(); }

// private:
//  typedef loop_wrapper_t<t_log, t_time> loop_wrapper;

// private:
//  loop_wrapper m_loop_wrapper;

//  t_log m_log{"concurrent::async_loop"};
//};
} // namespace concurrent
} // namespace tenacitas

#endif

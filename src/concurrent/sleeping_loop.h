#ifndef TENACITAS_CONCURRENT_SLEEPING_LOOP_H
#define TENACITAS_CONCURRENT_SLEEPING_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief sleeping_loop allows a function (object) to be called in a loop that
/// is executed asyncronously at a user defined period of time.
///
/// A \p sleeping_loop needs a Worker function, that will execute a defined work
/// at each round of the loop; a Timeout to execute; a Breaker function, that
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
/// \tparam t_timeout is the type of time used for timeout control of the Worker
/// function
///
/// \tparam t_interval is the type of time used to control the interval that the
/// loop will sleep
///
/// \tparam t_params..., if not \p void, are the types of the the parameters
/// expected by the Worker function the loop; it must be:
///    - default constructible
///    - move constructible
///
template <typename t_log, typename t_timeout, typename t_interval,
          typename... t_params>
struct sleeping_loop_t {

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<bool, t_params...>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<bool, t_params...>::provider provider;

  /// \brief used to notify about timeout of \p worker
  typedef std::function<void()> timeout_callback;

  typedef t_timeout timeout;

  typedef t_interval interval;

  /// \brief sleeping_loop creates a \p sleeping_loop object, when <tt>t_data
  /// != void</tt>, and a \p provider is necessary
  ///
  /// \param p_interval the amount of time that the loop will sleep between
  /// executions
  ///
  /// \param p_worker function that will be executed each time the loop wakes up
  ///
  /// \param p_provide function that will provide data to the work function,
  /// each time the loop wakes up
  sleeping_loop_t(worker p_worker, t_timeout p_timeout, t_interval p_interval,
                  provider p_provider, timeout_callback p_timeout_callback)
      : m_async(p_worker, p_timeout, p_provider, p_timeout_callback,
                [this]() -> bool { return this->breaker(); }),

        m_interval(p_interval) {}

  /// \brief sleeping_loop creates a \p sleeping_loop object, when <tt>t_data
  /// == void</tt>, and a \p provider is not necessary
  ///
  /// \param p_interval the amount of time that the loop will sleep between
  /// executions
  ///
  /// \param p_work function that will be executed each time the loop wakes up
  sleeping_loop_t(worker p_worker, t_timeout p_timeout, t_interval p_interval,
                  timeout_callback p_timeout_callback)
      : m_async(p_worker, p_timeout, p_timeout_callback,
                [this]() -> bool { return this->breaker(); }),
        m_interval(p_interval) {}

  /// \brief default constructor not allowed
  sleeping_loop_t() = delete;

  /// \brief copy constructor not allowed
  sleeping_loop_t(const sleeping_loop_t &) = delete;

  /// \brief move constructor
  sleeping_loop_t(sleeping_loop_t &&p_sleep) = delete;

  /// \brief destructor interrupts the loop
  inline ~sleeping_loop_t() {
    concurrent_debug(m_log, "destructor");
    stop();
  }

  /// \brief copy assignment not allowed
  sleeping_loop_t &operator=(const sleeping_loop_t &) = delete;

  /// \brief move assignment
  sleeping_loop_t &operator=(sleeping_loop_t &&p_sleep) = delete;

  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_async.is_stopped(); }

  /// \brief get_interval
  /// \return the interval of loop execution
  inline t_interval get_interval() const { return m_interval; }

  /// \brief redefines the value of the execution interval
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_interval(t_interval p_interval) { m_interval = p_interval; }

  /// \brief retrieves the timeout for the Work function
  ///
  /// \return the timeout
  inline t_timeout get_timeout() const { return m_async.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_timeout(t_timeout p_timeout) {
    m_async.set_timeout(p_timeout);
  }

  inline worker get_worker() const { return m_async.get_worker(); }

  inline provider get_provider() const { return m_async.get_provider(); }

  /// \brief run starts the loop
  /// \tparam t_timeout is the type of timeout, it should be one of
  /// std::chrono::* time types
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  void start() {
    if (!m_async.is_stopped()) {
      concurrent_debug(m_log,
                       "not running async loop because it was not stopped");
      return;
    }
    concurrent_debug(m_log, "running async loop");
    m_stopped = false;
    m_async.start();
  }

  /// \brief stop stops the loop
  void stop() {
    if (m_async.is_stopped()) {
      concurrent_debug(m_log,
                       "not stopping async loop because it was not running");
      return;
    }

    concurrent_debug(m_log, "stop");

    m_stopped = true;

    m_cond_var.notify_all();

    m_async.stop();
  }

  /// \brief Stops the loop, and starts it again
  void restart() {
    concurrent_debug(m_log, "restart");
    stop();
    start();
  }

private:
  /// \brief a simpler name
  typedef async_loop_t<t_log, t_timeout, t_params...> async_loop;

private:
  /// \brief breaker defines if the loop should stop
  ///
  /// \return \p true if the loop should break; \p false othewise
  bool breaker() {
    std::unique_lock<std::mutex> _lock(m_mutex);
    if (m_cond_var.wait_for(_lock, m_interval) == std::cv_status::timeout) {
      // timeout, so do not stop
      concurrent_debug(m_log, "must not stop");
      return false;
    }
    // no timeout, which means the loop was stopped
    concurrent_debug(m_log, "must stop");
    return true;
  }

private:
  /// \brief m_loop the async_loop used
  async_loop m_async;

  /// \brief m_interval
  t_interval m_interval;

  /// \brief m_mutex
  std::mutex m_mutex;

  /// \brief m_cond_var
  std::condition_variable m_cond_var;

  bool m_stopped{true};

  t_log m_log{"concurrent::sleeping_loop"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_SLEEPING_LOOP_H

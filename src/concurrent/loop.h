#ifndef TENACITAS_CONCURRENT_LOOP_H
#define TENACITAS_CONCURRENT_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <future>

#include <concurrent/executer.h>
#include <concurrent/internal/log.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief loop implements a generic loop that is executed in a blocking way
///
/// A \p loop needs a Worker function, that will execute a defined work at each
/// round of the loop; a Timeout to execute; a Breaker function, that indicates
/// when the loop should break; and a Provider function, that will provide data
/// for the Work function, if the Worker function expects parameters.
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
template <typename t_log, typename t_time, typename... t_params> struct loop_t {

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<bool, t_params...>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<bool, t_params...>::provider provider;

  /// \brief breaker type
  /// \sa traits_t<t_data>::breaker in concurrent/traits.h
  typedef typename traits_t<bool, t_params...>::breaker breaker;

  /// \brief loop constructor
  ///
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
  inline loop_t(worker p_worker, breaker p_break, t_time p_timeout,
                provider p_provider)
      : m_breaker(p_break), m_executer(p_worker, p_timeout, p_provider) {}

  /// \brief loop constructor
  ///
  /// This constructor must be used when \p t_params... is \p void, and
  /// therefore, no Provider function must be defined
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  inline loop_t(worker p_worker, breaker p_break, t_time p_timeout)
      : m_breaker(p_break), m_executer(p_worker, p_timeout) {}

  /// \brief loop default constuctor not allowed
  loop_t() = delete;

  /// \brief destructor
  ~loop_t() {
    stop();
    concurrent_debug(m_log, "leaving destructor");
  }

  /// \brief copy constructor not allowed
  loop_t(const loop_t &) = delete;

  /// \brief loop move constructor not allowed
  loop_t(loop_t &&p_loop) = delete;

  /// \brief copy assignment not allowed
  loop_t &operator=(const loop_t &) = delete;

  /// \brief move assignment not allowed
  loop_t &operator=(loop_t &&p_loop) = delete;

  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief returns the Breaker function
  inline breaker get_breaker() const { return m_breaker; }

  /// \brief returns the Worker function
  inline worker get_worker() const { return m_executer.get_worker(); }

  /// \brief returns the Provide function
  inline provider get_provider() const { return m_executer.get_provider(); }

  /// \brief retrieves the timeout for the worker
  inline t_time get_timeout() const { return m_executer.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// \param p_timeout is the new value of the worker function
  inline void set_timeout(t_time p_timeout) {
    m_executer.set_timeout(p_timeout);
  }

  /// \brief Stops the loop, and starts it again
  inline void restart() {
    stop();
    start();
  }

  /// \brief stop forces the loop to stop
  inline void stop() {
    if (!m_stopped) {
      concurrent_warn(m_log, "stopping");
      m_stopped = true;
      m_executer.stop();
    }
  }

  /// \brief start initates the loop
  ///
  /// If the loop was already initiated, it does not start a new loop
  void start() {
    if (!m_stopped) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return;
    }

    concurrent_info(m_log, "starting");

    m_stopped = false;
    m_executer.start();

    while (true) {

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      std::optional<bool> _executer_result = m_executer();
      if (!_executer_result) {
        concurrent_warn(m_log,
                        "stopping beacuse Worker did not return any results");
        stop();
        break;
      }

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (!_executer_result.value()) {
        concurrent_warn(m_log, "stopping because Worker returned 'false'");
        stop();
        break;
      }

      if (m_stopped) {
        concurrent_debug(m_log, "stopped");
        break;
      }

      if (m_breaker()) {
        concurrent_warn(m_log, "breaker ordered to stop");
        stop();
        break;
      }
    }
  }

private:
  typedef executer_t<t_log, t_time, bool, t_params...> executer;

private:
  /// \brief m_breaker instance of the function that will indicate when the loop
  /// must stop
  breaker m_breaker;

  bool m_stopped{true};

  executer m_executer;

  t_log m_log{"concurrent::loop"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // LOOP_H

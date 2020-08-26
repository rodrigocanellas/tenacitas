#ifndef TENACITAS_CONCURRENT_LOOP_H
#define TENACITAS_CONCURRENT_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <future>

#include <concurrent/internal/log.h>
#include <concurrent/processor.h>
#include <concurrent/result.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief loop implements a generic loop
///
/// A loop needs a Worker function, that will execute a defined work at each
/// round of the loop; a Timeout to execute; a Breaker function, that indicates
/// when the loop should break; and a Provider function, that will provide data
/// for the Work function, if available.
///
/// If the Worker function needs no data,\p t_data is \p void, so the Provider
/// function does not need to be defined. In this situation, one does not need
/// to inform the \p p_provide parameter to the constructor.
///
/// \tparam t_data, if not \p void, is the type of the data manipulated during
/// the loop; it must be:
///    - default constructible
///    - move constructible
///
/// \tparam t_log provides log funcionality:
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
template <typename t_data, typename t_log,
          typename t_time = std::chrono::milliseconds>
struct loop_t {

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<t_data>::provider provider;

  /// \brief breaker type
  /// \sa traits_t<t_data>::breaker in concurrent/traits.h
  typedef typename traits_t<t_data>::breaker breaker;

  /// \brief loop constructor
  ///
  /// \param p_work instance of the function that will execute the defined
  /// work
  ///
  /// \param p_break instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  ///
  /// \param p_provide instance of the function that will provide an instance
  /// of \p t_data, if available. If \p t_data is \p void, this parameter
  /// assumes a default value of a \p void returning function
  inline loop_t(
      worker p_work, breaker p_break, t_time p_timeout,
      provider p_provide = []() -> status::result { return status::ok; })
      : m_worker(p_work), m_breaker(p_break), m_timeout(p_timeout),
        m_provider(p_provide), m_stopped_by_user(true),
        m_processor(m_worker, m_provider, m_timeout) {}

  /// \brief loop decault constuctor not allowed
  loop_t() = delete;

  /// \brief destructor
  ~loop_t() { m_stopped_by_destructor = true; }

  /// \brief copy constructor not allowed
  loop_t(const loop_t &) = delete;

  /// \brief loop move constructor
  /// \param p_loop an instance o \p loop to be moved
  loop_t(loop_t &&p_loop) = delete;

  /// \brief copy assignment not allowed
  loop_t &operator=(const loop_t &) = delete;

  /// \brief move assignment
  loop_t &operator=(loop_t &&p_loop) = delete;

  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return (m_running == false); }

  /// \brief get_worker
  ///
  /// \return a copy of the function that executes a defined work in each
  /// round of the loop
  inline worker get_worker() const { return m_worker; }

  /// \brief get_breaker
  ///
  /// \return a copy of the function that can make the loop stop
  inline breaker get_breaker() const { return m_breaker; }

  /// \brief get_provider
  ///
  /// \return a copy of the function that provides an instance of \p t_data,
  /// if available, to the work function
  inline provider get_provider() const { return m_provider; }

  /// \brief retrieves the timeout for the Work function
  ///
  /// \return the timeout
  inline t_time get_timeout() const { return m_timeout; }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_timeout(t_time p_timeout) { m_timeout = p_timeout; }

  /// \brief Stops the loop, and starts it again
  ///
  /// \return \p status::ok if could restart, or any other \p status::result
  /// otherwise
  inline status::result restart() {
    stop();
    return start();
  }

  /// \brief stop forces the loop to stop
  inline void stop() {
    m_stopped_by_user = true;
    m_processor.stop();
  }

  /// \brief start initates the loop
  ///
  /// If the loop was already initiated, it does not start a new loop
  ///
  /// \return \p status::ok if the loop finished with no error;
  ///         \p concurrent::already_running if the loop was already started
  ///         or any other value, therwise
  status::result start() {
    if (m_running == true) {
      concurrent_debug(m_log, "not starting beacause it is running");
      return concurrent::already_running;
    }

    status::result _result = status::ok;
    m_stopped_by_user = false;
    m_running = true;

    while (true) {

      if (m_stopped_by_user) {

        _result = concurrent::stopped_by_user;
        concurrent_warn(m_log, _result);
        break;
      }

      if (m_stopped_by_destructor) {
        concurrent_warn(m_log,
                        "loop stopped: ", concurrent::stop_by_destructor);
        _result = status::ok;
        break;
      }

      status::result _worker_result = m_processor();

      if (_worker_result != status::ok) {
        concurrent_warn(m_log, "worker returned ", _worker_result);
        if ((_worker_result == concurrent::stopped_by_worker) ||
            (_worker_result == concurrent::stopped_by_provider)) {
          _result = _worker_result;
          break;
        }
      }

      if (m_stopped_by_user) {
        _result = concurrent::stopped_by_user;
        concurrent_warn(m_log, "loop stopped: ", _result);
        break;
      }

      if (m_stopped_by_destructor) {
        concurrent_warn(m_log,
                        "loop stopped: ", concurrent::stop_by_destructor);
        _result = status::ok;
        break;
      }

      _result = m_breaker();
      if (_result != status::ok) {
        if (_result == concurrent::stopped_by_breaker) {
          concurrent_warn(m_log, "loop stopped: ", _result);
          _result = status::ok;
        } else {
          concurrent_error(m_log, _result);
        }
        break;
      }
    }
    m_running = false;
    return _result;
  }

private:
  typedef processor_t<t_data, t_time, t_log> processor;

private:
  /// \brief m_worker instance of the function that will execute the defined
  /// work
  worker m_worker;

  /// \brief m_breaker instance of the function that will indicate when the loop
  /// must stop
  breaker m_breaker;

  /// \brief defines the amount of time the work function has to execute
  t_time m_timeout;

  /// \brief m_provider instance of the function that will provide an instance
  /// of \p t_data, if available
  provider m_provider;

  /// \brief m_stopped_by_user indicates if the loop is running or not
  bool m_stopped_by_user;

  bool m_stopped_by_destructor{false};

  bool m_running{false};

  processor m_processor;

  t_log m_log{"concurrent::loop"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // LOOP_H

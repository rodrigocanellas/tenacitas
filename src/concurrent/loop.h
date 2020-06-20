#ifndef TENACITAS_CONCURRENT_LOOP_H
#define TENACITAS_CONCURRENT_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <future>

#include <concurrent/internal/log.h>
#include <concurrent/internal/process.h>
#include <concurrent/result.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief loop implements a generic loop
///
/// A loop needs a Work function, that will execute a defined work at each round
/// of the loop; a Timeout to execute; a Break function, that indicates when
/// the loop should break; and a Provide function, that will provide data for
/// the Work function, if available.
///
/// If the Work function needs no data, so the Provide function does not need
/// to be defined. In this situation, one does not need to provide the \p
/// p_provide parameter to the constructor.
///
/// \tparam t_data, if not void, is the type of the data manipulated during the
/// loop; it must be:
///    - default constructible
///    - move constructible
///
/// \tparam t_log provides log funcionality:
/// static void debug(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void info(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void warn(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void error(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void fatal(const std::string & p_file, int p_line, const
/// t_params&... p_params)
///
/// \tparam t_time is the type of time used for timeout control
///
template <typename t_data, typename t_log,
          typename t_time = std::chrono::milliseconds>
struct loop_t {

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in tenacitas/concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in tenacitas/concurrent/traits.h
  typedef typename traits_t<t_data>::provider provider;

  /// \brief breaker type
  /// \sa traits_t<t_data>::breaker in tenacitas/concurrent/traits.h
  typedef typename traits_t<t_data>::breaker breaker;

  /// \brief log alias for @p t_log
  typedef t_log log;

  /// \brief loop constructor
  ///
  /// \param p_work instance of the function that will execute the defined
  /// work
  ///
  /// \param p_break instance of the function that will indicate when the loop
  /// must stop
  ///
  /// \param p_provide instance of the function that will provide an instance
  /// of \p t_data, if available
  ///
  /// \param p_timeout defines the amount of time the work function has to
  /// execute
  inline loop_t(worker p_work, breaker p_break, t_time p_timeout,
                provider p_provide = []() -> status::result {
                  return status::ok;
                })
      : m_work(p_work), m_break(p_break), m_timeout(p_timeout),
        m_provide(p_provide), m_stopped(true) {}

  /// \brief loop decault constuctor not allowed
  loop_t() = delete;

  /// \brief destructor
  ~loop_t() = default;

  /// \brief copy constructor not allowed
  loop_t(const loop_t &) = delete;

  ///
  /// \brief loop move constructor
  /// \param p_loop an instance o \p loop to be moved
  ///
  loop_t(loop_t &&p_loop) noexcept
      : m_work(std::move(p_loop.m_work)), m_break(std::move(p_loop.m_break)),
        m_timeout(std::move(p_loop.m_timeout)),
        m_provide(std::move(p_loop.m_provide)), m_stopped(true) {}

  /// \brief copy assignment not allowed
  loop_t &operator=(const loop_t &) = delete;

  /// \brief move assignment
  loop_t &operator=(loop_t &&p_loop) noexcept {
    if (this != &p_loop) {
      m_work = std::move(p_loop.m_work);
      m_break = std::move(p_loop.m_break);
      m_timeout = std::move(p_loop.m_timeout);
      m_provide = std::move(p_loop.m_provide);
      m_stopped = true;
    }
    return *this;
  }

  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_stopped; }

  /// \brief get_work
  ///
  /// \return a copy of the function that executes a defined work in each
  /// round of the loop
  inline worker get_worker() const { return m_work; }

  /// \brief get_break
  ///
  /// \return a copy of the function that can make the loop stop
  inline breaker get_breaker() const { return m_break; }

  /// \brief get_provide
  ///
  /// \return a copy of the function that provides an instance of \p t_data,
  /// if available, to the work function
  inline provider get_provider() const { return m_provide; }

  inline t_time get_timeout() const { return m_timeout; }

  inline void set_timeout(t_time p_timeout) {
    stop();
    m_timeout = p_timeout;
    start();
  }

  /// \brief stop forces the loop to stop
  inline void stop() { m_stopped = true; }

  /// \brief start initates the loop
  ///
  /// If the loop was already initiated, it does not start a new loop
  ///
  /// \return status::ok if the loop finished with no error, or any other value,
  /// otherwise
  status::result start() {
    if (m_stopped == false) {
      concurrent_log_debug(log, " not starting beacause it is running");
      return concurrent::already_running;
    }

    status::result _result = status::ok;
    m_stopped = false;

    while (true) {

      if (m_stopped) {
        _result = concurrent::stopped_by_user;
        concurrent_log_debug(log, _result);
        break;
      }

      concurrent_log_debug(log, "calling work");
      _result =
          process<t_data, t_time>()(m_work, m_provide, m_timeout, m_stopped);
      if (_result != status::ok) {
        concurrent_log_error(log, _result);
        break;
      }

      if (m_stopped) {
        _result = concurrent::stopped_by_user;
        concurrent_log_debug(log, _result);
        break;
      }

      _result = m_break();
      if (_result != status::ok) {
        if (_result == concurrent::stopped_by_breaker) {
          _result = status::ok;
        }
        concurrent_log_error(log, _result);
        m_stopped = true;
        break;
      }
    }
    return _result;
  }

private:
  /// \brief m_work instance of the function that will execute the defined
  /// work
  worker m_work;

  /// \brief m_break instance of the function that will indicate when the loop
  /// must stop
  breaker m_break;

  /// \brief defines the amount of time the work function has to execute
  t_time m_timeout;

  /// \brief m_provide instance of the function that will provide an instance
  /// of \p t_data, if available
  provider m_provide;

  /// \brief m_stopped indicates if the loop is running or not
  bool m_stopped;
};

} // namespace concurrent
} // namespace tenacitas

#endif // LOOP_H

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
#include <concurrent/loop.h>
#include <concurrent/result.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief allows to execute a loop asyncronously
///
/// \param t_data is the type of the data to be handled. If it is not \p void,
/// it must be:
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
struct async_loop_t {

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<t_data>::provider provider;

  /// \brief break_t is the type of function that indicates if the loop should
  /// \brief breaker type
  /// \sa traits_t<t_data>::breaker in concurrent/traits.h
  typedef typename traits_t<t_data>::breaker breaker;

  /// \brief log alias for @p t_log
  typedef t_log log;

  /// \brief async_loop constructor
  ///
  /// This constructor should be used when \p t_data is not \p void
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
  inline async_loop_t(worker p_work, breaker p_break, t_time p_timeout,
                      provider p_provide)
      : m_loop(p_work, p_break, p_timeout, p_provide), m_timeout(p_timeout),
        m_thread() {}

  /// \brief async_loop constructor
  ///
  /// This constructor should be used when \p t_data is \p void
  ///
  /// \param p_worker instance of the function that will execute the defined
  /// work
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  /// \param p_breaker instance of the function that will indicate when the loop
  /// must stop
  inline async_loop_t(worker p_worker, breaker p_breaker, t_time p_timeout)
      : m_loop(p_worker, p_breaker, p_timeout), m_timeout(p_timeout),
        m_thread() {}

  /// \brief default constructor not allowed
  async_loop_t() = delete;

  /// \brief copy constructor not allowed
  async_loop_t(const async_loop_t &) = delete;

  /// \brief async_loop move constructor
  async_loop_t(async_loop_t &&p_async) noexcept
      : m_loop(std::move(p_async.m_loop)) {}

  /// \brief copy assignment not allowed
  async_loop_t &operator=(const async_loop_t &) = delete;

  /// \brief move assignment
  async_loop_t &operator=(async_loop_t &&p_async) noexcept {
    if (this != &p_async) {
      m_loop = std::move(p_async.m_loop);
    }
    return *this;
  }

  /// \brief destructor stops the loop
  inline ~async_loop_t() { stop(); }

  /// \brief is_stopped
  ///
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_loop.is_stopped(); }

  /// \brief get_worker
  ///
  /// \return a copy of the function that executes a defined work in each
  /// round of the loop
  inline worker get_worker() const { return m_loop.get_worker(); }

  /// \brief get_breaker
  ///
  /// \return a copy of the function that can make the loop stop
  inline breaker get_breaker() const { return m_loop.get_breaker(); }

  /// \brief get_provider
  ///
  /// \return a copy of the function that provides an instance of \p t_data,
  /// if available, to the work function
  inline provider get_provider() const { return m_loop.get_provider(); }

  /// \brief retrieves the timeout for the Work function
  ///
  /// \return the timeout
  inline t_time get_timeout() const { return m_loop.get_timeout(); }

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

  /// \brief run starts the loop asynchronously
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  /// \return status::ok if the loop finished with no error; otherwise, it
  /// returns any other value
  status::result start() {

    if (!m_loop.is_stopped()) {
      concurrent_log_debug(
          log, this, "not starting the loop because it is already running");
      return concurrent::already_running;
    }
    concurrent_log_debug(log, "starting the loop");
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_thread = thread([this]() -> status::result { return m_loop.start(); });

    return status::ok;
  }

  /// \brief stops the loop
  void stop() {
    if (m_loop.is_stopped()) {
      concurrent_log_warn(log,
                          " not stopping the loop because it was not running");
      return;
    }

    std::lock_guard<std::mutex> _lock(m_mutex);
    concurrent_log_debug(log, "marking to stop");
    m_loop.stop();
    concurrent_log_debug(log, "joining");
  }

private:
  /// \brief loop_t is an easier name for the loop
  typedef loop_t<t_data, t_log, t_time> loop;

  /// \brief thread_t is an easier name for our wrapper to std::thread
  typedef thread thread_t;

private:
  /// \brief common method to start the loop
  template <typename t_timeout> void run_core(t_timeout p_timeout) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_thread = thread([this, p_timeout]() -> void { m_loop.start(p_timeout); });
  }

private:
  /// \brief m_loop is the \p loop to be executed asyncronously
  loop m_loop;

  t_time m_timeout;

  /// \brief m_thread is the thread where the \p loop will run
  thread_t m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;
};

} // namespace concurrent
} // namespace tenacitas

#endif

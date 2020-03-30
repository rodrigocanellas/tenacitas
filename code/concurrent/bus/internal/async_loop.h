#ifndef TENACITAS_CONCURRENT_BUS_ASYNC_LOOP_H
#define TENACITAS_CONCURRENT_BUS_ASYNC_LOOP_H

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

#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/internal/loop.h>
#include <concurrent/bus/thread.h>
#include <concurrent/bus/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {
/// \brief namespace of the class group
namespace bus {

///
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
template <typename t_data, typename t_log> struct async_loop_t {
  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or work_status::dont_stop if it should continue
  typedef typename traits_t<t_data>::worker worker;

  typedef typename traits_t<t_data>::worker_ptr worker_ptr;

  ///
  /// \brief provide_t is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return a pair, where if \p first is \p true, the \p second has a
  /// meaningful data; if \p first is \p false, then \p second has a default
  /// value of \p t_data
  ///
  typedef typename traits_t<t_data>::provider provider;

  ///
  /// \brief break_t is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or work_status::dont_stop if it should continue
  typedef std::function<work_status()> breaker;

  ///
  /// \brief log alias for @p t_log
  ///
  typedef t_log log;

  ///
  /// \brief async_loop constructor
  ///
  /// This constructor should be used when \p t_data is not \p void
  ///
  /// \param p_work instance of the function that will execute the defined
  /// work
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  /// \param p_break instance of the function that will indicate when the loop
  /// must stop
  /// \param p_provide instance of the function that will provide an instance
  /// of \p t_data, if available
  ///
  inline async_loop_t(worker p_work, std::chrono::milliseconds p_timeout,
                      breaker p_break, provider p_provide)
      : m_loop(p_work, p_timeout, p_break, p_provide), m_thread() {}

  ///
  /// \brief async_loop constructor
  ///
  /// This constructor should be used when \p t_data is \p void
  ///
  /// \param p_work instance of the function that will execute the defined
  /// work
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  /// \param p_break instance of the function that will indicate when the loop
  /// must stop
  ///
  inline async_loop_t(worker p_work, std::chrono::milliseconds p_timeout,
                      breaker p_break)
      : m_loop(p_work, p_timeout, p_break, []() -> void {}), m_thread() {}

  /// \brief default constructor not allowed
  async_loop_t() = delete;

  /// \brief copy constructor not allowed
  async_loop_t(const async_loop_t &) = delete;

  /// \brief async_loop move constructor not allowed
  async_loop_t(async_loop_t &&p_async) noexcept
      : m_loop(std::move(p_async.m_loop)) {}

  /// \brief copy assignment not allowed
  async_loop_t &operator=(const async_loop_t &) = delete;

  /// \brief move assignment not allowed
  async_loop_t &operator=(async_loop_t &&p_async) noexcept {
    if (this != &p_async) {
      m_loop = std::move(p_async.m_loop);
    }
    return *this;
  }

  /// \brief destructor stops the loop
  inline ~async_loop_t() {
    concurrent_log_debug(log, this, " destructor");
    stop();
  }

  ///
  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  ///
  inline bool is_stopped() const { return m_loop.is_stopped(); }

  ///
  /// \brief get_work
  /// \return a copy of the function that executes a defined work in each
  /// round of the loop
  ///
  inline worker get_work() const { return m_loop.get_worker(); }

  ///
  /// \brief get_break
  /// \return a copy of the function that can make the loop stop
  ///
  inline breaker get_break() const { return m_loop.get_breaker(); }

  ///
  /// \brief get_provide
  /// \return a copy of the function that provides an instance of \p t_data,
  /// if available, to the work function
  ///
  inline provider get_provide() const { return m_loop.get_provider(); }

  ///
  /// \brief get_timeout
  /// \return the amount of time that the loop will wait for the work function
  /// to finish
  ///
  inline std::chrono::milliseconds get_timeout() const {
    return m_loop.get_timeout();
  }

  ///
  /// \brief run starts the loop
  ///
  void run() {

    if (!m_loop.is_stopped()) {
      concurrent_log_debug(
          log, this, " not starting the loop because it is already running");
      return;
    }
    concurrent_log_debug(log, this, " starting the loop");
    run_core();
  }

  ///
  /// \brief stop stops the loop
  ///
  void stop() {
    if (m_loop.is_stopped()) {
      concurrent_log_debug(log, this,
                           " not stopping the loop because it was not running");
      return;
    }

    std::lock_guard<std::mutex> _lock(m_mutex);
    concurrent_log_debug(log, this, " marking to stop");
    m_loop.stop();
    concurrent_log_debug(log, this, " joining");
  }

private:
  /// \brief loop_t is an easier name for the loop
  typedef loop_t<t_data, t_log> loop;

  /// \brief thread_t is an easier name for our wrapper to std::thread
  typedef thread thread_t;

private:
  /// \brief common method to start the loop
  void run_core() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_thread = thread([this]() -> void { m_loop.start(); });
  }

private:
  /// \brief m_loop is the \p loop to be executed asyncronously
  loop m_loop;

  /// \brief m_thread is the thread where the \p loop will run
  thread_t m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;
};

} // namespace bus
} // namespace concurrent
} // namespace tenacitas

#endif

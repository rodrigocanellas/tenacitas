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
#include <concurrent/status.h>
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
template <typename t_data, typename t_log> struct async_loop_t {

  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return \p status::ok if the loop where this function is being called
  /// should continue, or any other value if it should stop
  typedef typename traits_t<t_data>::worker worker;

  /// \brief provide_t is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return a pair, where:
  /// if \p first is \p status::ok, the \p second has a meaningful data;
  ///
  /// if \p first is \p status::stopped_by_provider, then there is no more data
  /// to provide, and the \p second has a default value of \p t_data;
  ///
  /// if \p first is anything else, it means there was an error in the provider
  /// function, and \p second has a default value of \p t_data
  typedef typename traits_t<t_data>::provider provider;

  /// \brief break_t is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return \p true if the loop where this function is being called should
  /// stop, or \p false it should continue
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
  inline async_loop_t(worker p_work, breaker p_break, provider p_provide)
      : m_loop(p_work, p_break, p_provide), m_thread() {}

  /// \brief async_loop constructor
  ///
  /// This constructor should be used when \p t_data is \p void
  ///
  /// \param p_work instance of the function that will execute the defined
  /// work
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  /// \param p_break instance of the function that will indicate when the loop
  /// must stop
  inline async_loop_t(worker p_work, breaker p_break)
      : m_loop(p_work, p_break), m_thread() {}

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

  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_loop.is_stopped(); }

  /// \brief get_work
  /// \return a copy of the function that executes a defined work in each
  /// round of the loop
  inline worker get_work() const { return m_loop.get_worker(); }

  /// \brief get_break
  /// \return a copy of the function that can make the loop stop
  inline breaker get_break() const { return m_loop.get_breaker(); }

  /// \brief get_provide
  /// \return a copy of the function that provides an instance of \p t_data,
  /// if available, to the work function
  inline provider get_provide() const { return m_loop.get_provider(); }

  /// \brief run starts the loop
  ///
  /// \tparam t_timeout is the type of timeout, it should be one of
  /// std::chrono::* time types
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  /// \return status::ok if the loop finished with no error, or any other value,
  /// otherwise
  template <typename t_timeout> status::code run(t_timeout p_timeout) {

    if (!m_loop.is_stopped()) {
      concurrent_log_debug(
          log, this, " not starting the loop because it is already running");
      return status::already_running;
    }
    concurrent_log_debug(log, this, " starting the loop");
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_thread = thread([this, p_timeout]() -> status::code {
      return m_loop.start(p_timeout);
    });

    return status::ok;
  }

  /// \brief stop stops the loop
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
  template <typename t_timeout> void run_core(t_timeout p_timeout) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_thread = thread([this, p_timeout]() -> void { m_loop.start(p_timeout); });
  }

private:
  /// \brief m_loop is the \p loop to be executed asyncronously
  loop m_loop;

  /// \brief m_thread is the thread where the \p loop will run
  thread_t m_thread;

  /// \brief m_mutex protects the start of the \p m_loop execution \p m_thread
  std::mutex m_mutex;
};

} // namespace concurrent
} // namespace tenacitas

#endif

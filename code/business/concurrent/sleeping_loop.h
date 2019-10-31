#ifndef TENACITAS_BUSINESS_CONCURRENT_SLEEPING_LOOP_H
#define TENACITAS_BUSINESS_CONCURRENT_SLEEPING_LOOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include <business/concurrent/internal/async_loop.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace business {
/// \brief namespace of the class group
namespace concurrent {

///
/// \brief sleeping_loop allows a function (object) to be called in a loop that
/// is executed asyncronously at a user defined period of time.
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
template<typename t_data, typename t_log>
struct sleeping_loop_t
{
  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or result::dont_stop if it should continue
  typedef typename traits_t<t_data>::worker worker;

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
  /// \brief log alias for @p t_log
  ///
  typedef t_log log;

  ///
  /// \brief sleeping_loop creates a \p sleeping_loop object, when <tt>bt_data
  /// != void</tt>, and a \p provider is necessary
  ///
  /// \param p_interval the amount of time that the loop will sleep between
  /// executions
  /// \param p_work function that will be executed each time the loop wakes up
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  /// \param p_provide function that will provide data to the work function,
  /// each time the loop wakes up
  ///
  sleeping_loop_t(std::chrono::milliseconds p_interval,
                  worker&& p_work,
                  std::chrono::milliseconds p_timeout,
                  provider&& p_provide)
    : m_async(std::move(p_work),
              std::move(p_timeout),
              [this]() -> result { return this->break_loop(); },
              std::move(p_provide))
    , m_interval(p_interval)
  {}

  ///
  /// \brief sleeping_loop creates a \p sleeping_loop object, when <tt>bt_data
  /// == void</tt>, and a \p provider is not necessary
  ///
  /// \param p_interval the amount of time that the loop will sleep between
  /// executions
  /// \param p_work function that will be executed each time the loop wakes up
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  sleeping_loop_t(std::chrono::milliseconds p_interval,
                  worker&& p_work,
                  std::chrono::milliseconds p_timeout)
    : m_async(std::move(p_work),
              std::move(p_timeout),
              [this]() -> result { return this->break_loop(); },
              []() -> void {})
    , m_interval(p_interval)
  {}

  /// \brief default constructor not allowed
  sleeping_loop_t() = delete;

  /// \brief copy constructor not allowed
  sleeping_loop_t(const sleeping_loop_t&) = delete;

  /// \brief move constructor
  sleeping_loop_t(sleeping_loop_t&& p_sleep) noexcept
  {
    move(std::move(p_sleep));
  }

  /// \brief destructor interrupts the loop
  inline ~sleeping_loop_t() { stop(); }

  /// \brief copy assignment not allowed
  sleeping_loop_t& operator=(const sleeping_loop_t&) = delete;

  /// \brief move assignment
  sleeping_loop_t& operator=(sleeping_loop_t&& p_sleep) noexcept
  {
    if (this != &p_sleep) {
      move(std::move(p_sleep));
    }
    return *this;
  }

  ///
  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  ///
  inline bool is_stopped() const { return m_async.is_stopped(); }

  ///
  /// \brief get_work
  /// \return a copy of the function that executes a defined work in each
  /// round of the loop
  ///
  inline worker get_work() const { return m_async.get_work(); }

  ///
  /// \brief get_provide
  /// \return a copy of the function that provides an instance of \p t_data,
  /// if available, to the work function
  ///
  inline provider get_provide() const { return m_async.get_provide(); }

  ///
  /// \brief get_timeout
  /// \return the amount of time that the loop will wait for the work function
  /// to finish
  ///
  inline std::chrono::milliseconds get_timeout() const
  {
    return m_async.get_timeout();
  }

  ///
  /// \brief get_interval
  /// \return the interval of loop execution
  ///
  inline std::chrono::milliseconds get_interval() const { return m_interval; }

  /// \brief run starts the loop
  void run()
  {
    if (!m_async.is_stopped()) {
      concurrent_log_debug(
        log, this, " not running async loop because it was not stopped");
      return;
    }
    concurrent_log_debug(log, this, " running async loop");
    m_async.run();
  }

  /// \brief stop stops the loop
  void stop()
  {
    if (m_async.is_stopped()) {
      concurrent_log_debug(
        log, this, " not stopping async loop because it was not running");
      return;
    }
    concurrent_log_debug(log, this, " stop");
    m_cond_var.notify_all();

    concurrent_log_debug(log, this, " all notified, and m_async = ", &m_async);
    m_async.stop();
  }

private:
  /// \brief a simpler name
  typedef concurrent::async_loop_t<t_data, t_log> async_loop;

private:
  ///
  /// \brief move a \p sleeping_loop to this
  /// \param p_sleep the \p sleeping_loop to be moved
  ///
  inline void move(sleeping_loop_t&& p_sleep) noexcept
  {
    // save if the right side was running
    bool _stopped = p_sleep.is_stopped();
    // stop the right side
    p_sleep.stop();

    // move the interval
    m_interval = std::move(p_sleep.m_interval);

    // move the async_loop, reseting the break loop
    m_async = async_loop(std::move(p_sleep.get_work()),
                         std::move(p_sleep.get_timeout()),
                         [this]() -> bool { return this->break_loop(); },
                         std::move(p_sleep.get_provide()));

    // if the right side was not stopped
    if (!_stopped) {
      // run this sleeping_loop
      run();
    }
  }

  ///
  /// \brief break_loop function that defines if the loop should stop
  /// \return \p true if the loop should break; \p false othewise
  ///
  result break_loop()
  {
    std::unique_lock<std::mutex> _lock(m_mutex);
    if (m_cond_var.wait_for(_lock, m_interval) == std::cv_status::timeout) {
      // timeout, so do not stop
      concurrent_log_debug(log, this, " must not stop");
      return result::dont_stop;
    }
    // no timeout, so do stop
    concurrent_log_debug(log, this, " must stop");
    return result::stop;
  }

private:
  /// \brief m_loop the async_loop used
  async_loop m_async;

  /// \brief m_interval
  std::chrono::milliseconds m_interval;

  /// \brief m_mutex
  std::mutex m_mutex;

  /// \brief m_cond_var
  std::condition_variable m_cond_var;
};

} // namespace business
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_SLEEPING_LOOP_H

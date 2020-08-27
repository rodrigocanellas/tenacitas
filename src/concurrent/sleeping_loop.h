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
/// t_log(const char *p_id)
/// void debug(int p_line, const t_params&... p_params)
/// void info(int p_line, const t_params&... p_params)
/// void warn(int p_line, const t_params&... p_params)
/// void error(int p_line, const t_params&... p_params)
/// void fatal(int p_line, const t_params&... p_params)
///
/// \tparam t_interval is the type of time that will be used to define the
/// period of time that the \p p_worker function will be exececuted
///
/// \tparam t_timeout is the type of time used to define how long the \p
/// p_worker function should take
///
template <typename t_data, typename t_log,
          typename t_interval = std::chrono::milliseconds,
          typename t_timeout = std::chrono::milliseconds>
struct sleeping_loop_t {

  /// \brief worker type
  /// \sa traits_t<t_data>::worker in concurrent/traits.h
  typedef typename traits_t<t_data>::worker worker;

  /// \brief provider type
  /// \sa traits_t<t_data>::provider in concurrent/traits.h
  typedef typename traits_t<t_data>::provider provider;

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
  sleeping_loop_t(t_interval p_interval, worker p_worker, t_timeout p_timeout,
                  provider p_provider)
      : m_async(
            p_worker, [this]() -> bool { return this->break_loop(); },
            p_timeout, p_provider),
        m_interval(p_interval) {}

  /// \brief sleeping_loop creates a \p sleeping_loop object, when <tt>t_data
  /// == void</tt>, and a \p provider is not necessary
  ///
  /// \param p_interval the amount of time that the loop will sleep between
  /// executions
  ///
  /// \param p_work function that will be executed each time the loop wakes up
  sleeping_loop_t(t_interval p_interval, worker p_worker, t_timeout p_timeout)
      : m_async(
            p_worker, [this]() -> bool { return this->break_loop(); },
            p_timeout),
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

  /// \brief get_worker
  /// \return a copy of the function that executes a defined work in each
  /// round of the loop
  inline worker get_worker() const { return m_async.get_worker(); }

  /// \brief get_provider
  /// \return a copy of the function that provides an instance of \p t_data,
  /// if available, to the work function
  inline provider get_provider() const { return m_async.get_provider(); }

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

  /// \brief run starts the loop
  /// \tparam t_timeout is the type of timeout, it should be one of
  /// std::chrono::* time types
  ///
  /// \param p_timeout amount of time that the loop will wait for \p p_work to
  /// execute
  ///
  /// \return status::ok if the loop finished with no error, or any other value,
  /// otherwise
  status::result start() {
    if (!m_async.is_stopped()) {
      concurrent_debug(m_log,
                       "not running async loop because it was not stopped");
      return concurrent::already_running;
    }
    concurrent_debug(m_log, "running async loop");
    return m_async.start();
  }

  /// \brief stop stops the loop
  void stop() {
    if (m_async.is_stopped()) {
      concurrent_debug(m_log,
                       "not stopping async loop because it was not running");
      return;
    }
    //    concurrent_debug(m_log, "stop");
    //    m_cond_var.notify_all();

    concurrent_debug(m_log, "all notified");
    m_async.stop();
  }

  /// \brief Stops the loop, and starts it again
  ///
  /// \return \p status::ok if could restart, or any other \p status::result
  /// otherwise
  void restart() {
    concurrent_debug(m_log, "restart");
    stop();
    return start();
  }

private:
  /// \brief a simpler name
  typedef async_loop_t<t_data, t_log, t_timeout> async_loop;

private:
  //  /// \brief move a \p sleeping_loop to this
  //  ///
  //  /// \param p_sleep the \p sleeping_loop to be moved
  //  inline void move(sleeping_loop_t &&p_sleep) noexcept {
  //    // save if the right side was running
  //    bool _stopped = p_sleep.is_stopped();
  //    // stop the right side
  //    p_sleep.stop();

  //    // move the interval
  //    m_interval = std::move(p_sleep.m_interval);

  //    // move the async_loop, reseting the break loop
  //    m_async = async_loop(
  //        std::move(p_sleep.get_worker()),
  //        [this]() -> status::result { return this->break_loop(); },
  //        p_sleep.get_timeout(), std::move(p_sleep.get_provider()));

  //    // if the right side was not stopped
  //    if (!_stopped) {
  //      // run this sleeping_loop
  //      start();
  //    }
  //  }

  /// \brief break_loop function that defines if the loop should stop
  /// \return \p true if the loop should break; \p false othewise
  bool break_loop() {
    std::unique_lock<std::mutex> _lock(m_mutex);
    if (m_cond_var.wait_for(_lock, m_interval) == std::cv_status::timeout) {
      // timeout, so do not stop
      concurrent_debug(m_log, "must not stop");
      return false;
    }
    // no timeout, so do stop
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

  t_log m_log{"concurrent::sleeping_loop"};
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_SLEEPING_LOOP_H

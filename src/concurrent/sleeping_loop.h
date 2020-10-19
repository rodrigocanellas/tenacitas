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

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_log, typename t_timeout, typename t_interval,
          typename... t_params>
struct sleeping_loop_base_t {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_params...)> worker;

  typedef t_interval interval;
  typedef t_timeout timeout;

  /// \brief used to notify about timeout of \p worker
  typedef std::function<void(std::thread::id)> timeout_callback;

  /// \brief default constructor not allowed
  sleeping_loop_base_t() = delete;

  /// \brief copy constructor not allowed
  sleeping_loop_base_t(const sleeping_loop_base_t &) = delete;

  /// \brief move constructor
  sleeping_loop_base_t(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief destructor interrupts the loop
  inline virtual ~sleeping_loop_base_t() {
    concurrent_debug(this->m_log, "destructor");
    stop();
  }

  /// \brief copy assignment not allowed
  sleeping_loop_base_t &operator=(const sleeping_loop_base_t &) = delete;

  /// \brief move assignment
  sleeping_loop_base_t &operator=(sleeping_loop_base_t &&p_sleep) = delete;

  /// \brief get_interval
  /// \return the interval of loop execution
  inline t_interval get_interval() const { return m_interval; }

  /// \brief redefines the value of the execution interval
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  inline void set_interval(t_interval p_interval) { m_interval = p_interval; }

  /// \brief is_stopped
  /// \return \p true if the loop is not running; \p false othewise
  inline bool is_stopped() const { return m_async.is_stopped(); }

  //  inline worker get_worker() const { return m_async.get_worker(); }

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

  /// \brief run starts the loop
  void start() {
    if (!m_async.is_stopped()) {
      concurrent_debug(this->m_log,
                       "not running async loop because it was not stopped");
      return;
    }
    concurrent_debug(this->m_log, "running async loop");
    this->m_stopped = false;
    m_async.start();
  }

  /// \brief stop stops the loop
  void stop() {
    if (m_async.is_stopped()) {
      concurrent_debug(this->m_log,
                       "not stopping async loop because it was not running");
      return;
    }

    concurrent_debug(this->m_log, "stop");

    this->m_stopped = true;

    this->m_cond_var.notify_all();

    m_async.stop();
  }

  /// \brief Stops the loop, and starts it again
  void restart() {
    concurrent_debug(this->m_log, "restart");
    stop();
    start();
  }

protected:
  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

protected:
  sleeping_loop_base_t(t_timeout p_timeout, t_interval p_interval,
                       worker p_worker, timeout_callback p_timeout_callback,
                       provider p_provider)
      : m_async(
            p_timeout, p_worker, [this]() -> bool { return this->breaker(); },
            p_timeout_callback, p_provider),
        m_interval(p_interval) {
    concurrent_debug(m_log, "timeout = ", p_timeout.count(),
                     ", interval = ", p_interval.count());
  }

  sleeping_loop_base_t(t_timeout p_timeout, t_interval p_interval,
                       worker p_worker, timeout_callback p_timeout_callback)
      : m_async(
            p_timeout, p_worker, [this]() -> bool { return this->breaker(); },
            p_timeout_callback),
        m_interval(p_interval) {
    concurrent_debug(m_log, "timeout = ", p_timeout.count(),
                     ", interval = ", p_interval.count());
  }

protected:
  /// \brief a simpler name
  typedef async_loop_t<t_log, t_timeout, true, t_params...> async_loop;

protected:
  /// \brief breaker defines if the loop should stop
  ///
  /// \return \p true if the loop should break; \p false othewise
  bool breaker() {
    concurrent_debug(m_log, "interval = ", m_interval.count());

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

// ############### 1 #########################################################
template <typename t_log, typename t_timeout, typename t_interval,
          typename... t_params>
struct sleeping_loop_t
    : public sleeping_loop_base_t<t_log, t_timeout, t_interval, t_params...> {

  typedef typename sleeping_loop_base_t<t_log, t_timeout, t_interval,
                                        t_params...>::worker worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef typename sleeping_loop_base_t<t_log, t_timeout, t_interval,
                                        t_params...>::provider provider;

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
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  timeout_callback p_timeout_callback, provider p_provider)
      : sleeping_loop_base_t<t_log, t_timeout, t_interval, t_params...>(
            p_timeout, p_interval, p_worker,
            [this]() -> bool { return this->breaker(); }, p_timeout_callback,
            p_provider) {}

  inline provider get_provider() const { return this->m_async.get_provider(); }
};

// ############### 2 #########################################################
template <typename t_log, typename t_timeout, typename t_interval>
struct sleeping_loop_t<t_log, t_timeout, t_interval>
    : public sleeping_loop_base_t<t_log, t_timeout, t_interval> {

  typedef typename sleeping_loop_base_t<t_log, t_timeout, t_interval>::worker
      worker;

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
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval, worker p_worker,
                  timeout_callback p_timeout_callback)
      : sleeping_loop_base_t<t_log, t_timeout, t_interval>(
            p_timeout, p_interval, p_worker, p_timeout_callback) {}
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_SLEEPING_LOOP_H

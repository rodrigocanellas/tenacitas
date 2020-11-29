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
#include <concurrent/time_unit.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Base class for sleeping loops, which are loops that sleep during a
/// certain amount of time, just to wake up, and execute some operation
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the operation expects
template <typename t_log, typename... t_params> struct sleeping_loop_base_t {

  /// \brief type of operation executed in a loop in time intervals
  typedef std::function<void(t_params...)> operation;

  /// \brief used to notify about timeout of \p operation
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

  /// \brief retrieves the interval defined for the loop to sleep between the
  /// execution of the operation
  inline interval get_interval() const { return m_interval; }

  /// \brief redefines the value of the execution interval
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_interval>
  inline void set_interval(t_interval p_interval) {
    m_interval = to_interval(p_interval);
  }

  /// \brief retrieves if the loop is stopped
  inline bool is_stopped() const { return m_async.is_stopped(); }

  /// \brief retrieves the timeout for the operation
  inline timeout get_timeout() const { return m_async.get_timeout(); }

  /// \brief redefines the value of the timeout
  ///
  /// It does not restart the loop, it is necessary to call \p restart
  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
    m_async.set_timeout(p_timeout);
  }

  /// \brief Retrieves the operation defined to be executed
  inline operation get_operation() const { return m_async.get_operation(); }

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
  /// \brief Provider is the type of function that provides data to the
  /// operation
  ///
  /// \return \p an optional tuple of objects needed by the \p operation
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

protected:
  /// \brief Constructor
  ///
  /// \tparam t_timeout type of time used to define the timeout of the operation
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_timeout time used to define the timeout of the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  ///
  /// \param p_timeout_callback function to be executed if \p p_operation times
  /// out
  ///
  /// \param p_provider function that provides the parameters to the \p
  /// p_operation
  template <typename t_timeout, typename t_interval>
  sleeping_loop_base_t(t_timeout p_timeout, t_interval p_interval,
                       operation p_operation,
                       timeout_callback p_timeout_callback, provider p_provider)
      : m_async(
            p_timeout, p_operation,
            [this]() -> bool { return this->breaker(); }, p_timeout_callback,
            p_provider),
        m_interval(to_interval(p_interval)) {
    concurrent_debug(m_log, "timeout = ", p_timeout.count(),
                     ", interval = ", p_interval.count());
  }

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  ///
  /// \param p_provider function that provides the parameters to the \p
  /// p_operation
  template <typename t_interval>
  sleeping_loop_base_t(t_interval p_interval, operation p_operation,
                       provider p_provider)
      : m_async(
            p_operation, [this]() -> bool { return this->breaker(); },
            p_provider),
        m_interval(to_interval(p_interval)) {
    concurrent_debug(m_log, "interval = ", p_interval.count());
  }

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  template <typename t_timeout, typename t_interval>
  sleeping_loop_base_t(t_timeout p_timeout, t_interval p_interval,
                       operation p_operation,
                       timeout_callback p_timeout_callback)
      : m_async(
            p_timeout, p_operation,
            [this]() -> bool { return this->breaker(); }, p_timeout_callback),
        m_interval(to_interval(p_interval)) {
    concurrent_debug(m_log, "timeout = ", p_timeout.count(),
                     ", interval = ", p_interval.count());
  }

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  template <typename t_interval>
  sleeping_loop_base_t(t_interval p_interval, operation p_operation)
      : m_async(p_operation, [this]() -> bool { return this->breaker(); }),
        m_interval(to_interval(p_interval)) {
    concurrent_debug(m_log, "interval = ", p_interval.count());
  }

protected:
  /// \brief a simpler name
  typedef async_loop_t<t_log, true, t_params...> async_loop;

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
  /// \brief async_loop used
  async_loop m_async;

  /// \brief time used to define the execution interval of the operation
  interval m_interval;

  /// \brief controls asynchronous execution
  std::mutex m_mutex;

  /// \brief controls asynchronous execution
  std::condition_variable m_cond_var;

  /// \brief indicates if the loop is stopped
  bool m_stopped{true};

  /// \brief log for the class
  t_log m_log{"concurrent::sleeping_loop"};
};

// ############### 1 #########################################################
/// \brief Specialization of sleeping_loop_base_t for the operation that takes
/// multiple parameters
///
/// \tparam t_log
///
/// \tparam t_params are the parameters that the operation expects
template <typename t_log, typename... t_params>
struct sleeping_loop_t : public sleeping_loop_base_t<t_log, t_params...> {

  /// \brief Operation to be executed at a defined interval
  typedef
      typename sleeping_loop_base_t<t_log, t_params...>::operation operation;

  /// \brief provider is the type of function that provides data to the
  /// operation
  ///
  /// \return \p an optional tuple of objects needed by the \p operation
  typedef typename sleeping_loop_base_t<t_log, t_params...>::provider provider;

  /// \brief Constructor
  ///
  /// \tparam t_timeout type of time used to define the timeout of the operation
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_timeout time used to define the timeout of the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  ///
  /// \param p_timeout_callback function to be executed if \p p_operation times
  /// out
  ///
  /// \param p_provider function that provides the parameters to the \p
  /// p_operation
  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval,
                  operation p_operation, timeout_callback p_timeout_callback,
                  provider p_provider)
      : sleeping_loop_base_t<t_log, t_params...>(
            p_timeout, p_interval, p_operation,
            [this]() -> bool { return this->breaker(); }, p_timeout_callback,
            p_provider) {}

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  ///
  /// \param p_timeout_callback function to be executed if \p p_operation times
  /// out
  ///
  /// \param p_provider function that provides the parameters to the \p
  /// p_operation
  template <typename t_interval>
  sleeping_loop_t(t_interval p_interval, operation p_operation,
                  timeout_callback p_timeout_callback, provider p_provider)
      : sleeping_loop_base_t<t_log, t_params...>(
            p_interval, p_operation,
            [this]() -> bool { return this->breaker(); }, p_timeout_callback,
            p_provider) {}

  /// \brief Retrieves the provider function
  inline provider get_provider() const { return this->m_async.get_provider(); }
};

// ############### 2 #########################################################
/// \brief Specialization of sleeping_loop_base_t for the operation that takes
/// no parameters
///
/// \tparam t_log
template <typename t_log>
struct sleeping_loop_t<t_log> : public sleeping_loop_base_t<t_log> {

  /// \brief Operation to be executed at a defined interval
  typedef typename sleeping_loop_base_t<t_log>::operation operation;

  /// \brief Constructor
  ///
  /// \tparam t_timeout type of time used to define the timeout of the operation
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_timeout time used to define the timeout of the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  ///
  /// \param p_timeout_callback function to be executed if \p p_operation times
  /// out
  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(t_timeout p_timeout, t_interval p_interval,
                  operation p_operation, timeout_callback p_timeout_callback)
      : sleeping_loop_base_t<t_log>(p_timeout, p_interval, p_operation,
                                    p_timeout_callback) {}

  /// \brief Constructor
  ///
  /// \tparam t_interval type of time used to define the execution interval of
  /// the operation
  ///
  /// \param p_interval time used to define the execution interval of the
  /// operation
  ///
  /// \param p_operation the operation to be executed at \p p_interval
  ///
  /// \param p_timeout_callback function to be executed if \p p_operation times
  /// out
  template <typename t_interval>
  sleeping_loop_t(t_interval p_interval, operation p_operation)
      : sleeping_loop_base_t<t_log>(p_interval, p_operation) {}
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_SLEEPING_LOOP_H

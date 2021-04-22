#ifndef TENACITAS_ASYNC_H
#define TENACITAS_ASYNC_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <set>
#include <thread>
#include <vector>

#include <tenacitas/calendar.h>
#include <tenacitas/internal/internal_async.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>
#include <tenacitas/number.h>

using namespace std::chrono_literals;

/// TODO improve documentation

/// \brief master namespace
namespace tenacitas {

/// \brief support for async (asynchronous) programming
namespace async {

/// \brief Priority
struct priority {

  priority() = delete;

  priority(const priority &) = default;
  priority(priority &&) = default;

  ~priority() = default;

  friend std::ostream &operator<<(std::ostream &p_out, priority p_priority) {
    p_out << static_cast<uint16_t>(p_priority.m_value);
    return p_out;
  }

  bool operator<(priority p_priority) const {
    return m_value < p_priority.m_value;
  }

  bool operator>(priority p_priority) const {
    return m_value > p_priority.m_value;
  }

  bool operator==(priority p_priority) const {
    return m_value == p_priority.m_value;
  }

  bool operator!=(priority p_priority) const {
    return m_value != p_priority.m_value;
  }

  priority &operator=(const priority &) = default;
  priority &operator=(priority &&) = default;

  static const priority lowest;

  static const priority low;

  static const priority low_middle;

  static const priority middle;

  static const priority middle_high;

  static const priority high;

  static const priority highest;

private:
  explicit priority(uint8_t p_value) : m_value(p_value) {}

private:
  uint8_t m_value;
};

const priority priority::lowest{1};
const priority priority::low{45};
const priority priority::low_middle{90};
const priority priority::middle{135};
const priority priority::middle_high{180};
const priority priority::high{225};
const priority priority::highest{255};

template <typename... t_params>
struct sleeping_loop_t : public internal::loop_t<t_params...> {

  /// \brief Signature of the function that will be called in each round of the
  /// loop
  typedef std::function<void(std::shared_ptr<bool>, t_params...)> function;

  /// \brief Signature of the function that will be called in the loop to
  /// provide the parameters for the function
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  sleeping_loop_t() = delete;

  sleeping_loop_t(sleeping_loop_t &&p_loop)
      : internal::loop_t<t_params...>(std::move(p_loop)),
        m_interval(std::move(p_loop.m_interval)) {}

  sleeping_loop_t &operator=(sleeping_loop_t &&p_loop) {
    this->move_assign(std::move(p_loop));
    m_interval = std::move(p_loop.m_interval);
    return *this;
  }

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(const number::id &p_owner, function p_function,
                  t_timeout p_timeout, provider p_provider,
                  t_interval p_interval)
      : internal::loop_t<t_params...>(p_owner, p_function, p_timeout,
                                      p_provider),
        m_interval(calendar::convert<internal::interval>(p_interval)) {}

protected:
  void loop() {
    DEB(this->m_log, this->m_owner, ':', this->m_id,
        " - entering loop, m_timeout = ", this->m_timeout.count());
    while (true) {
      if (!this->core()) {
        break;
      }
      if (!internal::sleeping_loop_control(this->m_stopped, this->m_log,
                                           this->m_cond_stop, m_interval,
                                           this->m_owner, this->m_id)) {
        break;
      }
      //      {
      //        if (this->m_stopped) {
      //          DEB(this->m_log, this->m_owner, ':', this->m_id, " - stop");
      //        }

      //        DEB(this->m_log, this->m_owner, ':', this->m_id, " - waiting for
      //        ",
      //            m_interval.count(), "ms to elaps, or a stop order");
      //        std::unique_lock<std::mutex> _lock(m_mutex_stop);
      //        if (!this->m_cond_stop.wait_for(_lock, m_interval, [this]() ->
      //        bool {
      //              return this->m_stopped;
      //            })) {
      //          if (this->m_stopped) {
      //            DEB(this->m_log, this->m_owner, ':', this->m_id,
      //                " - ordered to stop");
      //            break;
      //          }
      //          DEB(this->m_log, this->m_owner, ':', this->m_id, " - ",
      //              m_interval.count(), "ms elapsed");
      //        }
      //      }
    }
    DEB(this->m_log, this->m_owner, ':', this->m_id, " - leaving loop");
  }

private:
  internal::interval m_interval;
  //  std::mutex m_mutex_stop;
};

template <> struct sleeping_loop_t<void> : public internal::loop_t<void> {

  /// \brief Signature of the function that will be called in each round of the
  /// loop
  typedef std::function<void(std::shared_ptr<bool>)> function;

  sleeping_loop_t() = delete;

  sleeping_loop_t(const sleeping_loop_t &) = delete;
  sleeping_loop_t &operator=(const sleeping_loop_t &) = delete;

  sleeping_loop_t(sleeping_loop_t &&p_loop)
      : internal::loop_t<void>(std::move(p_loop)),
        m_interval(std::move(p_loop.m_interval)) {}

  sleeping_loop_t &operator=(sleeping_loop_t &&p_loop) {
    this->move_assign(std::move(p_loop));
    m_interval = std::move(p_loop.m_interval);
    return *this;
  }

  template <typename t_timeout, typename t_interval>
  sleeping_loop_t(const number::id &p_owner, function p_function,
                  t_timeout p_timeout, t_interval p_interval)
      : internal::loop_t<void>(p_owner, p_function, p_timeout),
        m_interval(calendar::convert<internal::interval>(p_interval)) {}

protected:
  void loop() {
    DEB(this->m_log, this->m_owner, ':', this->m_id,
        " - entering loop, m_timeout = ", this->m_timeout.count());
    while (true) {
      if (!core()) {
        break;
      }
      if (!internal::sleeping_loop_control(this->m_stopped, this->m_log,
                                           this->m_cond_stop, m_interval,
                                           this->m_owner, this->m_id)) {
        break;
      }

      //      if (this->m_stopped) {
      //        DEB(this->m_log, this->m_owner, ':', this->m_id, " - stop");
      //      }
      //      {
      //        DEB(this->m_log, this->m_owner, ':', this->m_id, " - waiting for
      //        ",
      //            m_interval.count(), "ms to elaps, or a stop order");
      //        std::unique_lock<std::mutex> _lock(m_mutex_stop);
      //        if (!m_cond_stop.wait_for(_lock, m_interval,
      //                                  [this]() -> bool { return m_stopped;
      //                                  })) {
      //          if (m_stopped) {
      //            DEB(this->m_log, this->m_owner, ':', this->m_id,
      //                " - ordered to stop");
      //            break;
      //          }
      //          DEB(this->m_log, this->m_owner, ':', this->m_id, " - ",
      //              m_interval.count(), "ms elapsed");
      //        }
      //      }
    }
    DEB(this->m_log, this->m_owner, ':', this->m_id, " - leaving loop");
  }

private:
  internal::interval m_interval;
  //  std::mutex m_mutex_stop;
};

///// \brief Breaker function signature
///// Used to stop a loop
// typedef internal::breaker breaker;

///// \brief Base class for sleeping loops, which are loops that sleep during
///// a certain amount of time, then wake up and execute some work
/////
///// \tparam t_params are the parameters that the work function
// template <typename... t_params> struct sleeping_loop_t {

//  /// \brief Type of function to be executed in a loop in time intervals
//  typedef std::function<void(t_params...)> function;

//  /// \brief Type of function to be executed if a function times out
//  typedef std::function<void(t_params...)> on_timeout;

//  /// \brief Provider is the type of function that provides data to the
//  /// work function
//  ///
//  /// \return \p an optional tuple of objects needed by the \p worker
//  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

//  /// \brief Constructor
//  ///
//  /// \tparam t_timeout type of time used to define the timeout of the
//  /// work function
//  ///
//  /// \tparam t_interval type of time used to define the execution interval
//  /// of the work function
//  ///
//  /// \param p_timeout time used to define the timeout of the work function
//  ///
//  /// \param p_interval time used to define the execution interval of the
//  /// work function
//  ///
//  /// \param p_function the work function to be executed at \p p_interval
//  ///
//  /// \param p_on_timeout function to be executed if \p p_function times out
//  ///
//  /// \param p_provider function that provides the parameters to the \p
//  /// p_function
//  template <typename t_timeout, typename t_interval>
//  sleeping_loop_t(const number::id &p_owner, t_timeout p_timeout,
//                  t_interval p_interval, function p_function,
//                  on_timeout p_on_timeout, provider p_provider)
//      : m_impl(std::make_unique<implementation>(p_owner, p_timeout,
//      p_interval,
//                                                p_function, p_on_timeout,
//                                                p_provider)) {}

//  /// \brief Constructor
//  /// Using this constructor, if a work function times out, nothing will be
//  done
//  ///
//  /// \tparam t_timeout type of time used to define the timeout of the
//  /// work function
//  ///
//  /// \tparam t_interval type of time used to define the execution interval
//  /// of the work function
//  ///
//  /// \param p_timeout time used to define the timeout of the work function
//  ///
//  /// \param p_interval time used to define the execution interval of the
//  /// work function
//  ///
//  /// \param p_function the work function to be executed at \p p_interval
//  ///
//  /// \param p_provider function that provides the parameters to the \p
//  /// p_function
//  template <typename t_timeout, typename t_interval>
//  sleeping_loop_t(const number::id &p_owner, t_timeout p_timeout,
//                  t_interval p_interval, function p_function,
//                  provider p_provider)
//      : m_impl(std::make_unique<implementation>(p_owner, p_timeout,
//      p_interval,
//                                                p_function, p_provider)) {}

//  /// \brief Default constructor not allowed
//  sleeping_loop_t() = delete;

//  ~sleeping_loop_t() = default;

//  /// \brief Copy constructor not allowed
//  sleeping_loop_t(const sleeping_loop_t &) = delete;

//  sleeping_loop_t(sleeping_loop_t &&p_sl) = default;

//  /// \brief Copy assignment not allowed
//  sleeping_loop_t &operator=(const sleeping_loop_t &) = delete;

//  sleeping_loop_t &operator=(sleeping_loop_t &&p_sl) = default;

//  /// \brief Sets a new interval for the loop
//  ///
//  /// \tparam t_interval type of time used to define the execution interval
//  /// of the work function
//  ///
//  /// \param p_interval time used to define the execution interval of the
//  /// worker
//  template <typename t_interval>
//  inline void set_interval(t_interval p_interval) {
//    m_impl->set_interval(p_interval);
//  }

//  /// \brief Retrieves the interval for the loop
//  ///
//  /// \tparam t_time type of time used to convert the interval time to
//  ///
//  /// \return the interval time
//  template <typename t_time> inline t_time get_interval() const {
//    return m_impl->template get_interval<t_time>();
//  }

//  const number::id &get_owner() const { return m_impl->get_owner(); }

//  const number::id &get_id() const { return m_impl->get_id(); }

//  /// \brief Starts the loop
//  inline void start() { m_impl->start(); }

//  /// \brief Stops the loop
//  inline void stop() { m_impl->stop(); }

//  /// \brief Starts and stops the loop
//  inline void restart() { m_impl->restart(); }

//  /// \brief Retrieves if the loop is stopped
//  inline bool is_stopped() const { return m_impl->is_stopped(); }

//  /// \brief Retrieves the timeout for the worker
//  ///
//  /// \tparam t_time type of time used to convert the timeout to
//  template <typename t_time> inline t_time get_timeout() const {
//    return m_impl->template get_timeout<t_time>();
//  }

//  /// \brief Redefines the value of the timeout
//  ///
//  /// \tparam t_timeout is the type of time used to define timeout for the
//  /// worker function
//  ///
//  /// It does not restart the loop, it is necessary to call \p restart
//  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
//    m_impl->set_timeout(p_timeout);
//  }

// private:
//  /// \brief Implementation of the loop
//  /// Using an internal implentation makes it easy to move loop objects
//  struct implementation
//      : public internal::sleeping_loop_base_t<on_timeout, t_params...> {

//    /// \brief Constructor
//    ///
//    /// \tparam t_timeout is the type of time used to define the
//    ///
//    /// \tparam t_interval is the type of time used to define the interval of
//    /// the loop execution
//    ///
//    /// \param p_timeout is the maximum time that the work function has to
//    /// execute
//    ///
//    /// \param p_interval is the time interval that the loop will execute
//    ///
//    /// \param p_function is the work function that will be executed at each
//    \p
//    /// p_interval
//    ///
//    /// \param p_on_timeout is the function that will be called if \p
//    p_function
//    /// times out
//    ///
//    /// \param p_provider is the function that will provide data for the \p
//    /// p_function
//    template <typename t_timeout, typename t_interval>
//    implementation(const number::id &p_owner, t_timeout p_timeout,
//                   t_interval p_interval, function p_function,
//                   on_timeout p_on_timeout, provider p_provider)
//        : internal::sleeping_loop_base_t<on_timeout, t_params...>(
//              p_owner, p_interval, p_function, p_timeout, p_on_timeout,
//              p_provider) {
//      DEB(this->m_log, this->m_owner, ':', this->m_id,
//          " - timeout = ", p_timeout.count(),
//          ", interval = ", p_interval.count());
//    }

//    /// \brief Constructor
//    /// Using this constructor, if a work function times out, no action will
//    be
//    /// taken
//    ///
//    /// \tparam t_timeout is the type of time used to define the
//    ///
//    /// \tparam t_interval is the type of time used to define the interval of
//    /// the loop execution
//    ///
//    /// \param p_timeout is the maximum time that the work function has to
//    /// execute
//    ///
//    /// \param p_interval is the time interval that the loop will execute
//    ///
//    /// \param p_function is the work function that will be executed at each
//    \p
//    /// p_interval
//    ///
//    /// \param p_provider is the function that will provide data for the \p
//    /// p_function
//    template <typename t_timeout, typename t_interval>
//    implementation(const number::id &p_owner, t_timeout p_timeout,
//                   t_interval p_interval, function p_function,
//                   provider p_provider)
//        : internal::sleeping_loop_base_t<on_timeout, t_params...>(
//              p_owner, p_interval, p_function, p_timeout, p_provider) {
//      DEB(this->m_log, this->m_owner, ':', this->m_id,
//          " - timeout = ", p_timeout.count(),
//          ", interval = ", p_interval.count());
//    }
//  };

// private:
//  /// \brief Implementation pointer
//  std::unique_ptr<implementation> m_impl;
//};

///// \brief Specialization of sleeping_loop_t for a work function that takes no
///// parameter
// template <> struct sleeping_loop_t<void> {

//  /// \brief Type of work function executed in a loop in time intervals
//  typedef std::function<void()> function;

//  /// \brief Type of function to be executed if a function times out
//  typedef std::function<void()> on_timeout;

//  /// \brief Constructor
//  ///
//  /// \tparam t_timeout is the type of time used to define the
//  ///
//  /// \tparam t_interval is the type of time used to define the interval of
//  the
//  /// loop execution
//  ///
//  /// \param p_timeout is the maximum time that the work function has to
//  /// execute
//  ///
//  /// \param p_interval is the time interval that the loop will execute
//  ///
//  /// \param p_function is the work function that will be executed at each \p
//  /// p_interval
//  ///
//  /// \param p_on_timeout is the function that will be called if \p p_function
//  /// times out
//  template <typename t_timeout, typename t_interval>
//  sleeping_loop_t(const number::id &p_owner, t_timeout p_timeout,
//                  t_interval p_interval, function p_function,
//                  on_timeout p_on_timeout)
//      : m_impl(std::make_unique<implementation>(p_owner, p_timeout,
//      p_interval,
//                                                p_function, p_on_timeout)) {}

//  /// \brief Constructor
//  /// Using this constructor, if a work function times out, no action will be
//  /// taken
//  ///
//  /// \tparam t_timeout is the type of time used to define the
//  ///
//  /// \tparam t_interval is the type of time used to define the interval of
//  the
//  /// loop execution
//  ///
//  /// \param p_timeout is the maximum time that the work function has to
//  /// execute
//  ///
//  /// \param p_interval is the time interval that the loop will execute
//  ///
//  /// \param p_function is the work function that will be executed at each \p
//  /// p_interval
//  template <typename t_timeout, typename t_interval>
//  sleeping_loop_t(const number::id &p_owner, t_timeout p_timeout,
//                  t_interval p_interval, function p_function)
//      : m_impl(std::make_unique<implementation>(p_owner, p_timeout,
//      p_interval,
//                                                p_function)) {}

//  /// \brief Default constructor is not allowed
//  sleeping_loop_t() = delete;

//  ~sleeping_loop_t() = default;

//  /// \brief Copy constructor is not allowed
//  sleeping_loop_t(const sleeping_loop_t &) = delete;

//  sleeping_loop_t(sleeping_loop_t &&p_sl) = default;

//  /// \brief Assignment operator not allowed
//  sleeping_loop_t &operator=(const sleeping_loop_t &) = delete;

//  sleeping_loop_t &operator=(sleeping_loop_t &&p_sl) = default;

//  /// \brief Sets a new interval for the loop
//  ///
//  /// \tparam t_interval type of time used to define the execution interval
//  /// of the work function
//  ///
//  /// \param p_interval time used to define the execution interval of the
//  /// worker
//  template <typename t_interval>
//  inline void set_interval(t_interval p_interval) {
//    m_impl->set_interval(p_interval);
//  }

//  const number::id &get_owner() const { return m_impl->get_owner(); }

//  const number::id &get_id() const { return m_impl->get_id(); }

//  /// \brief Retrieves the interval for the loop
//  ///
//  /// \tparam t_time type of time used to convert the interval time to
//  ///
//  /// \return the interval time
//  template <typename t_time> inline t_time get_interval() const {
//    return m_impl->template get_interval<t_time>();
//  }

//  /// \brief Starts the loop
//  inline void start() { m_impl->start(); }

//  /// \brief Stops the loop
//  inline void stop() { m_impl->stop(); }

//  /// \brief Starts and stops the loop
//  inline void restart() { m_impl->restart(); }

//  /// \brief Retrieves if the loop is stopped
//  inline bool is_stopped() const { return m_impl->is_stopped(); }

//  /// \brief Retrieves the timeout for the worker
//  ///
//  /// \tparam t_time type of time used to convert the timeout to
//  template <typename t_time> inline t_time get_timeout() const {
//    return m_impl->template get_timeout<t_time>();
//  }

//  /// \brief Redefines the value of the timeout
//  ///
//  /// \tparam t_timeout is the type of time used to define timeout for the
//  /// worker function
//  ///
//  /// It does not restart the loop, it is necessary to call \p restart
//  template <typename t_timeout> inline void set_timeout(t_timeout p_timeout) {
//    m_impl->set_timeout(p_timeout);
//  }

// private:
//  /// \brief Implementation of the loop
//  /// Using an internal implentation makes it easy to move loop objects
//  struct implementation
//      : public internal::sleeping_loop_base_t<std::function<void()>, void> {

//    /// \brief Constructor
//    ///
//    /// \tparam t_timeout is the type of time used to define the
//    ///
//    /// \tparam t_interval is the type of time used to define the interval of
//    /// the loop execution
//    ///
//    /// \param p_timeout is the maximum time that the work function has to
//    /// execute
//    ///
//    /// \param p_interval is the time interval that the loop will execute
//    ///
//    /// \param p_function is the work function that will be executed at each
//    \p
//    /// p_interval
//    ///
//    /// \param p_on_timeout is the function that will be called if \p
//    p_function
//    /// times out
//    template <typename t_timeout, typename t_interval>
//    implementation(const number::id &p_owner, t_timeout p_timeout,
//                   t_interval p_interval, function p_function,
//                   on_timeout p_on_timeout)
//        : internal::sleeping_loop_base_t<on_timeout, void>(
//              p_owner, p_interval, p_function, p_timeout, p_on_timeout) {

//      DEB(this->m_log, this->m_owner, ':', this->m_id,
//          " - timeout = ", p_timeout.count(),
//          ", interval = ", p_interval.count());
//    }

//    /// \brief Constructor
//    /// Using this constructor, if a work function times out, no action will
//    be
//    /// taken
//    ///
//    /// \tparam t_timeout is the type of time used to define the
//    ///
//    /// \tparam t_interval is the type of time used to define the interval of
//    /// the loop execution
//    ///
//    /// \param p_timeout is the maximum time that the work function has to
//    /// execute
//    ///
//    /// \param p_interval is the time interval that the loop will execute
//    ///
//    /// \param p_function is the work function that will be executed at each
//    \p
//    /// p_interval
//    template <typename t_timeout, typename t_interval>
//    implementation(const number::id &p_owner, t_timeout p_timeout,
//                   t_interval p_interval, function p_function)
//        : internal::sleeping_loop_base_t<on_timeout, void>(
//              p_owner, p_interval, p_function, p_timeout) {

//      DEB(this->m_log, this->m_owner, ':', this->m_id,
//          " - timeout = ", p_timeout.count(),
//          ", interval = ", p_interval.count());
//    }
//  };

// private:
//  /// \brief Implementation pointer
//  std::unique_ptr<implementation> m_impl;
//};

// #########################################

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
template <typename t_msg, typename t_time>
static inline number::id
add_handlers(const priority &p_priority, t_time p_timeout,
             std::function<void(const t_msg &)> p_on_timeout) {
  return internal::messenger_t<t_msg, priority>::add_handlers(
      p_priority, p_timeout, p_on_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \details In this configuration, if any handler times out, the message will
/// be added again to the queue
template <typename t_msg, typename t_time>
static inline number::id add_handlers(const priority &p_priority,
                                      t_time p_timeout) {
  return internal::messenger_t<t_msg, priority>::add_handlers(p_priority,
                                                              p_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message.
template <typename t_msg, typename t_time>
static number::id
add_handlers(t_time p_timeout,
             std::function<void(const t_msg &)> p_on_timeout) {
  return internal::messenger_t<t_msg, priority>::add_handlers(p_timeout,
                                                              p_on_timeout);
}

/// \brief Adds a queue to receive messages to be handled
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_id is the identifier of the queue
///
/// \param p_timeout is the maximum amount of time that the handler function
/// will have to complete its work.
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message; and if any
/// handler times out, the message will be added again to the queue
template <typename t_msg, typename t_time>
static inline number::id add_handlers(t_time p_timeout) {
  return internal::messenger_t<t_msg, priority>::add_handlers(p_timeout);
}

/// \brief Defines the priority of a message queue
/// The priority of the queue defines the order in which a message will be
/// added.
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_id is the identifier of the queue
///
/// \param p_priority is the priority of the queue. This defines the order that
/// the message will be placed in the queue.
template <typename t_msg>
static inline void set_priority(const number::id &p_handlers,
                                priority p_priority) {
  internal::messenger_t<t_msg, priority>::set_priority(p_handlers, p_priority);
}

/// \brief Retrieves the priority of a message queue
/// The priority of the queue defines the order in which a message will be
/// added.
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_id is the identifier of the queue
///
/// \return The priority of the queue
template <typename t_msg>
static inline std::optional<priority>
get_priority(const number::id &p_handlers) {
  return internal::messenger_t<t_msg, priority>::get_priority(p_handlers);
}

/// \brief Sends a message to all the queues associated to a message type
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_msg is the message to be copied to all the queues
template <typename t_msg> static inline void send(const t_msg &p_msg) {
  internal::messenger_t<t_msg, priority>::send(p_msg);
}

/// \brief Adds a handler to a queue
/// It is possible to add many handlers to the same queue. Those handlers will
/// compete with each other to handle a added message.
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_id is the identifier of the queue to which this handler will be
/// added to
///
/// \param p_handler is the function that will handle a message added to the
/// queue
template <typename t_msg>
static inline void add_handler(const number::id &p_handlers,
                               std::function<void(const t_msg &)> p_handler) {
  internal::messenger_t<t_msg, priority>::add_handler(p_handlers, p_handler);
}

/// \brief Adds a handler to a queue
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \tparam t_time is the type of time used to define the timeout
///
/// \param p_handler is the function that will handle a message added to the
/// queue
///
/// \param p_on_timeout is the function that will be called to handle the
/// message that the handler function could not handle in time.
///
/// \return An auto generated \p id of the queue
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message; and if any
/// handler times out, the message will be added again to the queue
template <typename t_msg, typename t_time = std::chrono::minutes>
static inline number::id
add_handler(std::function<void(const t_msg &)> p_handler,
            t_time p_timeout = 10min) {
  return internal::messenger_t<t_msg, priority>::add_handler(p_handler,
                                                             p_timeout);
}

template <typename t_msg, typename t_priority>
static inline void traverse(std::function<void(const number::id &, t_priority,
                                               std::chrono::milliseconds)>
                                p_visitor) {
  internal::messenger_t<t_msg, t_priority>::traverse(p_visitor);
}

} // namespace async
} // namespace tenacitas

#endif

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
#include <tenacitas/number.h>

using namespace std::chrono_literals;

/// TODO improve documentation

/// \brief master namespace
namespace tenacitas {

/// \brief support for async (asynchronous) programming
namespace async {

typedef internal::priority priority;

// template <typename... t_params>
// struct sleeping_loop_t : public internal::loop_t<t_params...> {

//  /// \brief Signature of the function that will be called in each round of
//  the
//  /// loop
//  typedef std::function<void(std::shared_ptr<bool>, t_params...)> function;

//  /// \brief Signature of the function that will be called in the loop to
//  /// provide the parameters for the function
//  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

//  sleeping_loop_t() = delete;

//  sleeping_loop_t(sleeping_loop_t &&p_loop)
//      : internal::loop_t<t_params...>(std::move(p_loop)),
//        m_interval(std::move(p_loop.m_interval)) {}

//  sleeping_loop_t &operator=(sleeping_loop_t &&p_loop) {
//    this->move_assign(std::move(p_loop));
//    m_interval = std::move(p_loop.m_interval);
//    return *this;
//  }

//  template <typename t_timeout, typename t_interval>
//  sleeping_loop_t(const number::id &p_owner, function p_function,
//                  t_timeout p_timeout, provider p_provider,
//                  t_interval p_interval)
//      : internal::loop_t<t_params...>(p_owner, p_function, p_timeout,
//                                      p_provider),
//        m_interval(calendar::convert<internal::interval>(p_interval)) {}

// protected:
//  virtual bool core() override {
//    if (!internal::loop_t<t_params...>::core()) {
//      return false;
//    }
//    if (!internal::sleeping_loop_control(this->m_stopped, this->m_log,
//                                         this->m_cond_stop, m_interval,
//                                         this->m_owner, this->m_id)) {
//      return false;
//    }
//    return true;
//  }

// private:
//  internal::interval m_interval;
//  //  std::mutex m_mutex_stop;
//};

// template <> struct sleeping_loop_t<void> : public internal::loop_t<void> {

struct sleeping_loop {

  /// \brief Signature of the function that will be called in each round of the
  /// loop
  typedef std::function<void(std::shared_ptr<bool>)> function;

  sleeping_loop() = delete;

  sleeping_loop(const sleeping_loop &) = delete;
  sleeping_loop &operator=(const sleeping_loop &) = delete;

  sleeping_loop(sleeping_loop &&p_loop) {
    bool _stopped = p_loop.m_stopped;

    m_owner = std::move(p_loop.m_owner);
    m_function = std::move(p_loop.m_function);
    m_timeout = std::move(p_loop.m_timeout);
    m_interval = std::move(p_loop.m_interval);
    m_stopped = false;

    if (!_stopped) {
      start();
    }
  }

  sleeping_loop &operator=(sleeping_loop &&p_loop) {
    if (this != &p_loop) {

      bool _stopped = p_loop.m_stopped;

      m_owner = std::move(p_loop.m_owner);
      m_function = std::move(p_loop.m_function);
      m_timeout = std::move(p_loop.m_timeout);
      m_interval = std::move(p_loop.m_interval);
      m_stopped = false;

      if (!_stopped) {
        start();
      }
    }
    return *this;
  }

  template <typename t_timeout, typename t_interval>
  sleeping_loop(const number::id &p_owner, function p_function,
                t_timeout p_timeout, t_interval p_interval)
      : m_owner(p_owner), m_function(p_function),
        m_timeout(calendar::convert<internal::timeout>(p_timeout)),
        m_interval(calendar::convert<internal::interval>(p_interval)) {}

  void start() {
    if (!m_stopped) {
      return;
    }
    m_stopped = false;

    DEB(m_log, m_owner, ':', m_id, " - starting loop thread");
    m_thread = std::thread([this]() { loop(); });
  }

  /// \brief Stops the loop, if it was started
  void stop() {

    if (m_stopped) {
      DEB(m_log, m_owner, ':', m_id, " - not stopping because it is stopped");
      return;
    }
    DEB(m_log, m_owner, ':', m_id, " - stopping");
    m_stopped = true;
    m_cond.notify_one();

    if (m_thread.get_id() == std::thread::id()) {
      DEB(m_log, m_owner, ':', m_id,
          " - not joining because m_thread.get_id() == std::thread::id()");
      return;
    }
    DEB(m_log, m_owner, ':', m_id, " - m_thread.get_id() != std::thread::id()");

    if (!m_thread.joinable()) {
      DEB(m_log, m_owner, ':', m_id,
          " - not joining because m_thread is not joinable");
      return;
    }
    DEB(m_log, m_owner, ':', m_id, " - m_thread.joinable()");

    {
      std::lock_guard<std::mutex> _lock(m_mutex_join);
      m_thread.join();
    }
    DEB(m_log, m_owner, ':', m_id, " - leaving stop");
  }

private:
  void loop() {
    DEB(m_log, m_owner, ':', m_id,
        " - entering loop, m_timeout = ", m_timeout.count());

    while (true) {
      if (m_stopped) {
        DEB(m_log, m_owner, ':', m_id, " - stop");
        break;
      }

      DEB(m_log, m_owner, ':', m_id, " - calling handler");

      if (!internal::execute(m_timeout,
                             [this](std::shared_ptr<bool> p_stop) -> void {
                               m_function(p_stop);
                             })) {
        WAR(m_log, m_owner, ':', m_id, " - timeout");
      }

      if (m_stopped) {
        DEB(m_log, m_owner, ':', m_id, " - stop");
        break;
      }

      DEB(m_log, m_owner, ':', m_id, " - waiting for ", m_interval.count(),
          "ms to elaps, or a stop order");
      {
        std::unique_lock<std::mutex> _lock(m_mutex_interval);
        if ((m_cond.wait_for(_lock, m_interval) ==
             std::cv_status::no_timeout) &&
            (m_stopped)) {
          DEB(m_log, m_owner, ':', m_id, " - ordered to stop");
          break;
        }
      }
      DEB(m_log, m_owner, ':', m_id, " - ", m_interval.count(), "ms elapsed");
    }
    DEB(m_log, m_owner, ':', m_id, " - leaving loop");
  }

private:
  /// \brief Identifier of the object that owns this object
  number::id m_owner;

  /// \brief Function that will be called in each round of the loop
  function m_function;

  /// \brief Maximum time that \p m_function will have to execute
  internal::timeout m_timeout;

  internal::interval m_interval;

  /// \brief Identifier of this object, used for debugging purposes
  number::id m_id;

  /// \brief Indicates that the loop must stop
  std::atomic<bool> m_stopped{true};

  /// \brief Thread where the \p loop method will run
  std::thread m_thread;

  /// \brief Logger
  logger::cerr<> m_log{"sleeping_loop"};

  /// \brief Protects joining the thread
  std::mutex m_mutex_join;

  std::mutex m_mutex_interval;

  std::condition_variable m_cond;

  //  std::mutex m_mutex_stop;
};

// #########################################

///// \brief Adds a queue to receive messages to be handled
/////
///// \tparam t_msg is the type of message to be added to the queue
/////
///// \tparam t_time is the type of time used to define the timeout
/////
///// \param p_id is the identifier of the queue
/////
///// \param p_priority is the priority of the queue. This defines the order
/// that
///// the message will be placed in the queue.
/////
///// \param p_timeout is the maximum amount of time that the handler function
///// will have to complete its work.
/////
// template <typename t_msg, typename t_time>
// static inline number::id add_handlers(const priority &p_priority,
//                                      t_time p_timeout) {
//  return internal::messenger_t<t_msg>::add_handlers(p_priority, p_timeout);
//}

///// \brief Adds a queue to receive messages to be handled
/////
///// \tparam t_msg is the type of message to be added to the queue
/////
///// \tparam t_time is the type of time used to define the timeout
/////
///// \param p_id is the identifier of the queue
/////
///// \param p_timeout is the maximum amount of time that the handler function
///// will have to complete its work.
/////
///// \details In this configuration, this queue has the lowest priority,
/// i.e.,
///// this queue will be the last to receive a copy of the message.
// template <typename t_msg, typename t_time>
// static number::id add_handlers(t_time p_timeout) {
//  return internal::messenger_t<t_msg>::add_handlers(p_timeout);
//}

///// \brief Defines the priority of a message queue
///// The priority of the queue defines the order in which a message will be
///// added.
/////
///// \tparam t_msg is the type of message to be added to the queue
/////
///// \param p_id is the identifier of the queue
/////
///// \param p_priority is the priority of the queue. This defines the order
/// that
///// the message will be placed in the queue.
// template <typename t_msg>
// static inline void set_priority(const number::id &p_handlers,
//                                priority p_priority) {
//  internal::messenger_t<t_msg>::set_priority(p_handlers, p_priority);
//}

///// \brief Retrieves the priority of a message queue
///// The priority of the queue defines the order in which a message will be
///// added.
/////
///// \tparam t_msg is the type of message to be added to the queue
/////
///// \param p_id is the identifier of the queue
/////
///// \return The priority of the queue
// template <typename t_msg>
// static inline std::optional<priority>
// get_priority(const number::id &p_handlers) {
//  return internal::messenger_t<t_msg>::get_priority(p_handlers);
//}

///// \brief Sends a message to all the queues associated to a message type
/////
///// \tparam t_msg is the type of message to be added to the queue
/////
///// \param p_msg is the message to be copied to all the queues
// template <typename t_msg> static inline void send(const t_msg &p_msg) {
//  internal::messenger_t<t_msg>::send(p_msg);
//}

///// \brief Adds a handler to a queue
///// It is possible to add many handlers to the same queue. Those handlers
/// will
///// compete with each other to handle a added message.
/////
///// \tparam t_msg is the type of message to be added to the queue
/////
///// \param p_id is the identifier of the queue to which this handler will be
///// added to
/////
///// \param p_handler is the function that will handle a message added to the
///// queue
// template <typename t_msg>
// static inline void add_handler(const number::id &p_handlers,
//                               std::function<void(const t_msg &)> p_handler)
//                               {
//  internal::messenger_t<t_msg>::add_handler(p_handlers, p_handler);
//}

///// \brief Adds a handler to a queue
/////
///// \tparam t_msg is the type of message to be added to the queue
/////
///// \tparam t_time is the type of time used to define the timeout
/////
///// \param p_handler is the function that will handle a message added to the
///// queue
/////
///// \return An auto generated \p id of the queue
/////
///// \details In this configuration, this queue has the lowest priority,
/// i.e.,
///// this queue will be the last to receive a copy of the message; and if any
///// handler times out, the message will be added again to the queue
// template <typename t_msg, typename t_time = std::chrono::minutes>
// static inline number::id
// add_handler(std::function<void(std::shared_ptr<bool>, const t_msg &)>
// p_handler,
//            t_time p_timeout = 10min) {
//  return internal::messenger_t<t_msg>::add_handler(p_handler, p_timeout);
//}

// template <typename t_msg, typename t_priority>
// static inline void traverse(std::function<void(const number::id &,
// t_priority,
//                                               std::chrono::milliseconds)>
//                                p_visitor) {
//  internal::messenger_t<t_msg>::traverse(p_visitor);
//}

} // namespace async
} // namespace tenacitas

#endif

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

typedef internal::priority priority;

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
  virtual bool core() override {
    if (!internal::loop_t<t_params...>::core()) {
      return false;
    }
    if (!internal::sleeping_loop_control(this->m_stopped, this->m_log,
                                         this->m_cond_stop, m_interval,
                                         this->m_owner, this->m_id)) {
      return false;
    }
    return true;
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
  virtual bool core() override {
    if (!internal::loop_t<void>::core()) {
      return false;
    }
    if (!internal::sleeping_loop_control(this->m_stopped, this->m_log,
                                         this->m_cond_stop, m_interval,
                                         this->m_owner, this->m_id)) {
      return false;
    }
    return true;
  }

private:
  internal::interval m_interval;
  //  std::mutex m_mutex_stop;
};

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
template <typename t_msg, typename t_time>
static inline number::id add_handlers(const priority &p_priority,
                                      t_time p_timeout) {
  return internal::messenger_t<t_msg>::add_handlers(p_priority, p_timeout);
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
/// this queue will be the last to receive a copy of the message.
template <typename t_msg, typename t_time>
static number::id add_handlers(t_time p_timeout) {
  return internal::messenger_t<t_msg>::add_handlers(p_timeout);
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
  internal::messenger_t<t_msg>::set_priority(p_handlers, p_priority);
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
  return internal::messenger_t<t_msg>::get_priority(p_handlers);
}

/// \brief Sends a message to all the queues associated to a message type
///
/// \tparam t_msg is the type of message to be added to the queue
///
/// \param p_msg is the message to be copied to all the queues
template <typename t_msg> static inline void send(const t_msg &p_msg) {
  internal::messenger_t<t_msg>::send(p_msg);
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
  internal::messenger_t<t_msg>::add_handler(p_handlers, p_handler);
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
/// \return An auto generated \p id of the queue
///
/// \details In this configuration, this queue has the lowest priority, i.e.,
/// this queue will be the last to receive a copy of the message; and if any
/// handler times out, the message will be added again to the queue
template <typename t_msg, typename t_time = std::chrono::minutes>
static inline number::id
add_handler(std::function<void(std::shared_ptr<bool>, const t_msg &)> p_handler,
            t_time p_timeout = 10min) {
  return internal::messenger_t<t_msg>::add_handler(p_handler, p_timeout);
}

template <typename t_msg, typename t_priority>
static inline void traverse(std::function<void(const number::id &, t_priority,
                                               std::chrono::milliseconds)>
                                p_visitor) {
  internal::messenger_t<t_msg>::traverse(p_visitor);
}

} // namespace async
} // namespace tenacitas

#endif

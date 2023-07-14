#ifndef TENACITAS_LIB_ASYNC_ALG_DISPATCHER_H
#define TENACITAS_LIB_ASYNC_ALG_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example async/dispatcher_000/main.cpp
/// \example async/dispatcher_001/main.cpp
/// \example async/dispatcher_002/main.cpp
/// \example async/temperature_sensors_simulator/uix/qt/main.cpp

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <thread>
#include <typeindex>
#include <utility>
#include <vector>

#include <tenacitas.lib/src/async/cpt/event.h>
#include <tenacitas.lib/src/async/internal/typ/event_queue.h>
#include <tenacitas.lib/src/async/typ/priority.h>
#include <tenacitas.lib/src/async/typ/queue_id.h>
#include <tenacitas.lib/src/async/typ/subscriber.h>
#include <tenacitas.lib/src/number/typ/id.h>
#include <tenacitas.lib/src/traits/alg/tuple.h>

#ifdef TENACITAS_LOG
#include <tenacitas.lib/src/log/alg/logger.h>
#endif

using namespace std::chrono_literals;

/// \brief Classes to help asynchronous programming
namespace tenacitas::lib::async::alg {

/** \brief Class that allows creation of queues for an event, creation of
subscribers for an event, and publising of events


The UML below, written in www.plantuml.com, shows the conceptual structure of
the \p dispatcher and other associated structures

@startuml
hide empty members
allow_mixing
skinparam linetype ortho

class dispatcher
class queue
class subscriber
class event

dispatcher -[#green]->  event
dispatcher -[#blue]->  event
queue *-- "*" event
subscriber .right.>  "handles" event
queue *-- priority
dispatcher *-- "*" queue

note "<color green>publishes\n<color blue>subscribes" as legend
@enduml

The central concept is an \p event, a struct that contains data about an
interesting event in the application. It can be, for example, a incoming message
from a network connection, a user menu choice or a temperature coming from a
sensor.

Every \p event class must define a <tt>operator <<(std::ostream&)</tt>.

A \p tenacitas::lib::async::typ::subscriber_t is a function class that will
handle a (shared) pointer to a \p t_event object.

A \p queue is where \p event objects will be queued for subsciber object to
access them. For example, a user menu choice can generate a log message, a
message sent to a remote program, and to change the color of widget in the user
interface.

Besides, it is possible to define, for example, one single subscriber for the
logging queue, five subscribers for the network message queue, and
two for the widget color changing. When the user makes its menu choice, the five
\p typ::subscriber_t objects in the message network queue will "fight
each other" to get the event to handle; the same for the two \p
typ::subscriber_t in the color changing queue.

It is possible to define a priority between the queues, so that an event
is delivered to the highest priority queue before.

The \p dispatcher class is responsible for managing the creation of
queue, adding \p typ::subscriber_t to the queues, and publish event
objects to the queues.

Please, look at the \p Examples section for examples on how to use these
functions and classes.
*/

template <cpt::event... t_events> struct dispatcher {
  /// \brief alias for the pointer to a \p dispatcher
  using ptr = std::shared_ptr<dispatcher>;

  /// \brief \p dispatcher objects can only be created in the heap memory area
  static ptr create() { return ptr(new dispatcher()); }

  dispatcher(const dispatcher &) = delete;
  dispatcher(dispatcher &&) = delete;
  dispatcher &operator=(const dispatcher &) = delete;
  dispatcher &operator=(dispatcher &&) = delete;

  /// \brief Destructor, when all the queues are stopped
  ~dispatcher();

  /// \brief Stops all the queues are stopped
  void stop() {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("stop");
#endif

    //    auto stop_each = [&]<std::size_t t_idx>(std::size_t /*p_idx*/) {
    //      for (auto &_event_queues : std::get<t_idx>(m_events_queues)) {
    //        for (auto &_event_queue : _event_queues) {
    //          _event_queue.halt();
    //        }
    //      }
    //    };

    //    auto stop_all = [&]<std::size_t...
    //    t_idxs>(std::index_sequence<t_idxs...>) {
    //      (stop_each<t_idxs>(), ...);
    //    };

    stop_all(std::make_index_sequence<sizeof...(t_events)>{});
  }

  /// \brief Adds a queue to the dispatcher, which will handle an event
  /// in a specific way
  ///
  /// \param p_priority is the priority of this queue
  ///
  /// \return a \p typ::queue_id, identifying the queue
  template <cpt::event t_event>
  typ::queue_id add_queue(typ::priority p_priority = typ::priority ::medium);

  /// \brief Adds a subscriber to a new queue
  template <cpt::event t_event>
  typ::queue_id subscribe(typ::subscriber_t<t_event> p_subscriber,
                          typ::priority p_priority = typ::priority ::medium);

  /// \brief Adds a subscriber to an existing queue
  ///
  /// \tparam t_event is the type of event that will be handled by the \p
  /// p_subscriber in the existing queue
  ///
  /// \param p_queue_id is the identifier of the queue
  ///
  /// \param p_subscriber is the subscriber to be added
  template <cpt::event t_event>
  void subscribe(const typ::queue_id &p_queue_id,
                 typ::subscriber_t<t_event> p_subscriber);

  /// \brief Adds a bunch of subscribers to an existing queue
  ///
  /// \tparam t_event is the type of event that will be handled by the \p
  /// p_subscriber in the existing queue
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \param p_num_workers defines the number of subscribers to be added
  ///
  /// \param p_factory is a function that creates subscribers
  template <cpt::event t_event>
  void subscribe(const typ::queue_id &p_queue_id, uint16_t p_num_workers,
                 std::function<async::typ::subscriber_t<t_event>()> p_factory);

  /// \brief Removes all queues of all events
  void clear();

  /// \brief Publishs an event to be handled
  ///
  /// This event pointer will be copied to all the queues, and one of the
  /// subscribers in each queue will handle it
  ///
  /// \tparam t_event is the type of event that is published in the queue
  ///
  /// \param p_event is the event to be handled
  template <cpt::event t_event> bool publish(const t_event &p_event);

  /// \brief Publishs an event to be handled to a specific queue
  ///
  /// \tparam t_event is the type of event that is published in the queue
  ///
  /// \param p_queue_id is the queue to which the event will be
  /// published
  ///
  /// \param p_event is the event to be handled
  template <cpt::event t_event>
  void publish_to_queue(typ::queue_id p_queue_id, const t_event &p_event);

  /// \brief Publishs an event to be handled
  ///
  /// \tparam t_event is the type of event that is published in the queue
  ///
  /// \tparam t_params are the types of the parameters for the \p t_event
  /// constructor
  ///
  /// \param p_params are the arguments to the \p t_event constructor
  template <cpt::event t_event, typename... t_params>
  bool publish(t_params &&...p_params);

  /// \brief Sets the priority for a queue
  ///
  /// \tparam t_event is the type of event that is published in the queue
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \param p_priority is the priority to be set for the queue
  template <cpt::event t_event>
  void set_priority(const typ::queue_id &p_id, typ::priority p_priority);

  /// \brief Retrieves the priority for a queue, if found
  ///
  /// \tparam t_event is the type of event that is published in the queue
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \return the priority of the queue or throws if p_queue_id does not exist
  template <cpt::event t_event>
  typ::priority get_priority(const typ::queue_id &p_queue_id);

  /// \brief Retrieves the size of the a queue of events
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \return the size of the event queue or 0 if no queue was found
  template <cpt::event t_event> size_t size(const typ::queue_id &p_id);

  /// \brief Retrieves how many positions in the queue of events for a
  /// queue are occupied
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \return the number of occupied positions or 0 if no queue was found
  template <cpt::event t_event> size_t occupied(const typ::queue_id &p_id);

  // \brief Waits for all the events in all the groups of  subscribers to be
  // handled
  //
  // TODO test it
  //  void wait() {
  //    for (const auto &_value_type : m_events_queues) {
  //      for (const queue_ptr &_queue_ptr : _value_type.second) {
  //                    _queue_ptr->empty_queue();
  //      }
  //    }
  //        TNCT_LOG_TRA("dispatcher ", get_id(), " - finished waiting");
  //  }

private:
  dispatcher() = default;

private:
  using events = std::tuple<t_events...>;

  using events_queues =
      std::tuple<std::list<internal::typ::event_queue<t_events>>...>;

  template <typename t_event>
  using event_queue = internal::typ::event_queue<t_event>;

  //  template <typename t_event>
  //  using event_queue_ptr = std::shared_ptr<event_queue<t_event>>;

  template <typename t_event>
  using event_queues = std::list<event_queue<t_event>>;

  template <typename t_event>
  using event_queues_iterator = typename event_queues<t_event>::iterator;

private:
  template <cpt::event t_event> event_queues<t_event> &get_event_queues();

  template <cpt::event t_event>
  event_queues_iterator<t_event>
  get_event_queues_iterator(typ::queue_id p_queue_id);

  template <cpt::event t_event> bool internal_publish(const t_event &p_event);

  template <cpt::event t_event>
  event_queues_iterator<t_event> internal_add_queue(typ::priority p_priority);

  template <std::size_t t_idx> void stop_each() {
    using event = std::tuple_element_t<t_idx, events>;
    event_queues<event> &_event_queues{std::get<t_idx>(m_events_queues)};

#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("##### event '", typeid(event).name(), " - stopping queues");
#endif

    for (event_queue<event> &_event_queue : _event_queues) {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("##### event '", typeid(event).name(), " - stopping queue ",
                   _event_queue.get_id());
#endif
      _event_queue.stop();
    }
  }

  template <std::size_t... t_idxs>
  void stop_all(std::index_sequence<t_idxs...>) {
    (stop_each<t_idxs>(), ...);
  }

  template <std::size_t t_idx> void clear_each() {
    using event = std::tuple_element_t<t_idx, events>;
    event_queues<event> &_event_queues(std::get<t_idx>(m_events_queues));

    for (event_queue<event> &_event_queue : _event_queues) {
      _event_queue.clear();
    }
  }
  template <std::size_t... t_idxs>
  void clear_all(std::index_sequence<t_idxs...>) {
    (clear_each<t_idxs>(), ...);
  }

private:
  events_queues m_events_queues;

  std::mutex m_mutex;
};

// IMPLEMENTATION ###########################################################

template <cpt::event... t_events>
template <cpt::event t_event>
typ::queue_id dispatcher<t_events...>::add_queue(typ::priority p_priority) {

  std::lock_guard<std::mutex> _lock(m_mutex);

  event_queues<t_event> &_event_queues{get_event_queues<t_event>()};

  event_queue<t_event> _event_queue{p_priority};

  typ::queue_id _queue_id{_event_queue.get_id()};

  _event_queues.push_back(std::move(_event_queue));

#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("##### event '", typeid(t_event).name(), " - now has ",
               _event_queues.size(), " queues, the last is # ", _queue_id);
#endif

  return _queue_id;
}

template <cpt::event... t_events>
template <cpt::event t_event>
void dispatcher<t_events...>::subscribe(
    const typ::queue_id &p_queue_id, typ::subscriber_t<t_event> p_subscriber) {

  event_queues_iterator<t_event> _ite{
      get_event_queues_iterator<t_event>(p_queue_id)};

  _ite->add_subscriber(p_subscriber);
}

template <cpt::event... t_events>
template <cpt::event t_event>
void dispatcher<t_events...>::subscribe(
    const typ::queue_id &p_queue_id, uint16_t p_num_workers,
    std::function<async::typ::subscriber_t<t_event>()> p_factory) {

  event_queues_iterator<t_event> _ite{
      get_event_queues_iterator<t_event>(p_queue_id)};

  _ite->add_subscriber(p_num_workers, p_factory);
}

template <cpt::event... t_events> inline void dispatcher<t_events...>::clear() {
  std::lock_guard<std::mutex> _lock(m_mutex);

  clear_all(std::make_index_sequence<sizeof...(t_events)>{});
}

template <cpt::event... t_events>
inline dispatcher<t_events...>::~dispatcher() {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("destructor");
#endif
  stop();
}

template <cpt::event... t_events>
template <cpt::event t_event>
typ::queue_id
dispatcher<t_events...>::subscribe(typ::subscriber_t<t_event> p_subscriber,
                                   typ::priority p_priority) {
  event_queues_iterator<t_event> _ite{internal_add_queue<t_event>(p_priority)};
  _ite->add_subscriber(p_subscriber);
  return _ite->get_id();
}

template <cpt::event... t_events>
template <cpt::event t_event>
bool dispatcher<t_events...>::publish(const t_event &p_event) {
  try {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", typeid(t_event).name());
#endif
    return internal_publish<t_event>(p_event);
  } catch (std::exception &_ex) {
    TNCT_LOG_ERR("error queue ", typeid(t_event).name(), ": '", _ex.what(),
                 '\'');
    return false;
  }
}

template <cpt::event... t_events>
template <cpt::event t_event>
void dispatcher<t_events...>::publish_to_queue(typ::queue_id p_queue_id,
                                               const t_event &p_event) {

  event_queues_iterator<t_event> _ite{
      get_event_queues_iterator<t_event>(p_queue_id)};

  _ite->add_event(p_event);
}

template <cpt::event... t_events>
template <cpt::event t_event, typename... t_params>
bool dispatcher<t_events...>::publish(t_params &&...p_params) {
  try {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("creating event to publish");
#endif
    t_event _event{std::forward<t_params>(p_params)...};
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("event to publish: ", typeid(t_event).name());
#endif
    return internal_publish<t_event>(_event);
  } catch (std::exception &_ex) {
    TNCT_LOG_ERR("error queue event '", _ex.what(), '\'');
    return false;
  }
}

template <cpt::event... t_events>
template <cpt::event t_event>
void dispatcher<t_events...>::set_priority(const typ::queue_id &p_queue_id,
                                           typ::priority p_priority) {
  event_queues_iterator<t_event> _ite{
      get_event_queues_iterator<t_event>(p_queue_id)};

  _ite->set_priority(p_priority);
}

template <cpt::event... t_events>
template <cpt::event t_event>
typ::priority
dispatcher<t_events...>::get_priority(const typ::queue_id &p_queue_id) {
  event_queues_iterator<t_event> _ite{
      get_event_queues_iterator<t_event>(p_queue_id)};
  return _ite->get_priority();
}

template <cpt::event... t_events>
template <cpt::event t_event>
size_t dispatcher<t_events...>::size(const typ::queue_id &p_queue_id) {

  event_queues_iterator<t_event> _ite{
      get_event_queues_iterator<t_event>(p_queue_id)};
  return _ite->get_size();
}

template <cpt::event... t_events>
template <cpt::event t_event>
size_t dispatcher<t_events...>::occupied(const typ::queue_id &p_queue_id) {
  event_queues_iterator<t_event> _ite{
      get_event_queues_iterator<t_event>(p_queue_id)};
  return _ite->get_occupied();
}

template <cpt::event... t_events>
template <cpt::event t_event>
bool dispatcher<t_events...>::internal_publish(const t_event &p_event) {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("internal queue ", typeid(t_event).name());
#endif

  event_queues<t_event> &_event_queues{get_event_queues<t_event>()};
  for (event_queue<t_event> &_event_queue : _event_queues) {
    _event_queue.add_event(p_event);
  }
  return true;
}

template <cpt::event... t_events>
template <cpt::event t_event>
dispatcher<t_events...>::event_queues_iterator<t_event>
dispatcher<t_events...>::internal_add_queue(typ::priority p_priority) {

  std::lock_guard<std::mutex> _lock(m_mutex);

  event_queues<t_event> &_event_queues{get_event_queues<t_event>()};

  event_queue<t_event> _event_queue{p_priority};

  typ::queue_id _queue_id{_event_queue.get_id()};

  //  std::pair<event_queues_iterator<t_event>, bool> _pair{
  //      _event_queues.insert(std::move(_event_queue))};

  _event_queues.push_back(std::move(_event_queue));

#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("##### event '", typeid(t_event).name(), " - now has ",
               _event_queues.size(), " queues, the last is # ", _queue_id);
#endif

  return std::prev(_event_queues.end());
}

template <cpt::event... t_events>
template <cpt::event t_event>
dispatcher<t_events...>::event_queues_iterator<t_event>
dispatcher<t_events...>::get_event_queues_iterator(typ::queue_id p_queue_id) {
  event_queues<t_event> &_event_queues{get_event_queues<t_event>()};

  event_queues_iterator<t_event> _ite{std::find_if(
      _event_queues.begin(), _event_queues.end(),
      [&](const auto &p_queue) { return p_queue.get_id() == p_queue_id; })};

  if (_ite == _event_queues.end()) {
    std::stringstream _stream;
    _stream << "event '" << typeid(t_event).name()
            << "' - no queue found for queue id " << p_queue_id;
    const std::string _str{_stream.str()};
    TNCT_LOG_FAT(_str);
    throw std::runtime_error(_str);
  }
  return _ite;
}

template <cpt::event... t_events>
template <cpt::event t_event>
dispatcher<t_events...>::event_queues<t_event> &
dispatcher<t_events...>::get_event_queues() {
  constexpr std::size_t _event_index{traits::alg::index_v<t_event, events>};
  return std::get<_event_index>(m_events_queues);
}

} // namespace tenacitas::lib::async::alg

#endif

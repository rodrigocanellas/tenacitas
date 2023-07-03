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
#include <thread>
#include <typeindex>
#include <utility>
#include <vector>

#include <tenacitas.lib/src/async/cpt/concepts.h>
#include <tenacitas.lib/src/async/internal/typ/queue.h>
#include <tenacitas.lib/src/async/typ/priority.h>
#include <tenacitas.lib/src/async/typ/queue_id.h>
#include <tenacitas.lib/src/async/typ/subscriber.h>
#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/number/typ/id.h>

using namespace std::chrono_literals;

/// \brief Classes to help asynchronous programming
namespace tenacitas::lib::src::async::alg {

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

struct dispatcher {
  /// \brief alias for the pointer to a \p dispatcher
  using ptr = std::shared_ptr<dispatcher>;

  /// \brief \p dispatcher objects can only be created in the heap memory area
  static ptr create() { return ptr(new dispatcher()); }

  dispatcher(const dispatcher &) = delete;
  dispatcher(dispatcher &&) = delete;
  dispatcher &operator=(const dispatcher &) = delete;
  dispatcher &operator=(dispatcher &&) = delete;

  /// \brief Destructor, when all the queues are stopped
  virtual ~dispatcher();

  /// \brief Stops all the queues are stopped
  void stop() {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("stop");
#endif
    for (auto &_value : m_events) {
      for (internal::typ::queue::ptr &_queue : _value.second) {
        _queue->halt();
      }
    }
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
  bool publish_to_queue(typ::queue_id p_queue_id, const t_event &p_event);

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
  /// \return the priority of the queue, if \p p_id exists
  template <cpt::event t_event>
  std::optional<typ::priority> get_priority(const typ::queue_id &p_id);

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

protected:
  dispatcher() = default;

private:
  // List of queues
  using queues = std::list<internal::typ::queue::ptr>;

  // Event runtime id, and the list of queues associated to the event
  using events = std::map<std::type_index, queues>;
  // std::type_index(typeid(int))

private:
  template <cpt::event t_event>
  internal::typ::queue_t<t_event> *convert(queues::iterator p_ite);

  template <cpt::event t_event> bool internal_publish(const t_event &p_event);

  queues::iterator find(queues &p_queues, const typ::queue_id &p_id);

  queues::const_iterator find(const queues &p_queues,
                              const typ::queue_id &p_id) const;

  // Sorts the list of queues in descending priority order
  void sort(queues &p_queues);

  template <cpt::event t_event> inline constexpr queues &get_queues();

private:
  // Events id and the associated queues
  events m_events;

  // Access control
  std::mutex m_mutex;
};

// IMPLEMENTATION ###########################################################

// void dispatcher::stop() {
//#ifdef TENACITAS_LOG
//   TNCT_LOG_TRA("stop");
//#endif
//   for (auto &_value : m_events) {
//     for (internal::typ::queue::ptr &_queue : _value.second) {
//       _queue->stop();
//     }
//   }
// }

template <cpt::event t_event>
typ::queue_id dispatcher::add_queue(typ::priority p_priority) {

  std::lock_guard<std::mutex> _lock(m_mutex);

  queues &_queues = get_queues<t_event>();

  _queues.push_back(internal::typ::queue::create<t_event>(p_priority));

  typ::queue_id _queue_id{_queues.back()->get_id()};

#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("##### event '", typeid(t_event).name(), "' - adding queue ",
               _queue_id);
#endif

  sort(_queues);

  return _queue_id;
}

template <cpt::event t_event>
void dispatcher::subscribe(const typ::queue_id &p_queue_id,
                           typ::subscriber_t<t_event> p_subscriber) {
  queues &_queues = get_queues<t_event>();

  auto _queue_ite = find(_queues, p_queue_id);
  if (_queue_ite != _queues.end()) {

    if ((*_queue_ite)->get_id() != p_queue_id) {
      std::stringstream _stream;
      _stream << "event '" << typeid(t_event).name() << "' - queue id should be"
              << p_queue_id << ", but it is " << (*_queue_ite)->get_id();
      const std::string _str{_stream.str()};
      TNCT_LOG_FAT(_str);
      throw std::runtime_error(_str);
    }

    convert<t_event>(_queue_ite)->add_subscriber(std::move(p_subscriber));
  }
}

template <cpt::event t_event>
void dispatcher::subscribe(
    const typ::queue_id &p_queue_id, uint16_t p_num_workers,
    std::function<async::typ::subscriber_t<t_event>()> p_factory) {
  queues &_queues = get_queues<t_event>();

  auto _queue_ite = find(_queues, p_queue_id);
  if (_queue_ite != _queues.end()) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("event '", typeid(t_event).name(), "' - adding ",
                 p_num_workers, " subscribers to queue ", p_queue_id);
#endif
    convert<t_event>(_queue_ite)->add_subscriber(p_num_workers, p_factory);
  }
}

inline void dispatcher::clear() {
  std::lock_guard<std::mutex> _lock(m_mutex);
  m_events.clear();
}

inline dispatcher::~dispatcher() {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("destructor");
#endif
  stop();
}

template <cpt::event t_event>
typ::queue_id dispatcher::subscribe(typ::subscriber_t<t_event> p_subscriber,
                                    typ::priority p_priority) {
  typ::queue_id _id = add_queue<t_event>(p_priority);
  subscribe(_id, p_subscriber);
  return _id;
}

template <cpt::event t_event> bool dispatcher::publish(const t_event &p_event) {
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

template <cpt::event t_event>
bool dispatcher::publish_to_queue(typ::queue_id p_queue_id,
                                  const t_event &p_event) {
  try {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("queue ", typeid(t_event).name(), ", in queue ", p_queue_id);
#endif

    queues &_queues = get_queues<t_event>();

    for (queues::iterator _ite = _queues.begin(); _ite != _queues.end();
         ++_ite) {

      if ((*_ite)->get_id() == p_queue_id) {
#ifdef TENACITAS_LOG
        TNCT_LOG_TRA("event '", typeid(t_event).name(),
                     "' - sending event to queue ", (*_ite)->get_id());
#endif
        convert<t_event>(_ite)->add_event(p_event);
      }
    }

  } catch (std::exception &_ex) {
    TNCT_LOG_ERR("error queue ", typeid(t_event).name(), ", in queue ",
                 p_queue_id, ": '", _ex.what(), '\'');
    return false;
  }
  return true;
}

template <cpt::event t_event, typename... t_params>
bool dispatcher::publish(t_params &&...p_params) {
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

template <cpt::event t_event>
void dispatcher::set_priority(const typ::queue_id &p_id,
                              typ::priority p_priority) {
  queues &_queues = get_queues<t_event>();
  auto _queues_ite = find(_queues, p_id);
  if (_queues_ite != _queues.end()) {
    (*_queues_ite)->set_priority(p_priority);
  }
}

template <cpt::event t_event>
std::optional<typ::priority>
dispatcher::get_priority(const typ::queue_id &p_id) {
  const queues &_queues = get_queues<t_event>();
  auto _queues_ite = find(_queues, p_id);
  if (_queues_ite != _queues.end()) {
    return {(*_queues_ite)->get_priority()};
  }
  return {};
}

template <cpt::event t_event>
size_t dispatcher::size(const typ::queue_id &p_id) {

  const queues &_queues = get_queues<t_event>();
  auto _queues_ite = find(_queues, p_id);
  if (_queues_ite != _queues.end()) {
    return (*_queues_ite)->get_size();
  }
  return 0;
}

template <cpt::event t_event>
size_t dispatcher::occupied(const typ::queue_id &p_id) {
  const queues &_queues = get_queues<t_event>();
  auto _queues_ite = find(_queues, p_id);
  if (_queues_ite != _queues.end()) {
    return (*_queues_ite)->get_occupied();
  }

  return 0;
}

template <cpt::event t_event>
internal::typ::queue_t<t_event> *dispatcher::convert(queues::iterator p_ite) {
  internal::typ::queue::ptr _queue_ptr{*p_ite};

  internal::typ::queue *_queue_raw_ptr{_queue_ptr.get()};

  auto _queue_event_raw_ptr{
      reinterpret_cast<internal::typ::queue_t<t_event> *>(_queue_raw_ptr)};
  return _queue_event_raw_ptr;
}

template <cpt::event t_event>
bool dispatcher::internal_publish(const t_event &p_event) {
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("internal queue ", typeid(t_event).name());
#endif

  queues &_queues = get_queues<t_event>();
#ifdef TENACITAS_LOG
  TNCT_LOG_TRA("got queues for ", typeid(t_event).name());
#endif

  for (queues::iterator _ite = _queues.begin(); _ite != _queues.end(); ++_ite) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("event '", typeid(t_event).name(),
                 "' - sending event to queue ", (*_ite)->get_id());
#endif
    convert<t_event>(_ite)->add_event(p_event);
  }
  return true;
}

inline dispatcher::queues::iterator
dispatcher::find(queues &p_queues, const typ::queue_id &p_id) {
  auto _cmp = [&p_id](const internal::typ::queue::ptr &p_queue) -> bool {
    return p_id == p_queue->get_id();
  };
  return std::find_if(p_queues.begin(), p_queues.end(), _cmp);
}

inline dispatcher::queues::const_iterator
dispatcher::find(const queues &p_queues, const typ::queue_id &p_id) const {
  auto _cmp = [&p_id](const internal::typ::queue::ptr &p_queue) -> bool {
    return p_id == p_queue->get_id();
  };
  return std::find_if(p_queues.begin(), p_queues.end(), _cmp);
}

inline void dispatcher::sort(queues &p_queues) {
  p_queues.sort([](const internal::typ::queue::ptr &p_i1,
                   const internal::typ::queue::ptr &p_i2) -> bool {
    return (p_i1->get_priority() > p_i2->get_priority());
  });
}

template <cpt::event t_event>
inline constexpr dispatcher::queues &dispatcher::get_queues() {
  //  return m_events[t_event::id];
  return m_events[std::type_index(typeid(t_event))];
}

} // namespace tenacitas::lib::src::async::alg

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASY_DISPATCHER_H
#define TENACITAS_LIB_ASY_DISPATCHER_H

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iterator>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std::chrono_literals;
#ifdef TENACITAS_LOG
#include <tenacitas.lib/log/log.h>
#endif

#include <tenacitas.lib/asy/event_handler.h>
#include <tenacitas.lib/asy/event_priority.h>
#include <tenacitas.lib/asy/internal/event_queue.h>
#include <tenacitas.lib/asy/queue_id.h>
#include <tenacitas.lib/sta/event.h>
#include <tenacitas.lib/sta/publisher.h>
#include <tenacitas.lib/sta/subscriber.h>
#include <tenacitas.lib/sta/tuple_index.h>

namespace tenacitas::lib::asy {

/** \brief Class that allows creation of queues for an event, creation of
subscribers for an event, and publising of events


The UML below, written in www.plantuml.com, shows the conceptual structure of
the \p dispatcher and other associated structures

@startuml
hide empty members
allow_mixing
skinparam linetype ortho


class dispatcher<t_event...>
class queue<t_event>
class subscriber
class event

dispatcher -[#green]->  event
dispatcher -[#blue]->  event
queue *-- "*" event
subscriber .right.>  "handles" event
queue *-- event_priority
dispatcher *-- "*" queue

note "<color green>publishes\n<color blue>subscribes" as legend
@enduml

The central concept is an \p event, a struct that contains data about an
interesting event in the application. It can be, for example, a incoming message
from a network connection, a user menu choice or a temperature coming from a
sensor.

A \p tenacitas::lib::subscriber is a function object that will
handle a \p t_event object.

A \p queue is where \p event objects will be queued for subscibers objects to
access them. For example, a user menu choice can generate a log message, a
message sent to a remote program, and to change the color of widget in the user
interface.

Besides, it is possible to define, for example, one single subscriber for the
logging queue, five subscribers for the network message queue, and
two for the widget color changing. When the user makes its menu choice, the five
\p tenacitas::lib::subscriber objects in the message network queue
will "fight each other" to get the event to handle; the same for the two \p
tenacitas::lib::subscriber in the color changing queue.

It is possible to define a event_priority between the queues, so that an
event is delivered to the highest event_priority queue before the others.

The \p dispatcher class is responsible for managing the creation of
queue, adding \p tenacitas::lib::subscriber to the queues, and
publish event objects to the queues.

Please, look at the \p Examples section for examples on how to use these
functions and classes.
*/
template <sta::event... t_events> struct dispatcher {

  using ptr = std::shared_ptr<dispatcher>;

  using events = std::tuple<t_events...>;

  static ptr create() { return ptr(new dispatcher()); }

  dispatcher(const dispatcher &) = delete;
  dispatcher(dispatcher &&) = delete;
  dispatcher &operator=(const dispatcher &) = delete;
  dispatcher &operator=(dispatcher &&) = delete;

  ~dispatcher() {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("destructor");
#endif
    // stop();
  }

  template <sta::event t_event>
  queue_id add_queue(event_priority p_priority = event_priority ::medium) {

    constexpr size_t _event_index{
        assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    event_queue_list_iterator<t_event> _ite{
        add_event_queue<t_event>(_event_queue_list, p_priority)};

    queue_id _queue_id{_ite->get_id()};

    sort_event_queue_list(_event_queue_list);

    return _queue_id;
  }

  template <sta::subscriber t_subscriber, sta::event t_event>
  queue_id subscribe(event_handler<t_event> p_subscriber,
                     event_priority p_priority = event_priority ::medium) {

    assert_event_defined_in_subscriber<t_subscriber, t_event>();

    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    event_queue_list_iterator<t_event> _ite{
        add_event_queue<t_event>(_event_queue_list, p_priority)};

    queue_id _queue_id{_ite->get_id()};

    _ite->add_subscriber(p_subscriber);

    sort_event_queue_list<t_event>(_event_queue_list);

    return _queue_id;
  }

  template <sta::subscriber t_subscriber, sta::event t_event>
  void subscribe(const queue_id &p_queue_id,
                 event_handler<t_event> p_subscriber) {
    assert_event_defined_in_subscriber<t_subscriber, t_event>();

    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_index, t_event>(p_queue_id)};

    _ite->add_subscriber(p_subscriber);
  }

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
  template <sta::subscriber t_subscriber, sta::event t_event>
  void subscribe(const queue_id &p_queue_id,
                 std::unsigned_integral auto p_num_workers,
                 std::function<event_handler<t_event>()> p_factory) {

    assert_event_defined_in_subscriber<t_subscriber, t_event>();

    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_index, t_event>(p_queue_id)};

    _ite->add_subscriber(p_num_workers, p_factory);
  }

  /// \brief Removes all queues of all events
  void clear() {
    //    typename event_queue_list_mutex::size_type _mutex_index{0};
    //    auto _function{[&](auto &p_event_queue_list) {
    //      for (auto &_event_queue : p_event_queue_list) {
    //        std::lock_guard<std::mutex> _lock(
    //            m_event_queue_list_mutex[_mutex_index++]);
    //        _event_queue.clear();
    //      }
    //    }};
    //    traverse(_function, m_event_queue_list_tuple);
    clear<0>();
  }

  template <sta::event t_event> void clear() {
    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    for (auto &_event_queue : _event_queue_list) {
      _event_queue.clear();
    }
  }

  template <sta::event t_event> void clear(queue_id p_queue_id) {
    constexpr auto _event_index{sta::tuple_index_v<t_event, events>};

    if (_event_index != -1) {
      return;
    }

    event_queue_list<t_event> &_event_queue_list{
        std::get<_event_index>(m_event_queue_list_tuple)};

    for (auto &_event_queue : _event_queue_list) {
      if (_event_queue.get_id() == p_queue_id) {
        _event_queue.clear();
        break;
      }
    }
  }

  template <sta::publisher t_publisher, sta::event t_event>
  bool publish(const t_event &p_event) {
    assert_event_defined_in_publisher<t_publisher, t_event>();

    constexpr auto _events_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    try {
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("queue ", typeid(t_event).name());
#endif
      return internal_publish<_events_queues_list_index, t_event>(p_event);
    } catch (std::exception &_ex) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error queue ", typeid(t_event).name(), ": '", _ex.what(),
                   '\'');
#endif
      return false;
    }
  }

  template <sta::publisher t_publisher, sta::event t_event>
  void publish_to_queue(queue_id p_queue_id, const t_event &p_event) {

    assert_event_defined_in_publisher<t_publisher, t_event>();

    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    _ite->add_event(p_event);
  }

  template <sta::publisher t_publisher, sta::event t_event,
            typename... t_params>
  bool publish(t_params &&...p_params) {
    assert_event_defined_in_publisher<t_publisher, t_event>();

    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    try {
      t_event _event{std::forward<t_params>(p_params)...};
#ifdef TENACITAS_LOG
      TNCT_LOG_TRA("event to publish: ", typeid(t_event).name());
#endif
      return internal_publish<_event_queues_list_index, t_event>(_event);
    } catch (std::exception &_ex) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error queue event '", _ex.what(), '\'');
#endif
      return false;
    }
  }

  template <sta::event t_event>
  void set_priority(const queue_id &p_queue_id, event_priority p_priority) {
    constexpr auto _event_index{assert_event_defined_in_dispatcher<t_event>()};

    std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[_event_index]);

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_index, t_event>(p_queue_id)};

    _ite->set_priority(p_priority);
  }

  template <sta::event t_event>
  event_priority get_priority(const queue_id &p_queue_id) {
    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    return _ite->get_event_priority();
  }

  /// \brief Retrieves the size of the a queue of events
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \return the size of the event queue or 0 if no queue was found
  template <sta::event t_event> size_t queue_size(const queue_id &p_queue_id) {

    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    return _ite->get_size();
  }

  /// \brief Retrieves how many positions in the queue of events for a
  /// queue are occupied
  ///
  /// \param p_id is the identifier of the queue
  ///
  /// \return the number of occupied positions or 0 if no queue was found
  template <sta::event t_event>
  size_t occupied_in_queue(const queue_id &p_queue_id) {
    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    event_queue_list_iterator<t_event> _ite{
        find_event_queue<_event_queues_list_index, t_event>(p_queue_id)};

    return _ite->get_occupied();
  }

  template <sta::event t_event> size_t amount_of_queues() const {
    constexpr auto _event_queues_list_index{
        assert_event_defined_in_dispatcher<t_event>()};

    const auto &_event_queues{
        std::get<_event_queues_list_index>(m_event_queue_list_tuple)};

    return _event_queues.size();
  }

  //  /// \brief Removes all events in all the queues of \p t_event
  //  template <sta::event t_event> void empty_queue();

  //  /// \brief Removes all events in all the queues of \p t_event
  //  template <sta::event t_event> void empty_queue(queue_id p_queue_id);

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
  template <typename t_event>
  using event_queue = internal::event_queue<t_event>;

  template <typename t_event>
  using event_queue_list = std::list<event_queue<t_event>>;

  using event_queue_list_tuple =
      std::tuple<std::list<internal::event_queue<t_events>>...>;

  template <typename t_event>
  using event_queue_list_iterator =
      typename event_queue_list<t_event>::iterator;

  template <sta::event t_event> struct event_mutex { std::mutex mutex; };

  using event_queue_list_mutex =
      std::array<std::mutex, std::tuple_size_v<events>>;

private:
  template <size_t t_idx = 0> void clear() {
    if constexpr (t_idx >= std::tuple_size_v<events>) {
      return;
    } else {
      std::lock_guard<std::mutex> _lock(m_event_queue_list_mutex[t_idx]);

      std::get<t_idx>(m_event_queue_list_tuple).clear();

      return clear<t_idx + 1>();
    }
  }

  /// \brief Stops all the queues
  // void stop() { stop_all(std::make_index_sequence<sizeof...(t_events)>{}); }

  template <size_t t_event_index, sta::event t_event>
  event_queue_list_iterator<t_event> find_event_queue(queue_id p_queue_id) {

    event_queue_list<t_event> &_event_queue_list{
        std::get<t_event_index>(m_event_queue_list_tuple)};

    return find_event_queue(_event_queue_list, p_queue_id);
  }

  template <size_t t_event_queues_index, typename t_event>
  bool internal_publish(const t_event &p_event) {
#ifdef TENACITAS_LOG
    TNCT_LOG_TRA("internal queue ", typeid(t_event).name());
#endif

    event_queue_list<t_event> &_event_queues{
        std::get<t_event_queues_index>(m_event_queue_list_tuple)};
    for (event_queue<t_event> &_event_queue : _event_queues) {
      _event_queue.add_event(p_event);
    }
    return true;
  }

  template <sta::event t_event>
  event_queue_list_iterator<t_event>
  add_event_queue(event_queue_list<t_event> &p_event_queue_list,
                  event_priority p_priority) {

    event_queue<t_event> _event_queue{p_priority};

    p_event_queue_list.push_back(std::move(_event_queue));

    return std::prev(p_event_queue_list.end());
  }

  template <sta::event t_event>
  static constexpr size_t assert_event_defined_in_dispatcher() {
    constexpr auto _index{sta::tuple_index_v<t_event, events>};
    static_assert(_index != -1,
                  "trying to subscribe to an event not defined in dispatcher");
    return static_cast<size_t>(_index);
  }

  template <sta::subscriber t_subscriber, sta::event t_event>
  static constexpr void assert_event_defined_in_subscriber() {
    using events_subscribed = typename t_subscriber::events_subscribed;

    static_assert(
        sta::tuple_index_v<t_event, events_subscribed> != -1,
        "trying to subscribe to an event not in 'events_subscribed' of the "
        "class that wants to subscribe for it");
  }

  template <sta::publisher t_publisher, sta::event t_event>
  constexpr void assert_event_defined_in_publisher() const {
    using events_published = typename t_publisher::events_published;

    static_assert(sta::tuple_index_v<t_event, events_published> != -1,
                  "trying to publish an event not in 'events_published' of the "
                  "class that wants to publish it");
  }

  template <sta::event t_event>
  void sort_event_queue_list(event_queue_list<t_event> &p_event_queue_list) {
    p_event_queue_list.sort(
        [](const event_queue<t_event> &p_event_queue_left,
           const event_queue<t_event> &p_event_queue_right) {
          return p_event_queue_left.get_event_priority() <
                 p_event_queue_right.get_event_priority();
        });
  }

  template <sta::event t_event>
  event_queue_list_iterator<t_event>
  find_event_queue(event_queue_list<t_event> &p_event_queue_list,
                   queue_id p_queue_id) {

    event_queue_list_iterator<t_event> _ite{std::find_if(
        p_event_queue_list.begin(), p_event_queue_list.end(),
        [&](const auto &p_queue) { return p_queue.get_id() == p_queue_id; })};

    if (_ite == p_event_queue_list.end()) {
      std::stringstream _stream;
      _stream << "event '" << typeid(t_event).name()
              << "' - no queue found for queue id " << p_queue_id;
      const std::string _str{_stream.str()};
#ifdef TENACITAS_LOG
      TNCT_LOG_FAT(_str);
#endif
      throw std::runtime_error(_str);
    }
    return _ite;
  }

private:
  event_queue_list_tuple m_event_queue_list_tuple;

  event_queue_list_mutex m_event_queue_list_mutex;
};

} // namespace tenacitas::lib::asy

#endif

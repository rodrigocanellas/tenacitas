/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_DISPATCHER_H
#define TENACITAS_LIB_ASYNC_DISPATCHER_H

#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <tuple>
#include <vector>

#include <tenacitas.lib/async/handling_id.h>
#include <tenacitas.lib/async/handling_priority.h>
#include <tenacitas.lib/async/internal/handler_id.h>
#include <tenacitas.lib/async/internal/handling.h>

#include <tenacitas.lib/format/fmt.h>
#include <tenacitas.lib/traits/event.h>
#include <tenacitas.lib/traits/handler.h>
#include <tenacitas.lib/traits/has_new_operator.h>
#include <tenacitas.lib/traits/is_tuple.h>
#include <tenacitas.lib/traits/publisher.h>
#include <tenacitas.lib/traits/queue.h>
#include <tenacitas.lib/traits/subscriber.h>
#include <tenacitas.lib/traits/tuple_find.h>
#include <tenacitas.lib/traits/tuple_like.h>
#include <tenacitas.lib/tuple/tuple_transform.h>
#include <tenacitas.lib/tuple/tuple_traverse.h>

namespace tenacitas::lib::async {

/** \brief Class that allows creation of queues for an event, creation of
subscribers for an event, and publising of events


The UML below, written in www.plantuml.com, shows the conceptual structure of
the \p dispatcher and other associated structures

@startuml
hide empty members
allow_mixing
skinparam linetype ortho


class dispatcher<t_events...>
class handling<t_event>
class queue<t_event>
class subscriber
class event

publisher -[#green]->  event
subscriber -[#blue]->  event
queue *-- "*" event
handling "1" *-- "*" subscriber
handling *-- handling_priority
dispatcher *-- "*" handling
handling *-- queue
subscriber o-- dispatcher
publisher o-- dispatcher

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

It is possible to define a handling_priority between the queues, so that an
event is delivered to the highest handling_priority queue before the others.

The \p dispatcher class is responsible for managing the creation of
queue, adding \p tenacitas::lib::subscriber to the queues, and
publish event objects to the queues.

Please, look at the \p Examples section for examples on how to use these
functions and classes.
*/

template <traits::logger t_logger, traits::event... t_events>
struct dispatcher {

public:
  using logger = t_logger;

  dispatcher() = delete;

  dispatcher(logger &p_logger) : m_logger(p_logger) {}

  dispatcher(const dispatcher &) = delete;
  dispatcher(dispatcher &&) = delete;

  ~dispatcher() { TNCT_LOG_TRA(m_logger, "dispactcher destructor"); }

  dispatcher &operator=(const dispatcher &) = delete;
  dispatcher &operator=(dispatcher &&) = delete;

  // template <traits::event t_event> void publish(const t_event &p_event) {
  //   constexpr size_t _idx{find_event_idx<t_event>()};

  //   TNCT_LOG_DEB(m_logger, format::fmt("idx = ", _idx));

  //   auto &_notifiers{get_notifiers<t_event>()};

  //   for (auto &_notifier : _notifiers) {
  //     _notifier(p_event);
  //   }
  // }

  template <traits::event t_event, traits::queue<t_logger, t_event> t_queue,
            traits::handler t_handler>
  [[nodiscard]] std::optional<handling_id>
  subscribe(t_handler &&p_handler,
            handling_priority p_handling_priority = handling_priority::medium) {

    if (is_handler_already_being_used<t_event, t_handler>()) {
      return std::nullopt;
    }

    using handling_concrete =
        internal::handling_concrete<t_logger, t_event, t_queue, t_handler>;

    try {
      std::lock_guard<std::mutex> _lock(m_mutex);

      handling_ptr<t_event> _handling_ptr{std::make_unique<handling_concrete>(
          ++m_handling_id, m_logger, std::move(p_handler))};

      get_handlings<t_event>().insert(
          {p_handling_priority, std::move(_handling_ptr)});

      return m_handling_id;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }

    return std::nullopt;
  }

  template <traits::event t_event, traits::queue<t_logger, t_event> t_queue,
            traits::handler t_handler>
  [[nodiscard]] std::optional<handling_id>
  subscribe(t_handler &&p_handler, handling_priority p_handling_priority,
            size_t p_num_handlers) {

    TNCT_LOG_TST(m_logger, format::fmt("p_handler = ",
                                       reinterpret_cast<void *>(&p_handler)));

    if (is_handler_already_being_used<t_event, t_handler>()) {
      return std::nullopt;
    }

    using handling_concrete =
        internal::handling_concrete<t_logger, t_event, t_queue, t_handler>;

    try {
      std::lock_guard<std::mutex> _lock(m_mutex);

      handling_ptr<t_event> _handling_ptr{std::make_unique<handling_concrete>(
          ++m_handling_id, m_logger, std::move(p_handler), p_num_handlers)};

      get_handlings<t_event>().insert(
          {p_handling_priority, std::move(_handling_ptr)});

      return m_handling_id;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }

    return std::nullopt;
  }

  template <traits::event t_event>
  [[nodiscard]] bool increment_handlers(handling_id p_handling_id,
                                        size_t p_amount) {
    try {
      if (find_handling<t_event>(p_handling_id,
                                 [&](handling<t_event> &p_handling) {
                                   p_handling.increment_handlers(p_amount);
                                 })) {
        return true;
      }
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }

  template <traits::event t_event>
  [[nodiscard]] std::optional<size_t>
  get_amount_handlers(handling_id p_handling_id) const {
    try {
      size_t _amount{0};
      if (find_handling<t_event>(p_handling_id,
                                 [&](const handling<t_event> &p_handling) {
                                   _amount = p_handling.get_amount_handlers();
                                 })) {
        return {_amount};
      }
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return std::nullopt;
  }

  template <traits::event t_event>
  [[nodiscard]] size_t get_amount_handlings() const {
    auto &_handlings{get_handlings<t_event>()};
    return static_cast<size_t>(_handlings.size());
  }

private:
  using events = std::tuple<t_events...>;

  template <traits::event t_event>
  using handling = internal::handling<t_logger, t_event>;

  template <traits::event t_event>
  using handling_ptr = std::unique_ptr<handling<t_event>>;

  template <traits::event t_event>
  using handling_const_ptr = std::unique_ptr<const handling<t_event>>;

  template <traits::event t_event>
  using handlings = std::multimap<handling_priority, handling_ptr<t_event>>;

  template <traits::event t_event>
  using handlings_ite = typename handlings<t_event>::iterator;

  template <traits::event t_event>
  using handlings_const_ite = typename handlings<t_event>::const_iterator;

  using events_handlings =
      typename tuple::tuple_transform<events, handlings>::type;

private:
  template <traits::event t_event, traits::handler t_handler>
  [[nodiscard]] bool is_handler_already_being_used() const {
    const size_t _handler_id{internal::handler_id<t_handler>()};

    TNCT_LOG_TST(m_logger, format::fmt("p_handler = ", _handler_id));

    const handlings<t_event> &_handlings{get_handlings<t_event>()};

    for (handlings_const_ite<t_event> _ite = _handlings.begin();
         _ite != _handlings.end(); ++_ite) {
      if (_ite->second->get_handler_id() == _handler_id) {
        TNCT_LOG_ERR(
            m_logger,
            format::fmt(
                "handler is already being used in a handling for event '",
                typeid(t_event).name(), '\''));

        return true;
      }
    }

    return false;
  }

  template <traits::event t_event>
  [[nodiscard]] handlings<t_event> &get_handlings() {
    constexpr auto _idx{traits::tuple_find<events, t_event>()};
    static_assert(_idx != -1, "event not found in the tuple of events");
    return std::get<static_cast<size_t>(_idx)>(m_events_handlings);
  }

  template <traits::event t_event>
  [[nodiscard]] const handlings<t_event> &get_handlings() const {
    constexpr auto _idx{traits::tuple_find<events, t_event>()};
    static_assert(_idx != -1, "event not found in the tuple of events");
    return std::get<static_cast<size_t>(_idx)>(m_events_handlings);
  }

  template <traits::event t_event>
  bool find_handling(handling_id p_handling_id,
                     std::function<void(handling<t_event> &)> p_exec) {
    handlings<t_event> &_handlings{get_handlings<t_event>()};

    auto _match{[&](const typename handlings<t_event>::value_type &p_value) {
      return p_value.second->get_id() == p_handling_id;
    }};

    auto _ite{std::find_if(_handlings.begin(), _handlings.end(), _match)};
    if (_ite != _handlings.end()) {
      p_exec(*(_ite->second));
      return true;
    }

    TNCT_LOG_WAR(m_logger,
                 format::fmt("Could not find handling id ", p_handling_id));
    return false;
  }

  template <traits::event t_event>
  bool
  find_handling(handling_id p_handling_id,
                std::function<void(const handling<t_event> &)> p_exec) const {
    const handlings<t_event> &_handlings{get_handlings<t_event>()};

    auto _match{[&](const typename handlings<t_event>::value_type &p_value) {
      return p_value.second->get_id() == p_handling_id;
    }};

    auto _ite{std::find_if(_handlings.cbegin(), _handlings.cend(), _match)};
    if (_ite != _handlings.cend()) {
      p_exec(*(_ite->second));
      return true;
    }

    TNCT_LOG_WAR(m_logger,
                 format::fmt("Could not find handling id ", p_handling_id));
    return false;
  }

private:
  logger &m_logger;

  events_handlings m_events_handlings;

  handling_id m_handling_id{0};
  std::mutex m_mutex;
};

// template <traits::logger t_logger, traits::handlings... t_handlings>
// struct dispatcher {
//   using logger = t_logger;

//   dispatcher(t_logger &p_logger)
//       : m_logger(p_logger), m_events_handlings(create_events_handlings())
//       {}

//   dispatcher() = delete;
//   dispatcher(const dispatcher &) = delete;
//   dispatcher(dispatcher &&) = delete;
//   dispatcher &operator=(const dispatcher &) = delete;
//   dispatcher &operator=(dispatcher &&) = delete;

//   void *operator new(size_t) = delete;
//   void *operator new[](size_t) = delete;

//   void operator delete(void *) = delete;
//   void operator delete[](void *) = delete;

//   ~dispatcher() { TNCT_LOG_TRA(m_logger, "destructor"); }

//   template <traits::subscriber t_subscriber, traits::event t_event>
//   [[nodiscard]] result
//   subscribe(const handling_id &p_handling_id, t_subscriber &p_subscriber,
//             handling_priority p_handling_priority, size_t p_num_handlers,
//             size_t p_queue_size = 100) noexcept {
//     try {
//       assert_event_defined_in_subscriber<t_subscriber, t_event>();

//       auto &_handlings(get_handlings<t_event>());

//       return _handlings.template add_handling<t_subscriber>(
//           p_handling_id, p_subscriber, p_num_handlers, p_handling_priority,
//           p_queue_size);

//     } catch (std::exception &_ex) {
//       m_logger.err(format::fmt("ERROR: ", _ex.what()));
//     } catch (...) {
//       m_logger.err("ERROR unidentified");
//     }
//     return result::UNIDENTIFIED;
//   }

//   /// \brief
//   ///
//   /// \attention you do not need to pass the 'p_queue_id' neither
//   'p_logger' in
//   /// 'p_params...' to create the 't_queue' std::unique_pointer object
//   template <traits::subscriber t_subscriber, traits::event t_event>
//   [[nodiscard]] result
//   subscribe(const handling_id &p_handling_id, t_subscriber &p_subscriber,
//             size_t p_num_handlers, size_t p_queue_size = 100) noexcept {
//     try {
//       assert_event_defined_in_subscriber<t_subscriber, t_event>();

//       auto &_handlings(get_handlings<t_event>());

//       return _handlings.template add_handling<t_subscriber>(
//           p_handling_id, p_subscriber, p_num_handlers,
//           handling_priority::medium, p_queue_size);

//     } catch (std::exception &_ex) {
//       m_logger.err(format::fmt("ERROR: ", _ex.what()));
//     } catch (...) {
//       m_logger.err("ERROR unidentified");
//     }
//     return result::UNIDENTIFIED;
//   }

//   template <traits::publisher t_publisher, traits::event t_event>
//   [[nodiscard]] result publish(const t_event &p_event) noexcept {
//     try {
//       assert_event_defined_in_publisher<t_publisher, t_event>();

//       auto &_handlings(get_handlings<t_event>());

//       auto _result(_handlings.add_event(p_event));
//       if (_result != result::OK) {
//         m_logger.err(format::fmt(_result));
//         return _result;
//       }
//       return result::OK;
//     } catch (std::exception &_ex) {
//       m_logger.err(format::fmt("ERROR: ", _ex.what()));
//     } catch (...) {
//       m_logger.err("ERROR unidentified");
//     }
//     return result::UNIDENTIFIED;
//   }

//   template <traits::publisher t_publisher, traits::event t_event,
//             typename... t_params>
//   [[nodiscard]] result publish(t_params &&...p_params) noexcept {
//     try {
//       assert_event_defined_in_publisher<t_publisher, t_event>();

//       auto &_handling_list(get_handlings<t_event>());

//       auto _result(
//           _handling_list.add_event(std::forward<t_params>(p_params)...));
//       if (_result != result::OK) {
//         m_logger.err(format::fmt(_result));
//         return _result;
//       }
//       return result::OK;
//     } catch (std::exception &_ex) {
//       m_logger.err(format::fmt("ERROR: ", _ex.what()));
//     } catch (...) {
//       m_logger.err("ERROR unidentified");
//     }
//     return result::UNIDENTIFIED;
//   }

//   /// \brief Cleans all handlings of all events
//   void clear() {
//     auto _clear_event = []<size_t t_idx>(events_handlings &p_tuple) {
//       std::get<t_idx>(p_tuple).clear();
//       return true;
//     };

//     traits::tuple_type_traverse(_clear_event, m_events_handlings);
//   }

//   /// \brief Cleans all handlings of a certain event
//   template <traits::event t_event> void clear() {
//     get_handlings<t_event>().clear();
//   }

//   /// \brief Cleans all handlings of a certain handling for a certaind
//   event template <traits::event t_event> void clear(handling_id
//   p_handling_id) {
//     get_handlings<t_event>().clear(p_handling_id);
//   }

//   template <traits::event t_event>
//   void set_priority(const handling_id &p_handling_id,
//                     handling_priority p_priority) {
//     get_handlings<t_event>().set_priority(p_handling_id, p_priority);
//   }

//   template <traits::event t_event>
//   constexpr handling_priority
//   get_priority(const handling_id &p_handling_id) const {
//     return get_handlings<t_event>().get_priority(p_handling_id);
//   }

//   template <traits::event t_event>
//   constexpr size_t get_num_events(const handling_id &p_handling_id) const {
//     return get_handlings<t_event>().get_num_events(p_handling_id);
//   }

//   /// \brief Retrieves the numbers of handlers in a handling of an event,
//   if the
//   /// handling exists'
//   template <traits::event t_event>
//   [[nodiscard]] constexpr std::optional<size_t>
//   get_num_handling_handlers(const handling_id &p_handling_id) const {
//     return get_handlings<t_event>().get_num_handlers(p_handling_id);
//   }

//   template <traits::event t_event> constexpr size_t get_num_handlings()
//   const
//   {
//     return get_handlings<t_event>().get_num_handlings();
//   }

//   // //  /// \brief Removes all events in all the queues of \p t_event
//   // //  template <traits::event t_event> void empty_queue();

//   // //  /// \brief Removes all events in all the queues of \p t_event
//   // //  template <traits::event t_event> void empty_queue(handling_id
//   // //  p_handling_id);

//   // // \brief Waits for all the events in all the groups of  subscribers
//   to be
//   // // handled
//   // //
//   // // TODO test it
//   // //  void wait() {
//   // //    for (const auto &_value_type : m_events_queues) {
//   // //      for (const queue_ptr &_queue_ptr : _value_type.second) {
//   // //                    _queue_ptr->empty_queue();
//   // //      }
//   // //    }
//   // //        m_logger.tra("dispatcher ", get_id(), " - finished
//   waiting");
//   // //  }

// private:
//   // template <traits::logger t_logger, traits::event t_event>
//   // using queue = typename t_supplier::template queue<t_logger, t_event>;

//   using handlings = std::tuple<t_handlings...>;

//   // 'replace_type_with_handling' and 'tuple_cat' were implemented by
//   // ChatGPT
//   // I designed, but I could not figure out how to implement them

//   // Metafunction to replace types in a tuple with handlings<logger, T>
//   template <typename t_event> struct events_to_handlings {
//     using type = handlings<t_event>;
//   };

//   template <typename... t_evs>
//   struct events_to_handlings<std::tuple<t_evs...>> {
//     using type = std::tuple<typename events_to_handlings<t_evs>::type...>;
//   };

//   // Alias template for easy usage
//   template <typename... t_evs>
//   using events_to_handlings_t = typename
//   events_to_handlings<t_evs...>::type;

//   using events_handlings = events_to_handlings_t<events>;

//   // // Metafunction to concatenate tuples
//   // template <typename... Ts> struct handlings_type_creator;

//   // // Base case: Concatenating one tuple, replace its types with
//   // // handlings<logger, T>
//   // template <typename... t_pairs>
//   // struct handlings_type_creator<std::tuple<t_pairs...>> {
//   //   using type =
//   //       typename events_to_handlings<std::tuple<t_pairs...>>::type;
//   // };

//   // // Alias template for easy usage
//   // template <typename... t_pairs>
//   // using handlings_type_creator_t =
//   //     typename handlings_type_creator<t_pairs...>::type;

//   // using events_handlings = handlings_type_creator_t<events>;

//   // // ***
//   // // Metafunction to replace types in a tuple with handlings<logger, T>
//   // template <typename t_pair> struct replace_type_with_events {
//   //   using type = typename t_pair::first_type;
//   // };

//   // template <typename... t_pairs>
//   // struct replace_type_with_events<std::tuple<t_pairs...>> {
//   //   using type =
//   //       std::tuple<typename replace_type_with_events<t_pairs>::type...>;
//   // };

//   // // Metafunction to concatenate tuples
//   // template <typename... Ts> struct events_type_creator;

//   // // Base case: Concatenating one tuple, replace its types with
//   // // handlings<logger, T>
//   // template <typename... t_pairs>
//   // struct events_type_creator<std::tuple<t_pairs...>> {
//   //   using type =
//   //       typename replace_type_with_events<std::tuple<t_pairs...>>::type;
//   // };

//   // // Alias template for easy usage
//   // template <typename... t_pairs>
//   // using events_type_creator_t = typename
//   // events_type_creator<t_pairs...>::type;

//   // using events = events_type_creator_t<events_queues>;
//   // ***

// private:
//   template <traits::event t_event, traits::tuple_like t_evs>
//   static constexpr int16_t exists() {
//     auto _is_same = [&]<traits::tuple_like t_tuple, size_t t_index>() {
//       return !std::is_same_v<std::tuple_element_t<t_index, t_tuple>,
//       t_event>;
//     };
//     constexpr auto _index(traits::tuple_type_traverse<t_evs>(_is_same));
//     return _index;
//   }

//   template <traits::subscriber t_subscriber, traits::event t_event>
//   static constexpr void assert_event_defined_in_subscriber() {
//     using events_subscribed = typename t_subscriber::events_subscribed;
//     constexpr auto _index(exists<t_event, events_subscribed>());
//     static_assert(
//         _index != -1,
//         "trying to subscribe to an event not in 'events_subscribed' of the
//         " "class that wants to subscribe for it");
//   }

//   template <traits::publisher t_publisher, traits::event t_event>
//   static constexpr void assert_event_defined_in_publisher() {
//     using events_published = typename t_publisher::events_published;
//     constexpr auto _index(exists<t_event, events_published>());
//     static_assert(_index != -1,
//                   "trying to publish an event not in 'events_published' of
//                   the " "class that wants to publish it");
//   }

//   template <traits::event t_event>
//   static constexpr size_t assert_event_defined_in_dispatcher() {
//     constexpr auto _index(exists<t_event, events>());
//     static_assert(_index < std::tuple_size_v<events_handlings>,
//                   "index is greater then tuple size");
//     static_assert(
//         _index != -1,
//         "trying to use an event not defined in the events of the dispatcher
//         ");

//     return static_cast<size_t>(_index);
//   }

//   template <traits::event t_event> auto &get_handlings() {
//     constexpr auto _index(assert_event_defined_in_dispatcher<t_event>());
//     return std::get<_index>(m_events_handlings);
//   }

//   template <traits::event t_event> const auto &get_handlings() const {
//     constexpr auto _index(assert_event_defined_in_dispatcher<t_event>());
//     return std::get<_index>(m_events_handlings);
//   }

//   events_handlings create_events_handlings() {
//     events_handlings _events_handlings(
//         [&]<std::size_t... t_idx>(std::index_sequence<t_idx...>)
//             ->events_handlings {
//               return std::make_tuple(
//                   handlings<typename std::tuple_element_t<t_idx, events>>{
//                       m_logger}...);
//             }(std::make_index_sequence<std::tuple_size_v<events>>()));

//     return _events_handlings;
//   }

// private:
//   logger &m_logger;
//   events_handlings m_events_handlings;
// };

} // namespace tenacitas::lib::async

#endif

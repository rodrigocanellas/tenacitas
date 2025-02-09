/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_DISPATCHER_H
#define TENACITAS_LIB_ASYNC_DISPATCHER_H

#include <cstring>
#include <ctime>
#include <functional>
#include <map>
#include <optional>
#include <tuple>

#include "tenacitas.lib/async/handling_id.h"
#include "tenacitas.lib/async/handling_priority.h"
#include "tenacitas.lib/async/internal/handler_id.h"
#include "tenacitas.lib/async/internal/handling.h"
#include "tenacitas.lib/async/result.h"

#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/traits/event.h"
#include "tenacitas.lib/traits/handler.h"
#include "tenacitas.lib/traits/is_type_in_tuple.h"
#include "tenacitas.lib/traits/queue.h"
#include "tenacitas.lib/traits/tuple_find.h"
#include "tenacitas.lib/traits/tuple_like.h"
#include "tenacitas.lib/tuple/tuple_transform.h"
#include "tenacitas.lib/tuple/tuple_traverse.h"

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
  using events = std::tuple<t_events...>;
  using logger = t_logger;

  dispatcher() = delete;

  dispatcher(logger &p_logger) : m_logger(p_logger) {}

  dispatcher(const dispatcher &) = delete;
  dispatcher(dispatcher &&) = delete;

  ~dispatcher() {
    TNCT_LOG_TRA(m_logger, "dispactcher destructor");
    stop();
  }

  dispatcher &operator=(const dispatcher &) = delete;
  dispatcher &operator=(dispatcher &&) = delete;

  void *operator new(size_t) = delete;
  void *operator new[](size_t) = delete;

  void operator delete(void *) = delete;
  void operator delete[](void *) = delete;

  template <traits::event t_event>
  [[nodiscard]] result publish(const t_event &p_event) noexcept {

    event_is_in_events_tupĺe<t_event>();

    try {
      handlings<t_event> &_handlings{get_handlings<t_event>()};

      for (auto &_value : _handlings) {
        _value.second->add_event(p_event);
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERRROR_PUBLISHNG;
    }
    return result::OK;
  }

  template <traits::event t_event, typename... t_event_params>
  [[nodiscard]] result publish(t_event_params &&...p_params) noexcept {

    event_is_in_events_tupĺe<t_event>();

    try {
      handlings<t_event> &_handlings{get_handlings<t_event>()};

      const t_event _event{std::forward<t_event_params>(p_params)...};

      for (auto &_value : _handlings) {
        _value.second->add_event(_event);
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERRROR_PUBLISHNG;
    }
    return result::OK;
  }

  template <traits::event t_event, traits::queue<t_logger, t_event> t_queue,
            traits::handler<t_event> t_handler>
  std::optional<handling_id> add_handling(
      t_handler &&p_handler, t_queue &&p_queue,
      handling_priority p_handling_priority = handling_priority::medium) {

    event_is_in_events_tupĺe<t_event>();

    if (is_handler_already_being_used<t_event, t_handler>()) {
      return std::nullopt;
    }

    using handling_concrete =
        internal::handling_concrete<t_logger, t_event, t_queue, t_handler>;

    try {
      std::lock_guard<std::mutex> _lock(m_mutex);

      handling_ptr<t_event> _handling_ptr{std::make_unique<handling_concrete>(
          ++m_handling_id, m_logger, std::move(p_handler), std::move(p_queue))};

      get_handlings<t_event>().insert(
          {p_handling_priority, std::move(_handling_ptr)});

      return m_handling_id;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }

    return std::nullopt;
  }

  template <traits::event t_event, traits::queue<t_logger, t_event> t_queue,
            traits::handler<t_event> t_handler>
  std::optional<handling_id>
  add_handling(t_handler &&p_handler, handling_priority p_handling_priority,
               t_queue &&p_queue, size_t p_num_handlers) {

    event_is_in_events_tupĺe<t_event>();

    if (is_handler_already_being_used<t_event, t_handler>()) {
      return std::nullopt;
    }

    using handling_concrete =
        internal::handling_concrete<t_logger, t_event, t_queue, t_handler>;

    try {
      std::lock_guard<std::mutex> _lock(m_mutex);

      handling_ptr<t_event> _handling_ptr{std::make_unique<handling_concrete>(
          ++m_handling_id, m_logger, std::move(p_handler), std::move(p_queue),
          p_num_handlers)};

      get_handlings<t_event>().insert(
          {p_handling_priority, std::move(_handling_ptr)});

      return m_handling_id;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }

    return std::nullopt;
  }

  template <traits::event t_event, traits::queue<t_logger, t_event> t_queue,
            traits::handler<t_event> t_handler>
  std::optional<handling_id> add_handling(t_handler &&p_handler, t_queue &&p_queue,
                                       size_t p_num_handlers) {

    event_is_in_events_tupĺe<t_event>();

    if (is_handler_already_being_used<t_event, t_handler>()) {
      return std::nullopt;
    }

    using handling_concrete =
        internal::handling_concrete<t_logger, t_event, t_queue, t_handler>;

    try {
      std::lock_guard<std::mutex> _lock(m_mutex);

      handling_ptr<t_event> _handling_ptr{std::make_unique<handling_concrete>(
          ++m_handling_id, m_logger, std::move(p_handler), std::move(p_queue),
          p_num_handlers)};

      get_handlings<t_event>().insert(
          {handling_priority::medium, std::move(_handling_ptr)});

      return m_handling_id;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }

    return std::nullopt;
  }

  /// \brief Cleans all handlings of a certain event
  template <traits::event t_event> void clear() {
    event_is_in_events_tupĺe<t_event>();
    try {
      handling<t_event> &_handlings{get_handlings<t_event>()};
      for (auto &_value : _handlings) {
        _value.second->clear();
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
  }

  /// \brief Cleans all handlers of a certain handling for a certain event
  template <traits::event t_event> result clear(handling_id p_handling_id) {
    event_is_in_events_tupĺe<t_event>();
    try {
      TNCT_LOG_TRA(m_logger, format::fmt("t_event = ", typeid(t_event).name()));
      auto _clear{[](handling<t_event> &p_handling) { p_handling.clear(); }};

      if (!find_handling<t_event>(p_handling_id, _clear)) {
        TNCT_LOG_ERR(m_logger,
                     format::fmt("Could not find handling ", p_handling_id,
                                 " in event ", typeid(t_event).name()));
        return result::HANDLING_NOT_FOUND;
      }
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_CLEARING;
    }

    return result::OK;
  }

  result clear() {
    try {
      auto _visit{[this]<traits::tuple_like t_tuple, size_t t_idx>() {
        clear<std::tuple_element_t<t_idx, t_tuple>>();
        return true;
      }};

      tuple::tuple_type_traverse<events, decltype(_visit)>(_visit);

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_CLEARING;
    }
    return result::OK;
  }

  template <traits::event t_event>
  [[nodiscard]] result add_handlers(handling_id p_handling_id,
                                    size_t p_amount = 1) {
    event_is_in_events_tupĺe<t_event>();
    try {
      if (find_handling<t_event>(p_handling_id,
                                 [&](handling<t_event> &p_handling) {
                                   p_handling.increment_handlers(p_amount);
                                 })) {
        return result::HANDLING_NOT_FOUND;
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_ADDING_HANDLER;
    }
    return result::OK;
  }

  template <traits::event t_event>
  [[nodiscard]] std::optional<size_t>
  get_amount_handlers(handling_id p_handling_id) const noexcept {
    event_is_in_events_tupĺe<t_event>();
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
  [[nodiscard]] size_t get_amount_handlings() const noexcept {
    event_is_in_events_tupĺe<t_event>();
    auto &_handlings{get_handlings<t_event>()};
    return static_cast<size_t>(_handlings.size());
  }

  result stop() noexcept {
    try {
      auto _visit{[this]<traits::tuple_like t_tuple, size_t t_idx>() {
        stop<std::tuple_element_t<t_idx, t_tuple>>();
        return true;
      }};

      tuple::tuple_type_traverse<events, decltype(_visit)>(_visit);

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_STOPPING;
    }
    return result::OK;
  }

  template <traits::event t_event> result stop() noexcept {
    event_is_in_events_tupĺe<t_event>();
    try {
      handlings<t_event> &_handlings{get_handlings<t_event>()};

      for (auto &_value : _handlings) {
        _value.second->stop();
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_STOPPING;
    }
    return result::OK;
  }

  template <traits::event t_event>
  [[nodiscard]] result stop(handling_id p_handling_id) noexcept {
    event_is_in_events_tupĺe<t_event>();
    try {
      TNCT_LOG_TRA(m_logger, format::fmt("t_event = ", typeid(t_event).name()));
      auto _stopper{[](handling<t_event> &p_handling) { p_handling.stop(); }};

      if (!find_handling<t_event>(p_handling_id, _stopper)) {
        TNCT_LOG_ERR(m_logger,
                     format::fmt("Could not find handling ", p_handling_id,
                                 " in event ", typeid(t_event).name()));
        return result::HANDLING_NOT_FOUND;
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_STOPPING;
    }
    return result::OK;
  }

  template <traits::event t_event>
  [[nodiscard]] std::optional<bool>
  is_stopped(handling_id p_handling_id) const {
    try {
      bool _is_stopped{false};
      auto _stopper{[&](const handling<t_event> &p_handling) {
        _is_stopped = p_handling.is_stopped();
      }};

      if (!find_handling<t_event>(p_handling_id, _stopper)) {
        return std::nullopt;
      }
      return _is_stopped;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return std::nullopt;
  }

private:
  template <traits::event t_event> using handling = internal::handling<t_event>;

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
  template <traits::event t_event, traits::handler<t_event> t_handler>
  [[nodiscard]] bool is_handler_already_being_used() const {

    const internal::handler_id _handler_id{
        internal::get_handler_id<t_event, t_handler>()};

    TNCT_LOG_TRA(m_logger, format::fmt("handler id = ", _handler_id,
                                       " for event ", typeid(t_event).name()));

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
  [[nodiscard]] static constexpr size_t get_handlings_index() {
    constexpr auto _idx{traits::tuple_find<events, t_event>()};
    static_assert(_idx != -1, "event not found in the tuple of events");
    return static_cast<size_t>(_idx);
  }

  template <traits::event t_event>
  [[nodiscard]] handlings<t_event> &get_handlings() {
    return std::get<get_handlings_index<t_event>()>(m_events_handlings);
  }

  template <traits::event t_event>
  [[nodiscard]] const handlings<t_event> &get_handlings() const {
    return std::get<get_handlings_index<t_event>()>(m_events_handlings);
  }

  template <traits::event t_event>
  [[nodiscard]] bool
  find_handling(handling_id p_handling_id,
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
  [[nodiscard]] handlings_ite<t_event>
  find_handling(handlings<t_event> &p_handlings, handling_id p_handling_id) {

    auto _match{[&](const typename handlings<t_event>::value_type &p_value) {
      return p_value.second->get_id() == p_handling_id;
    }};

    return {std::find_if(p_handlings.begin(), p_handlings.end(), _match)};
  }

  template <traits::event t_event>
  bool
  find_handling(handling_id p_handling_id,
                std::function<void(const handling<t_event> &)> p_exec) const {
    const handlings<t_event> &_handlings{get_handlings<t_event>()};

    auto _match{[&](const typename handlings<t_event>::value_type &p_value) {
      const auto _handling_id{p_value.second->get_id()};
      TNCT_LOG_TRA(m_logger,
                   format::fmt("p_value.second->get_id() = ", _handling_id));
      return _handling_id == p_handling_id;
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

  template <traits::event t_event>
  static constexpr void event_is_in_events_tupĺe() {
    static_assert(traits::is_type_in_tuple<events, t_event>,
                  "event is not in the 't_events...' of the dispatcher");
  }

private:
  logger &m_logger;

  events_handlings m_events_handlings;

  handling_id m_handling_id{0};
  std::mutex m_mutex;
};

} // namespace tenacitas::lib::async

#endif

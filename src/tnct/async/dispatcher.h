/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_DISPATCHER_H
#define TNCT_ASYNC_DISPATCHER_H

#include <cstring>
#include <ctime>
#include <functional>
#include <map>
#include <optional>
#include <tuple>

#include "tnct/async/handling_id.h"
#include "tnct/async/handling_priority.h"
#include "tnct/async/internal/handler_id.h"
#include "tnct/async/internal/handling.h"
#include "tnct/async/result.h"
#include "tnct/traits/async/handling_definition.h"

#include "tnct/format/fmt.h"
#include "tnct/traits/async/event.h"
#include "tnct/traits/async/handler.h"
#include "tnct/traits/container/queue.h"
#include "tnct/traits/tuple/contains_type.h"
#include "tnct/traits/tuple/find.h"
#include "tnct/traits/tuple/like.h"
#include "tnct/traits/tuple/traverse.h"
#include "tnct/tuple/tuple_transform.h"
#include "tnct/tuple/tuple_traverse.h"

namespace tnct::async {

/** \brief Class that allows creation of queues for an event, creation of
subscribers for an event, and publishing of events


The UML class diagram below, written in www.plantuml.com, shows the conceptual
structure of the \p dispatcher and other associated structures

@startuml
hide empty members
allow_mixing
skinparam linetype ortho


class dispatcher<t_events...>
class handling<t_event>
class queue<t_event>
class handler
class event

publisher -[#green]->  event
handling -[#blue]->  event
queue *-- "*" event
handling "1" *-- "*" handler
handling *-up- handling_priority
dispatcher *-- "*" handling
handling *-- queue
publisher o-left- dispatcher

note "<color green>publishes\n<color blue>subscribes" as legend
@enduml

The central concept is an \p event, a struct that contains data about an
interesting event in the application. It can be, for example, an incoming
message from a network connection, a user menu choice or a temperature coming
from a sensor.

A \p handling is how one can define which type of \p handler will be executed
for the \p event.

Each \p handling has its own \p queue of \p event objects, and the
implementantion of the queue can vary as much as necessary, as far as it
satisfies the requirements defined by \p traits::container::queue.

A \p handling can have multiples instances of the \p hander type, in case each
\p event instance can be handled independently from any other \p event.

It is possible to define a \p handling_priority for a \p handling, so that
handlings that have higher priority will have the event copied to its queue
before a handling with lower priority.

Please, take a look at the tests and examples for more information on how to use
the dispatcher class.
*/

template <traits::log::logger t_logger, traits::async::event... t_events>
struct dispatcher final {

public:
  using events = std::tuple<t_events...>;
  using logger = t_logger;

  dispatcher() = delete;

  dispatcher(logger &p_logger) : m_logger(p_logger) {}

  dispatcher(const dispatcher &) = delete;
  dispatcher(dispatcher &&) = delete;

  ~dispatcher() {
    TNCT_LOG_TRA(m_logger, "dispatcher destructor");
    stop();
  }

  dispatcher &operator=(const dispatcher &) = delete;
  dispatcher &operator=(dispatcher &&) = delete;

  void *operator new(size_t) = delete;
  void *operator new[](size_t) = delete;

  void operator delete(void *) = delete;
  void operator delete[](void *) = delete;

  template <traits::async::event t_event>
  [[nodiscard]] result publish(const t_event &p_event) noexcept {

    event_is_in_events_tupĺe<t_event>();

    try {
      handlings<t_event> &_handlings{get_handlings<t_event>()};

      for (auto &_value : _handlings) {
        _value.second->add_event(p_event);
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_PUBLISHNG;
    }
    return result::OK;
  }

  template <traits::async::event t_event, typename... t_event_params>
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
      return result::ERROR_PUBLISHNG;
    }
    return result::OK;
  }

  template <traits::async::handling_definition t_handling>
  result add_handling(t_handling &&p_handling) {
    return add_handling<typename t_handling::event,
                        typename t_handling::handler,
                        typename t_handling::queue>(
        p_handling.get_id(), std::move(p_handling.get_handler()),
        std::move(p_handling.get_queue()), p_handling.get_num_handlers(),
        p_handling.get_priority());
  }

  /// \brief Clears the events queue of all handlings of an event
  template <traits::async::event t_event> void clear() {
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

  /// \brief Clears the events queue of a handling of an event
  template <traits::async::event t_event>
  result clear(handling_id p_handling_id) {
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

  /// \brief Clears the events queue of all handlings of all events
  result clear() {
    try {
      auto _visit{[this]<traits::tuple::like t_tuple, size_t t_idx>() {
        clear<std::tuple_element_t<t_idx, t_tuple>>();
        return true;
      }};

      tuple::tuple_traverse<events, decltype(_visit)>(_visit);

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_CLEARING;
    }
    return result::OK;
  }

  template <traits::async::event t_event>
  [[nodiscard]] size_t get_amount_handlings() const noexcept {
    event_is_in_events_tupĺe<t_event>();
    auto &_handlings{get_handlings<t_event>()};
    return static_cast<size_t>(_handlings.size());
  }

  template <traits::async::event t_event>
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

private:
  template <traits::async::event t_event>
  using handling = internal::handling<t_event>;

  template <traits::async::event t_event>
  using handling_ptr = std::unique_ptr<handling<t_event>>;

  template <traits::async::event t_event>
  using handling_const_ptr = std::unique_ptr<const handling<t_event>>;

  template <traits::async::event t_event>
  using handlings = std::multimap<handling_priority, handling_ptr<t_event>>;

  template <traits::async::event t_event>
  using handlings_ite = typename handlings<t_event>::iterator;

  template <traits::async::event t_event>
  using handlings_const_ite = typename handlings<t_event>::const_iterator;

  using events_handlings =
      typename tuple::tuple_transform<events, handlings>::type;

private:
  // \brief Stops all the handlers of all handlings of all events
  result stop() noexcept {
    try {
      auto _visit{[this]<traits::tuple::like t_tuple, size_t t_idx>() {
        stop<std::tuple_element_t<t_idx, t_tuple>>();
        return true;
      }};

      traits::tuple::traverse<events>(_visit);

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
      return result::ERROR_STOPPING;
    }
    return result::OK;
  }

  template <traits::async::event t_event> result stop() noexcept {
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

  // template <traits::async::event t_event>
  // result stop(handling_id p_handling_id) noexcept {
  //   event_is_in_events_tupĺe<t_event>();
  //   try {
  //     TNCT_LOG_TRA(m_logger, format::fmt("t_event = ",
  //     typeid(t_event).name())); auto _stopper{[](handling<t_event>
  //     &p_handling) { p_handling.stop(); }};

  //     if (!find_handling<t_event>(p_handling_id, _stopper)) {
  //       TNCT_LOG_ERR(m_logger,
  //                    format::fmt("Could not find handling ", p_handling_id,
  //                                " in event ", typeid(t_event).name()));
  //       return result::HANDLING_NOT_FOUND;
  //     }

  //   } catch (std::exception &_ex) {
  //     TNCT_LOG_ERR(m_logger, _ex.what());
  //     return result::ERROR_STOPPING;
  //   }
  //   return result::OK;
  // }

  template <traits::async::event t_event,
            traits::async::handler<t_event> t_handler>
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

  template <traits::async::event t_event>
  [[nodiscard]] static constexpr std::size_t get_handlings_index() {
    constexpr auto _idx{traits::tuple::find<events, t_event>()};
    static_assert(_idx, "event not found in the tuple of events");
    return _idx.value();
  }

  template <traits::async::event t_event>
  [[nodiscard]] handlings<t_event> &get_handlings() {
    return std::get<get_handlings_index<t_event>()>(m_events_handlings);
  }

  template <traits::async::event t_event>
  [[nodiscard]] const handlings<t_event> &get_handlings() const {
    return std::get<get_handlings_index<t_event>()>(m_events_handlings);
  }

  template <traits::async::event t_event>
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

  template <traits::async::event t_event>
  [[nodiscard]] handlings_ite<t_event>
  find_handling(handlings<t_event> &p_handlings, handling_id p_handling_id) {

    auto _match{[&](const typename handlings<t_event>::value_type &p_value) {
      return p_value.second->get_id() == p_handling_id;
    }};

    return {std::find_if(p_handlings.begin(), p_handlings.end(), _match)};
  }

  template <traits::async::event t_event>
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

  template <traits::async::event t_event>
  static constexpr void event_is_in_events_tupĺe() {
    static_assert(traits::tuple::contains_type<events, t_event>(),
                  "event is not in the 't_events...' of the dispatcher");
  }

  template <traits::async::event t_event,
            traits::async::handler<t_event> t_handler,
            traits::container::queue<t_event> t_queue>
  result add_handling(
      const handling_id &p_handling_id, t_handler &&p_handler,
      t_queue &&p_queue, size_t p_num_handlers,
      handling_priority p_handling_priority = handling_priority::medium) {

    event_is_in_events_tupĺe<t_event>();

    if (is_handler_already_being_used<t_event, t_handler>()) {
      return result::ERROR_HANDLER_ALREADY_IN_USE;
    }

    using handling_concrete =
        internal::handling_concrete<t_logger, t_event, t_queue, t_handler>;

    try {
      std::lock_guard<std::mutex> _lock(m_mutex);

      handling_ptr<t_event> _handling_ptr{std::make_unique<handling_concrete>(
          p_handling_id, m_logger, std::move(p_handler), std::move(p_queue),
          p_num_handlers)};

      get_handlings<t_event>().insert(
          {p_handling_priority, std::move(_handling_ptr)});

      return result::OK;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }

    return result::ERROR_UNKNOWN;
  }

private:
  logger &m_logger;

  events_handlings m_events_handlings;

  std::mutex m_mutex;
};

} // namespace tnct::async

#endif

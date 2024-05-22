/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_SUBSCRIPTIONS_H
#define TENACITAS_LIB_ASYNC_SUBSCRIPTIONS_H

#include <functional>
#include <iostream>
#include <map>
#include <mutex>

#include <tenacitas.lib/async/handling_id.h>
#include <tenacitas.lib/async/handling_priority.h>
#include <tenacitas.lib/async/internal/handler.h>
#include <tenacitas.lib/async/internal/handling.h>
#include <tenacitas.lib/async/result.h>
#include <tenacitas.lib/generic/fmt.h>
#include <tenacitas.lib/traits/logger.h>
#include <tenacitas.lib/traits/queue.h>
#include <tenacitas.lib/traits/subscriber.h>
#include <tenacitas.lib/traits/tuple_traverse.h>

using namespace std::placeholders;

namespace tenacitas::lib::async::internal {

template <traits::logger t_logger, traits::event t_event> struct handlings {
  using event = t_event;
  using logger = t_logger;
  using handler = async::internal::handler<event>;

  handlings() = default;
  handlings(logger &p_logger) : m_logger(p_logger) {}
  handlings(const handlings &) = delete;
  handlings(handlings &&p_handlings)
      : m_logger(p_handlings.m_logger),
        m_container(std::move(p_handlings.m_container)) {}

  handlings &operator=(const handlings &) = delete;
  handlings &operator=(handlings &&) = default;

  ~handlings() = default;

  /// \brief
  ///
  /// \attention you do not need to pass the 'p_queue_id' neither 'p_logger' in
  /// 'p_params...' to create the 't_queue' std::unique_pointer object
  template <typename t_subscriber, typename t_queue, typename... t_queue_params>
  requires std::is_base_of_v<traits::i_queue<t_logger, t_event>, t_queue>
  [[nodiscard]] result add_handling(const handling_id &p_handling_id,
                                    t_subscriber &p_subscriber,
                                    size_t p_num_handlers,
                                    handling_priority p_handling_priority,
                                    t_queue_params &&...p_params) {
    std::lock_guard<std::mutex> _lock(m_mutex);

    if (get_handling(p_handling_id) != m_container.end()) {
      m_logger.err(generic::fmt("not adding handling '", p_handling_id,
                                "' because it exists"));
      return result::HANDLING_EXISTS;
    }

    const std::string _subscriber_id_str(
        std::string(typeid(t_subscriber).name()));

    const handler_id _handler_id_hash =
        std::hash<std::string>()(_subscriber_id_str);

    TNCT_LOG_TRA(m_logger,
                 generic::fmt("subscriber id str = ", _subscriber_id_str,
                              ", subscriber id hash =  ", _handler_id_hash,
                              ", for event ", typeid(event).name(),
                              " in handling ", p_handling_id));

    for (const auto &_value : m_container) {
      if (get_handler_id(_value) == _handler_id_hash) {
        m_logger.err(generic::fmt("handler already used in '",
                                  get_handling(_value).get_id(), '\''));
        return result::HANDLER_USED;
      }
    }

    handling_ptr _handling(
        handling::template create<t_queue, t_queue_params...>(
            p_handling_id, m_logger,
            [&](auto &&p_event) { p_subscriber.handle(std::move(p_event)); },
            p_num_handlers, p_handling_priority,
            std::forward<t_queue_params>(p_params)...));

    m_container.insert(
        {p_handling_priority,
         std::make_pair(std::move(_handling), _handler_id_hash)});

    return result::OK;
  }

  [[nodiscard]] result add_event(const event &p_event) {
    auto _result(result::OK);
    for (auto &_value : m_container) {
      _result = get_handling(_value).add_event(p_event);
      if (_result != result::OK) {
        m_logger.err(generic::fmt("error ", _result,
                                  " adding event for handling ",
                                  get_handling(_value).get_id()));
        return _result;
      }
    }
    return result::OK;
  }

  template <typename... t_params>
  [[nodiscard]] result add_event(t_params &&...p_params) {
    auto _result(result::OK);
    for (auto &_value : m_container) {
      _result = get_handling(_value).add_event(
          event(std::forward<t_params>(p_params)...));
      if (_result != result::OK) {
        m_logger.err(generic::fmt("error ", _result,
                                  " adding event for handling ",
                                  get_handling(_value).get_id()));
        return _result;
      }
    }
    return result::OK;
  }

  template <std::integral t_num_handlers>
  [[nodiscard]] result increment_handlers(const handling_id &p_handling_id,
                                          t_num_handlers p_num_handlers) {
    iterator _ite(get_handling(p_handling_id));
    if (_ite == m_container.end()) {
      return result::HANDLING_DOES_NOT_EXIST;
    }
    get_handling(_ite).increment_handlers(p_num_handlers);
    return result::OK;
  }

  void stop() {
    for (auto &_value : m_container) {
      get_handling(_value).stop();
    }
  }

  void clear(const handling_id &p_handling_id) {
    iterator _ite(get_handling(p_handling_id));
    if (_ite != m_container.end()) {
      get_handling(_ite).clear();
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_container.erase(_ite);
    }
  }

  void clear() {
    std::lock_guard<std::mutex> _lock(m_mutex);
    for (auto &_value : m_container) {
      get_handling(_value).clear();
    }
  }

  void set_priority(const handling_id &p_handling_id,
                    handling_priority p_handling_priority) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    iterator _ite(get_handling(p_handling_id));
    if (_ite != m_container.end()) {
      handling_ptr _handling_ptr = std::move(_ite->second.first);
      handler_id _handler_id = get_handler_id(_ite);
      _handling_ptr->set_priority(p_handling_priority);
      m_container.erase(_ite);
      m_container.insert(
          {p_handling_priority,
           std::make_pair(std::move(_handling_ptr), _handler_id)});
    }
  }

  constexpr size_t get_num_events(const handling_id &p_handling_id) const {
    const_iterator _ite(get_handling(p_handling_id));
    return (_ite != m_container.end() ? get_handling(_ite).get_num_events()
                                      : 0);
  }

  /// \brief Retrieves if a handling is stopped, if it exists
  constexpr std::optional<bool>
  is_stopped(const handling_id &p_handling_id) const {
    const_iterator _ite(get_handling(p_handling_id));
    return (_ite != m_container.end() ? get_handling(_ite).is_stopped()
                                      : std::nullopt);
  }

  constexpr handling_priority
  get_priority(const handling_id &p_handling_id) const {
    const_iterator _ite(get_handling(p_handling_id));
    return (_ite != m_container.end() ? get_handling(_ite).get_priority()
                                      : handling_priority::lowest);
  }

  /// \brief Retrieves the numbers of handlers in a handling, if it exists'
  constexpr std::optional<size_t>
  get_num_handlers(const handling_id &p_handling_id) const {
    const_iterator _ite(get_handling(p_handling_id));
    return (_ite != m_container.end() ? get_handling(_ite).get_num_handlers()
                                      : 0);
  }

  void traverse(std::function<void(handling<logger, event> &)> p_visit) {
    for (auto &_value : m_container) {
      p_visit(get_handling(_value));
    }
  }

  constexpr size_t get_num_handlings() const {
    return static_cast<size_t>(m_container.size());
  }

  friend std::ostream &operator<<(std::ostream &p_out, const handlings &p_eql) {
    p_out << std::endl;
    for (const auto &_value : p_eql.m_container) {
      p_out << "{" << p_eql.get_handling(_value) << "} " << std::endl;
    }
    return p_out;
  }

private:
  using handling = async::internal::handling<logger, event>;

  using handling_ptr = typename handling::ptr;

  using handler_id = size_t;

  using container = std::multimap<async::handling_priority,
                                  std::pair<handling_ptr, handler_id>,
                                  std::greater<async::handling_priority>>;

  using iterator = typename container::iterator;

  using const_iterator = typename container::const_iterator;

private:
  handling &get_handling(iterator p_ite) { return *(p_ite->second.first); }

  const handling &get_handling(const_iterator p_ite) const {
    return *(p_ite->second.first);
  }

  iterator get_handling(const handling_id &p_handling_id) {
    for (iterator _ite = m_container.begin(); _ite != m_container.end();
         ++_ite) {
      if (get_handling(_ite).get_id() == p_handling_id) {
        return _ite;
      }
    }
    return m_container.end();
  }

  const_iterator get_handling(const handling_id &p_handling_id) const {
    for (const_iterator _ite = m_container.begin(); _ite != m_container.end();
         ++_ite) {
      if (get_handling(_ite).get_id() == p_handling_id) {
        return _ite;
      }
    }
    return m_container.end();
  }

  handler_id get_handler_id(const_iterator p_ite) const {
    return p_ite->second.second;
  }

  handling &get_handling(container::value_type &p_value) {
    return *(p_value.second.first);
  }

  const handling &get_handling(const container::value_type &p_value) const {
    return *(p_value.second.first);
  }

  handler_id get_handler_id(const container::value_type &p_value) const {
    return p_value.second.second;
  }

private:
  logger &m_logger;
  container m_container;
  std::mutex m_mutex;
};

} // namespace tenacitas::lib::async::internal

#endif

#ifndef TENACITAS_LIB_IPC_POSIX_MESSENGER_H
#define TENACITAS_LIB_IPC_POSIX_MESSENGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>

#include <tenacitas/lib/async/dispatcher.h>
#include <tenacitas/lib/ipc/bus/active_connector.h>
#include <tenacitas/lib/ipc/bus/connection.h>
#include <tenacitas/lib/ipc/bus/deserialize.h>
#include <tenacitas/lib/ipc/bus/passive_connector.h>
#include <tenacitas/lib/ipc/bus/reader.h>
#include <tenacitas/lib/ipc/bus/serialize.h>
#include <tenacitas/lib/ipc/typ/types.h>

using namespace std::chrono_literals;

namespace tenacitas::lib::ipc::bus {

/// \brief
template <typename t_connection> struct messenger_t {

  using connection = t_connection;
  using connection_from = evt::connection_from_t<connection>;
  using passive_connector = std::function<void(const typ::endpoint &)>;
  //  using active_connector =
  //      std::function<std::optional<connection>(const typ::endpoint &)>;
  using reader =
      std::function<std::pair<typ::status, typ::buffer>>(connection &);
  using writer = std::function<typ::status(connection &, typ::buffer &&)>;

  messenger_t() = delete;

  messenger_t(async::alg::dispatcher::ptr p_dispatcher)
      : m_dispatcher(p_dispatcher), /*m_active_connector(p_dispatcher),*/
        m_buffer_read_handling(p_dispatcher->add_handling<user_message>()),
        m_read_connection_read(p_dispatcher->add_handling<read_connection>()) {

    m_dispatcher->subscribe<connection_from>(
        [this](auto p_event) -> void { on_connection_from(p_event); });

    //    m_dispatcher->subscribe<internal_message>(
    //        [this](auto p_event) -> void { handle_internal_message(p_event);
    //        });

    m_dispatcher->subscribe<evt::listen>([this](auto p_event) -> void {
      this->listen(p_event->endpoint, p_event->passive_connector);
    });
  }

  ~messenger_t() {
    m_stop = true;
    //    for (auto &_value: m_passive_connectors) {
    //      _value->stop();
    //    }
  }

  /// \brief The class responsible for \p p_passive_connector must use a
  /// tenacitas::lib::async::alg::dispatcher::publish to publish a
  /// tenacitas::lib::ipc::evt::connection_from<connection> event
  void
  listen(const typ::endpoint &p_endpoint,
         std::function<void(const typ::endpoint &)> &&p_passive_connector) {
    auto _ite = m_passive_connectors.insert(
        {p_endpoint, std::move(p_passive_connector)});
    _ite.first->second(p_endpoint);
  }

  /// \brief Defines the types of incoming messages
  template <typename... t_msgs> void in() {
    m_dispatcher->subscribe<user_message>(
        m_buffer_read_handling, sizeof...(t_msgs), [this]() {
          return ([this](auto p_event) {
            return this->on_user_msg<t_msgs...>(p_event);
          });
        });
  }

private:
  using passive_connectors = std::map<typ::endpoint, passive_connector>;
  using connections = std::map<typ::endpoint, connection>;
  using iterator = typename connections::iterator;
  using header_internal_msg = std::array<std::byte, 4>;

  struct user_message {
    user_message() = delete;
    user_message(connection &p_conn, std::vector<std::byte> &&p_msg)
        : conn(p_conn), msg(std::move(p_msg)) {}

    friend std::ostream &operator<<(std::ostream &p_out, user_message) {
      p_out << "user_message";
      return p_out;
    }

    std::vector<std::byte> msg;
    connection &conn;
  };

  struct read_connection {
    read_connection() = delete;
    read_connection(connection &p_connection) : conn(p_connection) {}
    friend std::ostream &operator<<(std::ostream &p_out,
                                    const read_connection &) {
      p_out << "read";
      return p_out;
    }

    connection &conn;
  };

private:
  template <typename... t_msgs>
  void on_user_msg(std::shared_ptr<user_message> p_event) {
    on_user_msg<0, t_msgs...>(p_event);
  }

  template <size_t index, typename... t_msgs>
  void on_user_msg(std::shared_ptr<user_message> p_event) {
    using messages = std::tuple<t_msgs...>;
    using message = typename std::tuple_element<index, messages>::type;

    std::optional<message> _maybe{deserialize<message>(p_event->msg)};
    if (_maybe) {
      m_dispatcher->publish<message>(std::move(_maybe.value()));
      return;
    }

    if constexpr (index + 1 != sizeof...(t_msgs)) {
      return on_user_msg<index + 1, t_msgs...>(p_event);
    }
  }

  void on_connection_from(std::shared_ptr<connection_from> p_event) {
    auto _pair{m_connections.insert(
        {p_event->connection.get_endpoint(), std::move(p_event->connection)})};

    m_dispatcher->subscribe<read_connection>(
        m_read_connection_read,
        [this](auto p_event) -> void { on_read_connection(p_event); });

    m_dispatcher->publish<read_connection>(_pair.first->second);
  }

  std::pair<typ::status, bool>
  handle_internal_msg(connection &p_connection,
                      header_internal_msg &p_header_internal_msg) {
    typ::status _status{p_connection.read(p_header_internal_msg.data(),
                                          p_header_internal_msg.size())};

    if (_status != typ::status::OK) {
      return {_status, false};
    }

    if (!is_internal_msg(p_header_internal_msg)) {
      return {typ::status::OK, false};
    }

    std::pair<typ::status, typ::internal_message_type> _result{
        read_internal_msg_type(p_connection)};

    if (_result.first != typ::status::OK) {
      return {_status, false};
    }

    if (_result.second == typ::internal_message_type::subscribe) {
      _status == handle_subscribe_internal_message(p_connection);
      if (_status != typ::status::OK) {
        return {_status, false};
      }
    }

    return {typ::status::ERROR_UNDEFINED_INTERNAL_MESSAGE, false};
  }

  typ::status handle_subscribe_internal_message(connection &p_connection) {
    // read message name and ...
  }

  std::pair<typ::status, typ::internal_message_type>
  read_internal_message_type(connection &p_connection) {
    using id_type = std::underlying_type<typ::internal_message_type>::type;
    id_type _id;

    typ::status _status =
        p_connection.read(reinterpret_cast<std::byte *>(&_id), sizeof(id_type));

    if (_status != typ::status::OK) {
      return {_status, typ::internal_message_type::undefined};
    }

    return {_status, typ::from_value(_id)};
  }

  void read_buffer(connection &p_connection,
                   header_internal_msg &&p_header_internal_msg) {
    std::vector<std::byte> _buffer{p_header_internal_msg.begin(),
                                   p_header_internal_msg.end()};

    typ::status _status{p_connection.read(_buffer.end())};

    if (_status != typ::status::OK) {
      return;
    }

    m_dispatcher->publish<user_message>(p_connection, std::move(_buffer));
  }

  void on_read_connection(std::shared_ptr<read_connection> p_event) {
    while (true) {
      if (m_stop) {
        return;
      }

      header_internal_msg _header_internal_msg;

      auto _result{handle_internal_msg(p_event->conn, _header_internal_msg)};

      if ((_result.first != typ::status::OK) || (_result.second)) {
        return;
      }

      if (m_stop) {
        return;
      }

      read_buffer(p_event->conn, std::move(_header_internal_msg));
    }
  }

  inline bool
  is_internal_msg(const header_internal_msg &p_header_internal_msg) {
    static std::array<std::byte, 4> _internal_msg;
    _internal_msg[0] = static_cast<std::byte>('T');
    _internal_msg[1] = static_cast<std::byte>('N');
    _internal_msg[2] = static_cast<std::byte>('C');
    _internal_msg[3] = static_cast<std::byte>('T');

    if ((p_header_internal_msg[0] != _internal_msg[0]) ||
        (p_header_internal_msg[1] != _internal_msg[1]) ||
        (p_header_internal_msg[2] != _internal_msg[2]) ||
        (p_header_internal_msg[3] != _internal_msg[3])) {
      return false;
    }
    return true;
  }

private:
  async::alg::dispatcher::ptr m_dispatcher;
  //  active_connector m_active_connector;

  /// \brief Handling for \p buffer_read events
  async::handling_id m_buffer_read_handling;
  async::handling_id m_read_connection_read;

  passive_connectors m_passive_connectors;
  connections m_connections;
  std::atomic_bool m_stop{false};

  static const constexpr uint8_t size_header_internal_msg{4};
}; // namespace tenacitas::lib::ipc::bus
} // namespace tenacitas::lib::ipc::bus

#endif

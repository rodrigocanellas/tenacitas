

#include <tenacitas.h>

namespace tnct::lib {

enum class communication_status_t : uint8_t {
  NO_ERROR = 0,
  ERROR_UNINDENTIFIED,
  ERROR_WRITING,
  ERROR_READING,
  ERROR_TIMEOUT_WRITING,
  ERROR_TIMEOUT_READING,
  ERROR_CONNECTING,
  ERROR_TIMEOUT_CONNECTING
};

std::ostream &operator<<(std::ostream &p_out, communication_status_t p_status) {
  switch (p_status) {
  case communication_status_t::NO_ERROR:
    p_out << "NO_ERROR";
    break;
  case communication_status_t::ERROR_UNINDENTIFIED:
    p_out << "ERROR_UNINDENTIFIED";
    break;
  case communication_status_t::ERROR_WRITING:
    p_out << "ERROR_WRITING";
    break;
  case communication_status_t::ERROR_READING:
    p_out << "ERROR_READING";
    break;
  case communication_status_t::ERROR_TIMEOUT_WRITING:
    p_out << "ERROR_TIMEOUT_WRITING";
    break;
  case communication_status_t::ERROR_TIMEOUT_READING:
    p_out << "ERROR_TIMEOUT_READING";
    break;
  case communication_status_t::ERROR_CONNECTING:
    p_out << "ERROR_CONNECTING";
    break;
  case communication_status_t::ERROR_TIMEOUT_CONNECTING:
    p_out << "ERROR_TIMEOUT_CONNECTING";
    break;
  }
  return p_out;
}

struct connection_error_unidentified_e {
  connection_error_unidentified_e() = default;

  connection_error_unidentified_e(const connection_error_unidentified_e &) =
      default;

  connection_error_unidentified_e(connection_error_unidentified_e &&) = default;

  connection_error_unidentified_e &
  operator=(const connection_error_unidentified_e &) = default;

  connection_error_unidentified_e &
  operator=(connection_error_unidentified_e &&) = default;

  connection_error_unidentified_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

struct connection_error_writing_e {
  connection_error_writing_e() = default;

  connection_error_writing_e(const connection_error_writing_e &) = default;

  connection_error_writing_e(connection_error_writing_e &&) = default;

  connection_error_writing_e &
  operator=(const connection_error_writing_e &) = default;

  connection_error_writing_e &
  operator=(connection_error_writing_e &&) = default;

  connection_error_writing_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

struct connection_error_reading_e {
  connection_error_reading_e() = default;

  connection_error_reading_e(const connection_error_reading_e &) = default;

  connection_error_reading_e(connection_error_reading_e &&) = default;

  connection_error_reading_e &
  operator=(const connection_error_reading_e &) = default;

  connection_error_reading_e &
  operator=(connection_error_reading_e &&) = default;

  connection_error_reading_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

struct connection_error_timeout_writing_e {
  connection_error_timeout_writing_e() = default;

  connection_error_timeout_writing_e(
      const connection_error_timeout_writing_e &) = default;

  connection_error_timeout_writing_e(connection_error_timeout_writing_e &&) =
      default;

  connection_error_timeout_writing_e &
  operator=(const connection_error_timeout_writing_e &) = default;

  connection_error_timeout_writing_e &
  operator=(connection_error_timeout_writing_e &&) = default;

  connection_error_timeout_writing_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

struct connection_error_timeout_reading_e {
  connection_error_timeout_reading_e() = default;

  connection_error_timeout_reading_e(
      const connection_error_timeout_reading_e &) = default;

  connection_error_timeout_reading_e(connection_error_timeout_reading_e &&) =
      default;

  connection_error_timeout_reading_e &
  operator=(const connection_error_timeout_reading_e &) = default;

  connection_error_timeout_reading_e &
  operator=(connection_error_timeout_reading_e &&) = default;

  connection_error_timeout_reading_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

struct connection_error_connecting_e {
  connection_error_connecting_e() = default;

  connection_error_connecting_e(const connection_error_connecting_e &) =
      default;

  connection_error_connecting_e(connection_error_connecting_e &&) = default;

  connection_error_connecting_e &
  operator=(const connection_error_connecting_e &) = default;

  connection_error_connecting_e &
  operator=(connection_error_connecting_e &&) = default;

  connection_error_connecting_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

struct connection_error_timeout_connecting_e {
  connection_error_timeout_connecting_e() = default;

  connection_error_timeout_connecting_e(
      const connection_error_timeout_connecting_e &) = default;

  connection_error_timeout_connecting_e(
      connection_error_timeout_connecting_e &&) = default;

  connection_error_timeout_connecting_e &
  operator=(const connection_error_timeout_connecting_e &) = default;

  connection_error_timeout_connecting_e &
  operator=(connection_error_timeout_connecting_e &&) = default;

  connection_error_timeout_connecting_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

struct connection_lost_e {
  connection_lost_e() = default;

  connection_lost_e(const connection_lost_e &) = default;

  connection_lost_e(connection_lost_e &&) = default;

  connection_lost_e &operator=(const connection_lost_e &) = default;

  connection_lost_e &operator=(connection_lost_e &&) = default;

  connection_lost_e(tld::id p_connection_id)
      : connection_id(p_connection_id) {}

  tld::id connection_id;
};

template <typename t>
concept one_byte_c = (sizeof(t) == 1);

template <typename t>
concept communication_io_c = requires(t p_t) {
  requires std::copy_constructible<t> && std::move_constructible<t> &&
      std::default_initializable<t>;

  {
    p_t.write(std::declval<const uint8_t *>(), std::declval<const uint8_t *>())
    } -> std::same_as<communication_status_t>;

  {
    p_t.read(std::declval<uint8_t *>(), std::declval<uint8_t *>(),
             std::declval<uint8_t **>())
    } -> std::same_as<communication_status_t>;

  {
    p_t.set_write_timeout(std::declval<std::chrono::milliseconds>())
    } -> std::same_as<void>;

  {
    p_t.set_read_timeout(std::declval<std::chrono::milliseconds>())
    } -> std::same_as<void>;

  { p_t.get_write_timeout() } -> std::same_as<std::chrono::milliseconds>;
  { p_t.get_read_timeout() } -> std::same_as<std::chrono::milliseconds>;
};

template <typename t, typename t_byte>
concept connection_c = requires(t p_t) {
  requires one_byte_c<t_byte>;

  typename t::events_published;

  requires std::same_as<
      typename t::events_published,
      std::tuple<connection_error_unidentified_e, connection_error_writing_e,
                 connection_error_reading_e, connection_error_timeout_writing_e,
                 connection_error_timeout_reading_e>>;
  {
    p_t.write(std::declval<const t_byte *>(), std::declval<const t_byte *>())
    } -> std::same_as<communication_status_t>;

  {
    p_t.read(std::declval<t_byte *>(), std::declval<t_byte *>(),
             std::declval<t_byte **>())
    } -> std::same_as<communication_status_t>;

  {
    p_t.set_write_timeout(std::declval<std::chrono::milliseconds>())
    } -> std::same_as<void>;

  {
    p_t.set_read_timeout(std::declval<std::chrono::milliseconds>())
    } -> std::same_as<void>;

  { p_t.get_write_timeout() } -> std::same_as<std::chrono::milliseconds>;
  { p_t.get_read_timeout() } -> std::same_as<std::chrono::milliseconds>;
};

template <
    dispatcher_c<connection_error_unidentified_e, connection_error_writing_e,
                 connection_error_reading_e, connection_error_timeout_writing_e,
                 connection_error_timeout_reading_e, connection_lost_e>
        t_dispatcher>
class connection_a {
public:
  using events_published =
      std::tuple<connection_error_unidentified_e, connection_error_writing_e,
                 connection_error_reading_e, connection_error_timeout_writing_e,
                 connection_error_timeout_reading_e>;

  using events_subscribed = std::tuple<connection_lost_e>;

  connection_a() = delete;
  connection_a(connection_a &) = delete;
  connection_a(connection_a &&) = default;
  virtual ~connection_a() = default;

  connection_a(t_dispatcher::ptr p_dispatcher)
      : m_id(), m_dispatcher(p_dispatcher) {
    m_dispatcher->template subscribe<connection_a, connection_lost_e>(
        [&](auto p_event) { on_connection_lost(p_event.connection_id); });
  }

  connection_a &operator=(connection_a &) = delete;
  connection_a &operator=(connection_a &&) = default;

  bool operator>(const connection_a &p_connection) const = delete;

  bool operator>=(const connection_a &p_connection) const = delete;

  bool operator<=(const connection_a &p_connection) const {
    return p_connection.m_id <= m_id;
  }

  bool operator<(const connection_a &p_connection) const {
    return p_connection.m_id < m_id;
  }

  bool operator==(const connection_a &p_connection) const {
    return p_connection.m_id == m_id;
  }

  bool operator!=(const connection_a &p_connection) const {
    return p_connection.m_id != m_id;
  }

  template <tlc::convertible_to_milli t_timeout>
  void set_write_timeout(t_timeout p_timeout) {
    m_write_timeout =
        std::chrono::duration_cast<std::chrono::milliseconds>(p_timeout);
    do_set_write_timeout(m_write_timeout);
  }

  template <tlc::convertible_to_milli t_timeout>
  void set_read_timeout(t_timeout p_timeout) {
    m_read_timeout =
        std::chrono::duration_cast<std::chrono::milliseconds>(p_timeout);
    do_set_read_timeout(m_read_timeout);
  }

  std::chrono::milliseconds get_write_timeout() const {
    return m_write_timeout;
  }

  std::chrono::milliseconds get_read_timeout() const { return m_read_timeout; }

  template <one_byte_c t_byte>
  communication_status_t write(const t_byte *p_begin, const t_byte *p_end) {
    auto _begin(reinterpret_cast<const uint8_t *>(p_begin));
    auto _end(reinterpret_cast<const uint8_t *>(p_end));
    auto _status(do_write(_begin, _end));

    if (_status == communication_status_t::ERROR_WRITING) {
      m_dispatcher->template publish<connection_a, connection_error_writing_e>(
          id());
    } else if (_status == communication_status_t::ERROR_TIMEOUT_WRITING) {
      m_dispatcher
          ->template publish<connection_a, connection_error_timeout_writing_e>(
              id());
    } else if (_status == communication_status_t::ERROR_UNINDENTIFIED) {
      m_dispatcher
          ->template publish<connection_a, connection_error_unidentified_e>();
    }
    //   break;
    // default:
    //   TNCT_LOG_ERR(_status);
    // }

    return _status;

    // \todo handle connection status
  }

  template <one_byte_c t_byte>
  communication_status_t read(t_byte *p_begin, t_byte *p_end, t_byte **p_read) {
    auto _begin(reinterpret_cast<uint8_t *>(p_begin));
    auto _end(reinterpret_cast<uint8_t *>(p_end));
    auto _read(reinterpret_cast<uint8_t **>(p_read));
    auto _status(do_read(_begin, _end, _read));

#ifdef TENACITAS_LOG
    TNCT_LOG_INF(_status);
#endif
    // \todo handle connection status

    return _status;
  }

  tld::id id() const { return m_id; }

protected:
  virtual communication_status_t do_write(const uint8_t *, const uint8_t *) = 0;
  virtual communication_status_t do_read(uint8_t *, uint8_t *, uint8_t **) = 0;
  virtual void do_set_write_timeout(std::chrono::milliseconds) = 0;
  virtual void do_set_read_timeout(std::chrono::milliseconds) = 0;

private:
  void on_connection_lost(id /*p_connection_id*/) {}

private:
  tld::id m_id;
  t_dispatcher::ptr m_dispatcher;
  std::chrono::milliseconds m_write_timeout{0ms};
  std::chrono::milliseconds m_read_timeout{0ms};
};

struct connection_manager_a {};

// template <typename t_byte> struct new_connection_e {
//   new_connection_e(const t_connection &p_connection)
//       : connection(p_connection) {}
//   t_connection connection;
// };

class unix_socket_a {
public:
  unix_socket_a(int p_socket) : m_socket(p_socket) {}

  communication_status_t write(const uint8_t * /*p_begin*/,
                               const uint8_t * /*p_end*/);

  communication_status_t read(uint8_t * /*p_begin*/, uint8_t * /*p_end*/,
                              uint8_t ** /*p_read*/);

  void set_write_timeout(std::chrono::milliseconds /*p_timeout*/);
  void set_read_timeout(std::chrono::milliseconds /*p_timeout*/);
  std::chrono::milliseconds get_write_timeout() const {
    return m_write_timeout;
  }
  std::chrono::milliseconds get_read_timeout() const { return m_read_timeout; }

private:
  int m_socket;
  std::chrono::milliseconds m_write_timeout;
  std::chrono::milliseconds m_read_timeout;
};

// template <typename t_byte, connection_c<t_byte> t_connection>
// struct new_connection_e {
//   new_connection_e(const t_connection &p_connection)
//       : connection(p_connection) {}
//   t_connection connection;
// };

// template <typename t_byte, connection_c<t_byte> t_connection>
// struct connection_lost_e {
//   connection_lost_e(const t_connection &p_connection)
//       : connection(p_connection) {}
//   t_connection connection;
// };

// template <typename t_byte, connection_c<t_byte> t_connection>
// struct connection_reconnected_e {
//   connection_reconnected_e(const t_connection &p_connection)
//       : connection(p_connection) {}
//   t_connection connection;
// };

} // namespace tnct::lib

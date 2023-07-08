#ifndef TENACITAS_LIB_IPC_READER_H
#define TENACITAS_LIB_IPC_READER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.c

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <vector>

#include <tenacitas.lib/src/async/alg/dispatcher.h>
//#include <tenacitas.lib/src/async/typ/receiver.h>
#include <tenacitas.lib/src/ipc/evt/events.h>
#include <tenacitas.lib/src/ipc/typ/types.h>
#include <tenacitas.lib/src/log/alg/logger.h>

namespace tenacitas::lib::socket::alg {

namespace internal {

typ::status handler_reading_error(uint8_t p_max_read_retries,
                                  uint8_t &p_read_tries) {
  typ::status _status{typ::status::OK};

  if (errno == EINTR) {
    if (p_read_tries < p_max_read_retries) {
      ++p_read_tries;
    } else {

      _status = typ::status::ERROR_MAX_READING;
    }
  } else {
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("errno = ", errno, ": '", strerror(errno), '\'');
#endif
    return typ::status::ERROR_IO;
  }

  return _status;
}

typ::status handle_status_not_ok(typ::status p_original_status,
                                 uint8_t p_max_read_retries,
                                 uint8_t &p_read_retries) {
  if ((p_original_status == typ::status::ERROR_TIMEOUT_READING) ||
      (p_original_status == typ::status::CONNECTION_CLOSED) ||
      (p_original_status == typ::status::ERROR_NOT_CONNECTED)) {
    return p_original_status;
  }
  typ::status _status =
      internal::handler_reading_error(p_max_read_retries, p_read_retries);
#ifdef TENACITAS_LOG
  TNCT_LOG_DEB("read tries = ", static_cast<uint16_t>(p_read_retries),
               ", max retries = ", static_cast<uint16_t>(p_max_read_retries));
#endif
  if (_status != typ::status::OK) {
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("status handling = ", _status);
#endif
    return _status;
  }
  return typ::status::OK;
}

struct receiver {
  receiver() = delete;

  template <typename t_timeout = std::chrono::milliseconds>
  receiver(t_timeout p_timeout = 0ms, uint8_t p_max_read_retries = 10)
      : m_timeout(std::chrono::duration_cast<decltype(m_timeout)>(p_timeout)),
        m_max_read_retries(p_max_read_retries) {}

  template <typename t_size>
  typ::status operator()(typ::read &p_read, std::byte *p_buffer,
                         t_size p_buffer_size) {
    uint8_t _read_retries{0};

    typ::status _status{typ::status::OK};

    size_t _left{static_cast<size_t>(p_buffer_size)};

    std::byte *_current{p_buffer};

    const auto _timeout{
        std::chrono::duration_cast<std::chrono::milliseconds>(m_timeout)};

    while (_left > 0) {
      std::pair<typ::status, ssize_t> _result =
          p_read(_current, _left, _timeout);

      if (_result.first == typ::status::OK) {
        _current += _result.second;
        _left -= _result.second;
      } else {
        _status = internal::handle_status_not_ok(
            _result.first, m_max_read_retries, _read_retries);
        if (_status != typ::status::OK) {
#ifdef TENACITAS_LOG
          TNCT_LOG_DEB("status = ", _status);
#endif
          break;
        }
      }
    }

    return _status;
  }

private:
  std::chrono::microseconds m_timeout;
  uint8_t m_max_read_retries;
};
} // namespace internal

template <typ::message_size t_message_size, size_t t_size> struct receiver_t;

/// \brief fixed message size
///
/// \tparam t_size is the size of the message
template <size_t t_size> struct receiver_t<typ::message_size::FIXED, t_size> {
  //  using buffer = std::array<std::byte, t_size>;

  receiver_t() = default;
  receiver_t(const receiver_t &) = delete;
  receiver_t(receiver_t &&) = default;

  template <typename t_timeout = std::chrono::milliseconds>
  receiver_t(t_timeout p_timeout = 0ms, uint8_t p_max_read_retries = 10)
      : m_timeout(std::chrono::duration_cast<decltype(m_timeout)>(p_timeout)),
        m_max_read_retries(p_max_read_retries) {}

  receiver_t &operator=(const receiver_t &) = delete;
  receiver_t &operator=(receiver_t &&) = default;

  template <typename t_connection>
  std::optional<async::typ::message> operator()(t_connection &p_connection) {

    async::typ::message _message;

    internal::receiver _read(m_timeout, m_max_read_retries);

    typ::status _status{_read(p_connection, &_message[0], t_size)};

    return (_status == typ::status::OK
                ? std::optional<async::typ::message>{std::move(_message)}
                : std::optional<async::typ::message>{});
  }

private:
  std::chrono::microseconds m_timeout{0ms};

  const uint8_t m_max_read_retries{10};
};

/// \brief unknown message size,  which is limited by a sequence of bytes
///
/// \tparam t_size is the size of the internal buffer used to read the message
template <size_t t_size>
struct receiver_t<typ::message_size::DELIMITED, t_size> {

  receiver_t() = default;
  receiver_t(const receiver_t &) = delete;
  receiver_t(receiver_t &&) = default;

  template <typename t_timeout = std::chrono::milliseconds>
  receiver_t(t_timeout p_timeout = 0ms,
             std::initializer_list<std::byte> p_delimeters = {std::byte{'\r'},
                                                              std::byte{'\n'}},
             uint8_t p_max_read_retries = 10)
      : m_timeout(std::chrono::duration_cast<decltype(m_timeout)>(p_timeout)),
        m_delimeters(std::move(p_delimeters)),
        m_max_read_retries(p_max_read_retries) {}

  receiver_t &operator=(const receiver_t &) = delete;
  receiver_t &operator=(receiver_t &&) = default;

  template <typename t_connection>
  std::optional<async::typ::message> operator()(t_connection &p_connection) {
    async::typ::message _message;
    using iterator = async::typ::message::iterator;

    typ::status _status{typ::status::OK};

    uint8_t _read_tries{0};

    while (true) {
      std::pair<typ::status, ssize_t> _result =
          p_connection.read(m_buffer.begin(), m_buffer.size(), m_timeout);

      if (_result.first == typ::status::OK) {
        typename internal_buffer::iterator _ite =
            std::find_first_of(m_buffer.begin(), m_buffer.end(),
                               m_delimeters.begin(), m_delimeters.end());

        if (_ite != m_buffer.end()) {
          iterator _end{_message.end()};
          _message.insert(_end, m_buffer.begin(), _ite);
          break;
        } else if (_result.second > 0) {
          iterator _end{_message.end()};
          _message.insert(_end, m_buffer.begin(),
                          std::next(m_buffer.begin(), _result.second));
#ifdef TENACITAS_LOG
          TNCT_LOG_DEB("buffer read so far = ",
                       async::typ::msg2str(_message.begin(), _message.end()));
#endif
        }
      } else {
        _status = internal::handle_status_not_ok(
            _result.first, m_max_read_retries, _read_tries);
        if (_status != typ::status::OK) {
          break;
        }
      }
    }

    if (_message.empty()) {
      return {};
    }

    return {std::move(_message)};
  }

private:
  using internal_buffer = std::array<std::byte, t_size>;

private:
  std::chrono::microseconds m_timeout{0ms};
  std::vector<std::byte> m_delimeters{std::byte{'\r'}, std::byte{'\n'}};
  const uint8_t m_max_read_retries{10};
  internal_buffer m_buffer;
};

//// MAX_SIZE
// template <> struct reader<typ::message_type::max_size> {
//  using buffer = std::vector<std::byte>;

//  reader() = delete;
//  reader(async::alg::dispatcher::ptr p_dispatcher, size_t p_max_buffer_size,
//         std::initializer_list<std::byte> p_delimeters = {std::byte{'\r'},
//                                                          std::byte{'\n'}},
//         uint8_t p_max_read_retries = 10)
//      : m_dispatcher(p_dispatcher), m_buffer(p_max_buffer_size, std::byte{0}),
//        m_delimeters(p_delimeters.begin(), p_delimeters.end()),
//        m_last(&m_buffer[0]), m_first(&m_buffer[0]),
//        m_end(&m_buffer[p_max_buffer_size]),
//        m_max_read_retries(p_max_read_retries) {}

//  template <typename t_connection,
//            typename t_timeout = std::chrono::milliseconds>
//  std::pair<typ::status, buffer> operator()(t_connection &p_connection,
//                                            t_timeout p_timeout = 0ms) {
//    if (m_last == m_first) {
//      return {read(p_connection, p_timeout, m_buffer.size())};
//    } else {
//      auto _maybe{
//          delimeted(m_first, m_last, m_delimeters.begin(),
//          m_delimeters.end())};
//      if (_maybe) {
//        return {typ::status::OK, std::move(_maybe.value())};
//      }
//      return {
//          read(p_connection, p_timeout, static_cast<size_t>(m_end - m_last))};
//    }
//  }

// private:
//  template <typename t_buffer_iterator, typename t_delimeter_iterator>
//  std::optional<buffer> delimeted(t_buffer_iterator &p_begin_buffer,
//                                  t_buffer_iterator &p_end_buffer,
//                                  t_delimeter_iterator p_begin_delimeter,
//                                  t_delimeter_iterator p_end_delimeter) {

//    std::byte *_ite = std::find_first_of(p_begin_buffer, p_end_buffer,
//                                         p_begin_delimeter, p_end_delimeter);
//    if (_ite == p_end_buffer) {
//      return {};
//    }

//    //_delimeter_found = true;
//    buffer _result{p_begin_buffer, _ite};

//#ifdef TENACITAS_LOG
//    TNCT_LOG_DEB("delimeter found: ",
//        typ::buffer2string(_result.begin(), _result.end()));
//#endif
//    _ite += std::distance(p_begin_delimeter, p_end_delimeter);
//    if (_ite >= p_end_buffer) {
//#ifdef TENACITAS_LOG
//      TNCT_LOG_DEB(static_cast<uint16_t>(*_ite),
//          " == ", static_cast<uint16_t>(*p_end_buffer));
//#endif
//      p_end_buffer = p_begin_buffer;
//    } else {
//      std::copy(_ite, p_end_buffer, p_begin_buffer);
//      p_end_buffer = p_begin_buffer + std::distance(_ite, p_end_buffer);

//#ifdef TENACITAS_LOG
//      TNCT_LOG_DEB("internal buffer reconfigured: ",
//          typ::buffer2string(p_begin_buffer, p_end_buffer));
//#endif
//    }
//    return {std::move(_result)};
//  }

//  template <typename t_connection, typename t_timeout>
//  std::pair<typ::status, buffer> read(t_connection &p_connection,
//                                      t_timeout p_timeout, size_t p_left) {
//    typ::status _status{typ::status::OK};
//    uint8_t _read_tries{0};
//    while (true) {

//      std::optional<buffer> _maybe{
//          delimeted(m_first, m_last, m_delimeters.begin(),
//          m_delimeters.end())};
//      if (_maybe) {
//#ifdef TENACITAS_LOG
//        TNCT_LOG_DEB("delimeter found");
//#endif
//        return {typ::status::OK, std::move(_maybe.value())};
//      }

//#ifdef TENACITAS_LOG
//      TNCT_LOG_DEB("delimeter not found");
//#endif
//      if (p_left <= 0) {
//        break;
//      }
//      std::pair<typ::status, ssize_t> _result =
//          p_connection.read(m_last, p_left, p_timeout);

//#ifdef TENACITAS_LOG
//      TNCT_LOG_DEB("connection read status = ", _result.first);
//#endif

//      if (_result.first == typ::status::OK) {
//#ifdef TENACITAS_LOG
//        TNCT_LOG_DEB("read ", _result.second, " bytes");
//#endif
//        m_last += _result.second;
//        p_left -= _result.second;
//      } else {
//        _status = internal::handle_status_not_ok(
//            m_dispatcher, _result.first, p_connection, m_max_read_retries,
//            _read_tries);
//        if (_status != typ::status::OK) {
//          break;
//        }
//      }
//    }
//    return {_status, {}};
//  }

// private:
//  async::alg::dispatcher::ptr m_dispatcher;
//  buffer m_buffer;
//  std::vector<std::byte> m_delimeters;
//  std::byte *m_last{nullptr};
//  std::byte *m_first{nullptr};
//  std::byte *m_end{nullptr};
//  const uint8_t m_max_read_retries;
//};

// template <size_t t_buffer_size>
// struct reader<typ::message_type::multiple, t_buffer_size> {
//  using buffer = typename typ::read_traits<typ::message_type::multiple,
//                                           t_buffer_size>::buffer;

//  reader(async::alg::dispatcher::ptr p_dispatcher,
//         std::initializer_list<std::byte> p_delimeters = {std::byte{'\r'},
//                                                          std::byte{'\n'}},
//         uint8_t p_max_read_retries = 10)
//      : m_dispatcher(p_dispatcher), m_delimeters(p_delimeters),
//        m_max_read_retries(p_max_read_retries) {}

//  template <typename t_connection, typename t_timeout>
//  std::optional<buffer> operator()(t_connection &p_connection,
//                                   t_timeout p_timeout);

// private:
//  async::alg::dispatcher::ptr m_dispatcher;
//  std::vector<std::byte> m_delimeters;
//  const uint8_t m_max_read_retries;
//};

} // namespace tenacitas::lib::socket::alg

#endif

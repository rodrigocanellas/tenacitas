#ifndef TENACITAS_REMOTE_H
#define TENACITAS_REMOTE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <array>
#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include <tenacitas.lib/async.h>

using namespace std::chrono_literals;

/// \brief master namespace
namespace tenacitas {

/// \brief
namespace remote {

struct message {
    message(std::string &&p_str)
        : str(std ::move(p_str)) {}

    message(const char *p_str)
        : str(p_str) {}

    std::string str;
};

template <typename t_connection>
struct new_connection {
    new_connection(t_connection &&p_connection)
        : connection(std::move(p_connection)) {}

    friend std::ostream &operator<<(std::ostream &p_out, new_connection &&) {
        p_out << "incoming";
        return p_out;
    }

    t_connection connection;
};

// template <typename t_connection, typename t_message_handler>
// struct new_connection_handler {

//    new_connection_handler(t_message_handler &&p_message_handler)
//        : m_message_handler(std::move(p_message_handler)) {}

//    void operator()(std::shared_ptr<bool> /*p_timeout*/,
//                    new_connection<t_connection> &&p_new_connection) {
//        m_message_handler(p_new_connection.connection);
//    }

// private:
//    t_message_handler m_message_handler;
//};

template <typename t_connector> /*, typename t_message_handler>*/
struct server {

    typedef typename t_connector::connection connection;

    //    template <typename t_time>
    //    server(uint16_t p_num_handlers, t_time p_timeout) {
    //        async::handling_id _handling_id =
    //            async::add_handling<new_connection<connection>>(p_timeout);
    //        for (uint16_t _i = 0; _i < p_num_handlers; ++_i) {
    //            async::add_handler(
    //                _handling_id,
    //                new_connection_handler<connection, t_message_handler> {
    //                    t_message_handler {}});
    //        }
    //    }

    void start_sync(t_connector p_connector) {
        while (true) {
            connection _connection = p_connector();
            async::dispatch(new_connection<connection> {_connection});
        }
    }
};

#ifdef POSIX
    #include <unistd.h>

template <uint16_t t_max_size>
struct udp_reader {

    udp_reader(int p_socket)
        : m_socket(p_socket) {}

    std::optional<message> read() {
        std::string _buf {t_max_size};
        if (read(m_socket, &_buf[0], t_max_size) < 0) {
            async::dispatch(async::reader_t<message>::error_reading {});
            return {};
        }
        return {message {std::move(_buf)}};
    }

private:
    int m_socket;
};

template <uint16_t t_size>
struct tcp_reader_stream {

    tcp_reader_stream(int p_socket)
        : m_socket(p_socket) {}

    void read() {
        uint16_t _amount_read {0};
        char _buf[t_size + 1];

        while (true) {

            memset(_buf, '\0', t_size);
            _amount_read = read(m_socket, _buf, t_size);

            if (_amount_read == 0) {
                async::dispatch(async::reader_t<message>::all_data_read {});
                break;
            }
            if (_amount_read < 0) {
                async::dispatch(async::reader_t<message>::error_reading {});
                break;
            }
            async::dispatch(
                async::reader_t<message>::data_read {std::string {_buf}});
        }
    }

private:
    int m_socket;
};

template <uint16_t t_max_size>
struct tcp_reader_record {

    tcp_reader_record(int p_socket)
        : m_socket(p_socket) {}

    std::optional<message> read() {
        uint16_t _total_read {0};

        char _result[t_max_size + 1];
        memset(_result, '\0', t_max_size);

        char _buf[t_max_size + 1];

        while (true) {
            memset(_buf, '\0', t_max_size);
            uint16_t _amount_read = read(m_socket, _result, t_max_size);

            if (_amount_read == 0) {
                break;
            }

            if (_amount_read < 0) {
                async::dispatch(async::reader_t<message>::error_reading {});
                return {};
            }

            if ((_total_read + _amount_read) > t_max_size) {
                async::dispatch(async::reader_t<message>::error_reading {});
                return {};
            }

            std::copy(&_buf[0], &_buf[strlen(_buf)], &_result[_total_read]);
            _total_read += _amount_read;

            if (_total_read == t_max_size) {
                break;
            }
        }
        return {message {_result}};
    }

private:
    int m_socket;
};

#endif

} // namespace remote
} // namespace tenacitas

#endif

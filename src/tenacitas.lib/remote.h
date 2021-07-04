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
#include <tenacitas.lib/logger.h>

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

enum class reading : char { RECORD = 'r', STREAM = 's' };
std::ostream &operator<<(std::ostream &p_out, const reading &p_reading) {
    if (p_reading == reading::RECORD) {
        p_out << "record";
    } else if (p_reading == reading::STREAM) {
        p_out << "stream";
    }
    return p_out;
}

enum class protocol : uint16_t { TCP = 0, UDP, BLUETOOTH };
std::ostream &operator<<(std::ostream &p_out, const protocol &p_protocol) {
    if (p_protocol == protocol::BLUETOOTH) {
        p_out << "bluetooth";
    } else if (p_protocol == protocol::TCP) {
        p_out << "tcp";
    } else if (p_protocol == protocol::UDP) {
        p_out << "udp";
    }
    return p_out;
}

const char *protocol2str(protocol p_protocol) {
    static constexpr char g_bluetooth[] = "bluetooth";
    static constexpr char g_tcp[] = "tcp";
    static constexpr char g_udp[] = "udp";
    static constexpr char g_never[] = "";
    if (p_protocol == protocol::BLUETOOTH) {
        return g_bluetooth;
    } else if (p_protocol == protocol::TCP) {
        return g_tcp;
    } else if (p_protocol == protocol::UDP) {
        return g_udp;
    }
    return g_never;
}

template <protocol protocol_type, reading reading_type, uint16_t buffer_size>
struct reader;

template <protocol protocol_type>
struct passive_connector;

template <protocol protocol_type>
struct active_connector;

#ifdef POSIX
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>

    #ifndef INADDR_NONE
        #define INADDR_NONE 0xFFFFFFFF
    #endif

template <uint16_t buffer_size>
struct reader<protocol::UDP, reading::RECORD, buffer_size> {
    std::optional<message> operator()(int p_socket) {

        std::string _buf {buffer_size};
        if (read(p_socket, &_buf[0], buffer_size) < 0) {
            async::dispatch(async::reader_t<message>::error_reading {});
            return {};
        }
        return {message {std::move(_buf)}};
    }
};

template <uint16_t buffer_size>
struct reader<protocol::TCP, reading::RECORD, buffer_size> {
    std::optional<message> operator()(int p_socket) {
        uint16_t _total_read {0};

        char _result[buffer_size + 1];
        memset(_result, '\0', buffer_size);

        char _buf[buffer_size + 1];

        while (true) {
            memset(_buf, '\0', buffer_size);
            uint16_t _amount_read = read(p_socket, _result, buffer_size);

            if (_amount_read == 0) {
                break;
            }

            if (_amount_read < 0) {
                async::dispatch(async::reader_t<message>::error_reading {});
                return {};
            }

            if ((_total_read + _amount_read) > buffer_size) {
                async::dispatch(async::reader_t<message>::error_reading {});
                return {};
            }

            std::copy(&_buf[0], &_buf[strlen(_buf)], &_result[_total_read]);
            _total_read += _amount_read;

            if (_total_read == buffer_size) {
                break;
            }
        }
        return {message {_result}};
    }
};

template <uint16_t buffer_size>
struct reader<protocol::TCP, reading::STREAM, buffer_size> {
    void tcp_reader_stream(int p_socket) {
        uint16_t _amount_read {0};
        char _buf[buffer_size + 1];

        while (true) {

            memset(_buf, '\0', buffer_size);
            _amount_read = read(p_socket, _buf, buffer_size);

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
};

template <typename t_number>
struct sockaddr_in calculate_port(const char * /*p_protocol_str*/,
                                  t_number p_port) {
    struct sockaddr_in _in;
    memset(&_in, 0, sizeof(_in));
    _in.sin_family = AF_INET;
    _in.sin_addr.s_addr = INADDR_ANY;

    _in.sin_port = htons(ntohs(static_cast<unsigned short>(p_port)));
    if (_in.sin_port == 0) {
        std::string _str {"could not map '" + std::to_string(p_port) +
                          "' to a valid port"};
        ERR(_str);
        throw std::runtime_error(_str);
    }
    return _in;
}

struct sockaddr_in calculate_port(const char *p_protocol_str,
                                  const char *p_service) {
    struct servent *_se = nullptr;
    _se = getservbyname(p_service, p_protocol_str);
    if (!_se) {
        std::string _str {"could not map '" + std::string(p_service) +
                          "' to a valid port"};
        ERR(_str);
        throw std::runtime_error(_str);
    }

    struct sockaddr_in _in;
    memset(&_in, 0, sizeof(_in));
    _in.sin_family = AF_INET;
    _in.sin_addr.s_addr = INADDR_ANY;
    _in.sin_port = htons(ntohs(static_cast<unsigned short>(_se->s_port)));
    return _in;
}

int calculate_protocol_entry(const char *p_protocol_str) {
    struct protoent *_pe = getprotobyname(p_protocol_str);
    if (_pe == nullptr) {
        std::string _str {"could not map " +
                          std::string(protocol2str(protocol::TCP)) +
                          " to a valid protocol number"};
        ERR(_str);
        throw std::runtime_error(_str);
    }
    return _pe->p_proto;
}

template <protocol protocol_type>
int calculate_socket_type() {
    if (protocol_type == protocol::UDP) {
        return SOCK_DGRAM;
    }
    if (protocol_type == protocol::TCP) {
        return SOCK_STREAM;
    }
    std::string _str {"could not calculate a valid protocol type for " +
                      std::string(protocol2str(protocol_type))};
    ERR(_str);
    throw std::runtime_error(_str);
}

int allocate_socket(int p_socket_type, int p_protocol_type) {
    return socket(PF_INET, p_socket_type, p_protocol_type);
}

void bind_socket_to_port(int p_socket, struct sockaddr_in *p_port) {
    if (bind(p_socket, (struct sockaddr *)p_port, sizeof(struct sockaddr_in)) <
        0) {
        std::string _str {"could not bind socket"};
        ERR(_str);
        throw std::runtime_error(_str);
    }
}

template <typename t_host>
void calculate_host(struct sockaddr_in &p_addr, t_host p_host) {

    struct hostent *_he = gethostbyname(p_host);
    if (_he) {
        memcpy(&p_addr.sin_addr, _he->h_addr_list[0], _he->h_length);
    } else {
        p_addr.sin_addr.s_addr = inet_addr(p_host);
        if (p_addr.sin_addr.s_addr == INADDR_NONE) {
            std::stringstream _stream;
            _stream << "it was not possibe to translate " << p_host
                    << " into a IP addredd";
            std::string _str {_stream.str()};
            ERR(_str);
            throw std::runtime_error(_str);
        }
    }
}

template <protocol protocol_type, typename t_service>
int create_socket(t_service p_service) {
    const char *_protocol_str = protocol2str(protocol_type);

    struct sockaddr_in _in = calculate_port(_protocol_str, p_service);

    int _protocol_entry = calculate_protocol_entry(_protocol_str);

    int _socket_type = calculate_socket_type<protocol_type>();

    int _socket = allocate_socket(_socket_type, _protocol_entry);

    bind_socket_to_port(_socket, &_in);

    return _socket;
}

template <protocol protocol_type, typename t_host, typename t_service>
int create_socket(t_host p_host, t_service p_service) {
    const char *_protocol_str = protocol2str(protocol_type);

    struct sockaddr_in _in = calculate_port(_protocol_str, p_service);

    calculate_host(_in, p_host);

    int _protocol_entry = calculate_protocol_entry(_protocol_str);

    int _socket_type = calculate_socket_type<protocol_type>();

    int _socket = allocate_socket(_socket_type, _protocol_entry);

    if (connect(_socket, (struct sockaddr *)&_in, sizeof(_in)) < 0) {
        std::stringstream _stream;
        _stream << "it was not possible to connect to " << p_host << ","
                << p_service;
        const std::string _str {_stream.str()};
        ERR(_str);
        throw std::runtime_error(_str);
    }

    return _socket;
}

template <>
struct active_connector<protocol::TCP> {
    typedef int connection;

    template <typename t_host, typename t_service>
    std::optional<connection> operator()(t_host p_host, t_service p_service) {
        try {
            return {create_socket<protocol::TCP>(p_host, p_service)};
        } catch (std::exception &_ex) {
            FAT(_ex.what());
        }
        return {};
    }
};

template <>
struct active_connector<protocol::UDP> {
    typedef int connection;

    template <typename t_host, typename t_service>
    std::optional<connection> operator()(t_host p_host, t_service p_service) {
        try {
            return {create_socket<protocol::UDP>(p_host, p_service)};
        } catch (std::exception &_ex) {
            FAT(_ex.what());
        }
        return {};
    }
};

template <>
struct passive_connector<protocol::TCP> {

    typedef int connection;

    template <typename t_service>
    passive_connector(t_service p_service) {

        m_master_socket = create_socket<protocol::TCP>(p_service);

        if (listen(m_master_socket, 64) < 0) {
            std::string _str {"error seting the socket to listen"};
            ERR(_str);
            throw std::runtime_error(_str);
        }
    }

    std::optional<connection> operator()() {
        struct sockaddr_in _client;
        unsigned int _len = 0;
        memset(&_client, 0, sizeof(_client));
        TRA("accepting...");
        connection _conn =
            accept(m_master_socket, (struct sockaddr *)&_client, &_len);
        if (_conn < 0) {
            ERR("error accepting connection");
            return {};
        }
        return {_conn};
    }

private:
    connection m_master_socket;
};

template <>
struct passive_connector<protocol::UDP> {

    typedef int connection;

    template <typename t_service>
    passive_connector(t_service p_service);

    connection operator()() { return -1; }
};

#endif // POSIX

template <typename t_connection>
struct new_connection {
    new_connection(t_connection &&p_connection)
        : connection(std::move(p_connection)) {}

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const new_connection &) {
        p_out << "new connection";
        return p_out;
    }

    t_connection connection;
};

template <typename t_connection, typename t_message_handler>
struct new_connection_handler {

    new_connection_handler(t_message_handler &&p_message_handler)
        : m_message_handler(std::move(p_message_handler)) {}

    void operator()(std::shared_ptr<bool> /*p_timeout*/,
                    new_connection<t_connection> &&p_new_connection) {
        m_message_handler(p_new_connection.connection);
    }

private:
    t_message_handler m_message_handler;
};

template <protocol protocol_type>
struct server {

    typedef passive_connector<protocol_type> connector;
    typedef typename connector::connection connection;

    //    template <typename t_time>
    //    server(uint16_t p_num_handlers,
    //           t_message_handler_factory &p_message_handler_factory,
    //           t_time p_timeout) {
    //        async::handling_id _handling_id =
    //            async::add_handling<new_connection<connection>>(p_timeout);
    //        for (uint16_t _i = 0; _i < p_num_handlers; ++_i) {
    //            async::add_handler(
    //                _handling_id,
    //                new_connection_handler<connection, message_handler> {
    //                    p_message_handler_factory()});
    //        }
    //    }

    template <typename t_service>
    void start_sync(t_service p_service) noexcept(false) {
        passive_connector<protocol_type> _connector(p_service);
        try {
            TRA("starting sync");

            while (true) {
                std::optional<connection> _maybe = _connector();
                if (_maybe) {
                    INF("connection established");
                    async::dispatch(
                        new_connection<connection> {std::move(*_maybe)});
                }
            }

        } catch (std::exception &_ex) {
            ERR(_ex.what());
        }
    }

private:
};

template <protocol protocol_type>
struct client {
    typedef active_connector<protocol_type> connector;
    typedef typename connector::connection connection;

    template <typename t_host, typename t_service>
    bool connect(t_host p_host, t_service p_service) {
        connector _connector;
        std::optional<connection> _maybe = _connector(p_host, p_service);
        if (_maybe) {
            m_connection = std::move(*_maybe);
            return true;
        }
        return false;
    }

private:
    connection m_connection;
};

} // namespace remote
} // namespace tenacitas

#endif

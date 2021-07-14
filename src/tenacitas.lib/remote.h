#ifndef TENACITAS_REMOTE_H
#define TENACITAS_REMOTE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
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

template <typename t_char_type>
struct message {

    typedef t_char_type type;

    typedef typename std::vector<type>::const_iterator const_iterator;

    message() = default;

    message(message &&p_msg) = default;
    message &operator=(message &&p_msg) = default;

    message(const message &p_msg) = default;
    message &operator=(const message &p_msg) = default;

    inline message(const char *p_buf, size_t p_size)
        : m_buf(reinterpret_cast<const type *>(p_buf),
                reinterpret_cast<const type *>(p_buf) + p_size) {}

    inline message(const char *p_begin, const char *p_end)
        : m_buf(reinterpret_cast<const type *>(p_begin),
                reinterpret_cast<const type *>(p_end)) {}

    inline message(const std::string &p_str)
        : message(p_str.c_str(), p_str.size()) {}

    friend std::ostream &p(std::ostream &p_out, const message &p_msg) {
        for (message::const_iterator _ite = p_msg.begin(); _ite != p_msg.end();
             ++_ite) {
            p_out << static_cast<char>(*_ite);
        }
        return p_out;
    }

    operator const char *() const {
        return reinterpret_cast<const char *>(&m_buf[0]);
    }

    inline size_t size() const { return m_buf.size(); }

    const std::vector<type> &get() const { return m_buf; }

    const_iterator begin() const { return m_buf.begin(); }
    const_iterator end() const { return m_buf.end(); }

    //    iterator begin() { return m_buf.begin(); }
    //    iterator end() { return m_buf.end(); }

    //    type &operator[](size_t p_index) {
    //        if (p_index < length()) {
    //            return m_buf[p_index];
    //        }
    //        std::string _error {std::to_string(p_index) + " is greater than "
    //        +
    //                            std::to_string(length())};
    //        ERR(_error);
    //        throw std::out_of_range(_error);
    //    }

    const type &operator[](size_t p_index) {
        if (p_index <= size()) {
            return m_buf[p_index];
        }
        std::string _error {std::to_string(p_index) + " is greater than " +
                            std::to_string(size())};
        ERR(_error);
        throw std::out_of_range(_error);
    }

    inline void append(const message &p_msg) {
        std::copy(p_msg.m_buf.begin(), p_msg.m_buf.end(),
                  std::inserter(m_buf, m_buf.begin()));
    }

    inline void append(const char *p_buf, size_t p_size) {
        const type *_buf = reinterpret_cast<const type *>(p_buf);
        std::copy(_buf, _buf + p_size, std::inserter(m_buf, m_buf.begin()));
    }

    inline void append(const char *p_begin, const char *p_end) {
        const type *_begin = reinterpret_cast<const type *>(p_begin);
        const type *_end = reinterpret_cast<const type *>(p_end);
        std::copy(_begin, _end, std::inserter(m_buf, m_buf.begin()));
    }

private:
    std::vector<type> m_buf;
};

enum class reading : char {
    AT_MOST = 'a',
    EXACTLY = 'e',
    DELIMITED = 'd',
    CONTINUOUS = 'c'
};
std::ostream &operator<<(std::ostream &p_out, const reading &p_reading) {
    if (p_reading == reading::AT_MOST) {
        p_out << "fixed size";
    } else if (p_reading == reading::CONTINUOUS) {
        p_out << "stream";
    } else if (p_reading == reading::DELIMITED) {
        p_out << "delimited";
    } else if (p_reading == reading::EXACTLY) {
        p_out << "exactly";
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

template <typename t_connection,
          typename t_data,
          reading reading_type,
          size_t size = 4 * 1024>
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

static constexpr uint16_t MAX_WRITES_ATTEMPT {5};
static constexpr uint16_t MAX_READS_ATTEMPT {5};

bool write_buffer(int p_socket,
                  const char *p_str,
                  size_t p_len,
                  uint16_t p_attempt = 0) {
    if (p_attempt >= MAX_WRITES_ATTEMPT) {
        return false;
    }
    size_t _left {p_len};
    ssize_t _written {0};
    const char *_aux = p_str;

    while (_left > 0) {
        _written = write(p_socket, _aux, _left);
        if (_written < 0) {
            if (errno == EINTR) {
                return write_buffer(p_socket, p_str, p_len, ++p_attempt);
            }
            return false;
        }
        _left -= _written;
        _aux += _written;
    }
    return true;
}

ssize_t
read_buffer(int p_socket, char *p_buf, size_t p_len, uint16_t p_attempt = 0) {
    if (p_attempt >= MAX_READS_ATTEMPT) {
        ERR("maximum read attempts");
        return -1;
    }

    ssize_t _read {0};
    size_t _left {p_len};
    char *_aux = p_buf;

    while (_left > 0) {
        TRA("about to read");
        _read = read(p_socket, _aux, _left);
        TRA("read ", _read, " bytes");
        if (_read < 0) {
            if (errno == EINTR) {
                return read_buffer(p_socket, p_buf, p_len, ++p_attempt);
            }
            return -1;
        } else if (_read == 0) {
            if (_left != 0) {
                ERR("reading interrupt");
                return -1;
            }
            break;
        }
        TRA("read '", _aux, '\'');
        _left -= _read;
        _aux += _read;
    }
    return static_cast<size_t>(_aux - p_buf);
}

template <typename t_connection, typename t_data, size_t size>
struct reader<t_connection, t_data, reading::AT_MOST, size> {

    std::optional<message<t_data>> operator()(t_connection p_socket) {
        char _buf[size + 1];
        memset(_buf, '\0', size);
        ssize_t _amount = read(p_socket, _buf, size);
        if (_amount == -1) {
            ERR("error reading");
            return {};
        }
        if (_amount == 0) {
            INF("nothing read");
            return {};
        }
        return {message<t_data> {&_buf[0], &_buf[_amount]}};
    }
};

template <typename t_connection, typename t_data, size_t size>
struct reader<t_connection, t_data, reading::EXACTLY, size> {

    std::optional<message<t_data>> operator()(t_connection p_socket) {
        char _buf[size + 1];
        memset(_buf, '\0', size);
        ssize_t _amount = read_buffer(p_socket, _buf, size);
        if (_amount == -1) {
            ERR("error reading");
            return {};
        }
        if (_amount == 0) {
            INF("nothing read");
            return {};
        }
        if (_amount != size) {
            ERR("read ", _amount, ", but ", size, " was expected");
            return {};
        }
        return {message<t_data> {&_buf[0], &_buf[_amount]}};
    }
};

template <typename t_connection, typename t_data, size_t size>
struct reader<t_connection, t_data, reading::DELIMITED, size> {
    std::optional<message<t_data>> operator()(t_connection p_socket,
                                              char *p_delimeter,
                                              size_t p_delimeter_size) {
        message<t_data> _msg;
        char _buf[size + 1];
        while (true) {
            memset(_buf, '\0', size);
            ssize_t _amount = read(p_socket, _buf, size);
            if (_amount == -1) {
                ERR("error reading");
                break;
            }
            if (_amount == 0) {
                INF("nothing read");
                return {};
            }
            TRA("read '", _buf, "', checking for delimeter");
            if (std::find_end(&_buf[0], &_buf[_amount], &p_delimeter[0],
                              &p_delimeter[p_delimeter_size]) !=
                &_buf[_amount]) {
                _msg.append(&_buf[0], &_buf[_amount - p_delimeter_size]);
                TRA("found, total message read '", _msg.get(), '\'');
                return _msg;
            }
            TRA("delimeter not found");
            _msg.append(&_buf[0], &_buf[_amount]);
            TRA("message read so far '", _msg.get(), '\'');
        }
        return {};
    }
};

template <typename t_connection, typename t_data, size_t size>
struct reader<t_connection, t_data, reading::CONTINUOUS, size> {
    void operator()(
        t_connection p_socket,
        std::function<void(int p_socket, message<t_data> &&)> p_handler) {

        char _buf[size + 1];
        memset(_buf, '\0', size);
        while (true) {
            ssize_t _amount = read(p_socket, _buf, size);
            if (_amount == -1) {
                ERR("error reading");
                break;
            }
            if (_amount == 0) {
                INF("nothing read");
                break;
            }
            TRA("read '", _buf, "', calling handler");
            p_handler(p_socket, message<t_data>(_buf, size));
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
        TRA("master socket = ", m_master_socket);
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

        TRA("connection accepted = ", _conn);
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

template <typename t_connection, typename t_char_type>
struct writer {

    bool operator()(t_connection p_connection,
                    const message<t_char_type> &p_msg) {
        TRA("writing '", p_msg, "' to socket ", p_connection);
        return write_buffer(p_connection, p_msg, p_msg.size());
    }
};

// struct new_text_msg {

//    message<char> msg;
//};

template <typename t_connection>
struct new_connection {
    new_connection() { TRA("new connection constructor"); }
    new_connection(t_connection &&p_connection)
        : connection(std::move(p_connection)) {}

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const new_connection &) {
        p_out << "'new connection'";
        return p_out;
    }

    t_connection connection;
};

// template <typename t_connection, typename t_message_handler>
// struct new_connection_handler {

//  new_connection_handler(t_message_handler &&p_message_handler)
//      : m_message_handler(std::move(p_message_handler)) {}

//  void operator()(std::shared_ptr<bool> /*p_timeout*/,
//                  new_connection<t_connection> &&p_new_connection) {
//    m_message_handler(p_new_connection.connection);
//  }

// private:
//  t_message_handler m_message_handler;
//};

template <protocol protocol_type>
struct server {

    typedef passive_connector<protocol_type> connector;
    typedef typename connector::connection connection;

    template <typename t_service>
    void start_sync(t_service p_service) noexcept(false) {

        try {
            passive_connector<protocol_type> _connector(p_service);
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
    std::optional<connection> connect(t_host p_host, t_service p_service) {
        connector _connector;
        std::optional<connection> _maybe = _connector(p_host, p_service);
        if (_maybe) {
            m_connection = std::move(*_maybe);
            return {m_connection};
        }
        return {};
    }

private:
    connection m_connection;
};

} // namespace remote
} // namespace tenacitas

#endif //  TENACITAS_REMOTE_H

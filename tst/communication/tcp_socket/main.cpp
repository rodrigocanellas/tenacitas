
#include <cstdint>
#include <iostream>
#include <string>

#include <communication/client.h>
#include <communication/internal/log.h>
#include <communication/status.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <resolv.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace tenacitas;

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {
/// \brief namespace for test
namespace tst {

struct tcp_socket {
  typedef tenacitas::logger::cerr::log logger;
  status open(const std::string &p_url) {
    char *_ip = new char(p_url.size() + 1);
    strcpy(_ip, p_url.c_str());
    comm_log_debug(logger, "connecting...");
    status _status = create_socket(_ip);
    delete _ip;
    if (_status == status::ok) {
      comm_log_debug(logger, "connected");
    } else {
      comm_log_error(logger, "not connected");
    }
    return _status;
  }

  status open(std::pair<std::string, int> p_ip_port) {
    comm_log_debug(logger, "connecting...");
    status _status =
        create_socket_ip(p_ip_port.first.c_str(), p_ip_port.second);
    if (_status == status::ok) {
      comm_log_debug(logger, "connected");
    } else {
      comm_log_error(logger, "not connected");
    }
    return _status;
  }

  status send(std::string::const_iterator p_begin,
              std::string::const_iterator p_end) {
    auto _sent = ::write(m_sockfd, &(*p_begin), std::distance(p_begin, p_end));

    if (_sent == -1) {
      comm_log_error(logger, "error '", std::strerror(errno),
                     "' while sending");
      return status::error_sending;
    }

    if (_sent != std::distance(p_begin, p_end)) {
      comm_log_error(logger, "error, ", _sent, " were sent, but ",
                     std::distance(p_begin, p_end), " should have been sent");
      return status::error_sending;
    }
    return status::ok;
  }

  status receive(std::string::iterator p_begin, std::string::iterator &p_end) {
    const auto _size = std::distance(p_begin, p_end);
    const auto _read = ::read(m_sockfd, &(*p_begin), _size);
    if (_read == -1) {
      return status::error_receiving;
    }
    if (_read == 0) {
      p_end = p_begin;
      return status::end_of_message;
    }

    if (_read != _size) {
      p_end = std::next(p_begin, _read);
    }

    return status::ok;
  }

private:
  status create_socket_ip(const char *p_ip, int p_port) {
    struct sockaddr_in servaddr;
    if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      comm_log_error(logger, "Error creating socket");
      return status::error_connecting;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(p_port); /* daytime server */
    int _rc = inet_pton(AF_INET, p_ip, &servaddr.sin_addr);
    if (_rc <= 0) {
      comm_log_error(logger, "Error '", std::strerror(errno), "', rc = ", _rc,
                     " on inet_pton error for ", p_ip);
      return status::error_connecting;
    }

    if (connect(m_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
      comm_log_error(logger, "Error connecting to ", p_ip, ":", p_port);
      return status::error_connecting;
    }
    return status::ok;
  }

  status create_socket(const char *p_ip) {
    /*
     * Lookup the daytime tcp service:
     */
    struct servent *sp = getservbyname("daytime", "tcp");
    if (!sp) {
      comm_log_error(logger, "Unknown service: daytime tcp");
      return status::error_connecting;
    }

    /*
     * Create a server socket address:
     */
    struct sockaddr_in adr_srvr; /* AF_INET */
    memset(&adr_srvr, 0, sizeof adr_srvr);
    adr_srvr.sin_family = AF_INET;
    adr_srvr.sin_port = sp->s_port;
    adr_srvr.sin_addr.s_addr = inet_addr(p_ip);

    if (adr_srvr.sin_addr.s_addr == INADDR_NONE) {
      comm_log_error(logger, "bad address");
    }
    int len_inet = sizeof adr_srvr;

    /*
     * Create a TCP/IP socket to use:
     */
    m_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (m_sockfd == -1) {
      comm_log_error(logger, "socket()");
    }
    /*
     * Connect to the server:
     */
    int z = connect(m_sockfd, (const sockaddr *)&adr_srvr, len_inet);
    if (z == -1) {
      comm_log_error(logger, "connect(2)");
    }
    return status::ok;
  }

  status create_socket_url(char url_str[]) {

    char hostname[256] = "";
    char portnum[6] = "443";
    char proto[6] = "";
    char *tmp_ptr = NULL;
    int port;
    struct hostent *host;
    struct sockaddr_in dest_addr;

    /* ---------------------------------------------------------- *
     * Remove the final / from url_str, if there is one           *
     * ---------------------------------------------------------- */
    if (url_str[strlen(url_str)] == '/')
      url_str[strlen(url_str)] = '\0';

    /* ---------------------------------------------------------- *
     * the first : ends the protocol string, i.e. http            *
     * ---------------------------------------------------------- */
    strncpy(proto, url_str, (strchr(url_str, ':') - url_str));

    /* ---------------------------------------------------------- *
     * the hostname starts after the "://" part                   *
     * ---------------------------------------------------------- */
    strncpy(hostname, strstr(url_str, "://") + 3, sizeof(hostname));

    /* ---------------------------------------------------------- *
     * if the hostname contains a colon :, we got a port number   *
     * ---------------------------------------------------------- */
    if (strchr(hostname, ':')) {
      tmp_ptr = strchr(hostname, ':');
      /* the last : starts the port number, if avail, i.e. 8443 */
      strncpy(portnum, tmp_ptr + 1, sizeof(portnum));
      *tmp_ptr = '\0';
    }

    port = atoi(portnum);

    if ((host = gethostbyname(hostname)) == NULL) {
      comm_log_error(logger, "Error: Cannot resolve hostname ", hostname);
      return status::error_connecting;
    }

    /* ---------------------------------------------------------- *
     * create the basic TCP socket                                *
     * ---------------------------------------------------------- */
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = *(long *)(host->h_addr);

    /* ---------------------------------------------------------- *
     * Zeroing the rest of the struct                             *
     * ---------------------------------------------------------- */
    memset(&(dest_addr.sin_zero), '\0', 8);

    tmp_ptr = inet_ntoa(dest_addr.sin_addr);

    /* ---------------------------------------------------------- *
     * Try to make the host connect here                          *
     * ---------------------------------------------------------- */
    if (connect(m_sockfd, (struct sockaddr *)&dest_addr,
                sizeof(struct sockaddr)) == -1) {
      comm_log_error(logger, "Error: Cannot connect to host ", hostname, "[",
                     tmp_ptr, "] on port ", port, ": ", std::strerror(errno));
      return status::error_connecting;
    }
    return status::ok;
  }

private:
  int m_sockfd{-1};
};

} // namespace tst
} // namespace communication
} // namespace tenacitas

struct basic_tcp_socket_test {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::tcp_socket connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    status _status = _client.connect("129.6.15.28");

    //    status _status = _client.connect("http://time-a-g.nist.gov:13");

    if (_status != status::ok) {
      comm_log_error(logger, "erro connecting: ", _status);
      return false;
    }

    std::pair<status, std::string> _result = _client.receive<std::string>();
    _status = _result.first;
    if (_status != status::ok) {
      comm_log_error(logger, "erro receiving: ", _status);
      return false;
    }

    if (_result.second.empty()) {
      comm_log_info(logger, "no msg received");
    } else {
      comm_log_info(logger, "received = ", _result.second);
    }

    return true;
  }

  static std::string desc() {
    return "Test connection to a site, and receive its home page";
  }
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, basic_tcp_socket_test);
}

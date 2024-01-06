
#include <chrono>

#include <tenacitas.h>

#include "../communication.h"

using namespace std::chrono_literals;

struct io_00_a {
  tnctl::communication_status_t write(const uint8_t *p_begin,
                                      const uint8_t *p_end) {
    TNCT_LOG_TST(std::string(reinterpret_cast<const char *>(p_begin)),
                 std::string(reinterpret_cast<const char *>(p_end)));

    return tnctl::communication_status_t::ERROR_WRITING;
  }

  tnctl::communication_status_t read(uint8_t *, uint8_t *, uint8_t **) {
    return tnctl::communication_status_t::NO_ERROR;
  }

  void set_write_timeout(std::chrono::milliseconds) {}

  void set_read_timeout(std::chrono::milliseconds) {}

  std::chrono::milliseconds get_write_timeout() { return 100ms; }
  std::chrono::milliseconds get_read_timeout() { return 100ms; }
};

struct dummy_e {
  dummy_e() {}
};

using dispatcher_a =
    tla::dispatcher<tnctl::connection_error_unidentified_e,
                        tnctl::connection_error_writing_e,
                        tnctl::connection_error_reading_e,
                        tnctl::connection_error_timeout_writing_e,
                        tnctl::connection_error_timeout_reading_e,
                        tnctl::connection_lost_e, dummy_e>;

// using connection_a = tnctl::connection_a<char, io_00_a, dispatcher_a>;

struct connection_dummy_a : tnctl::connection_a<dispatcher_a> {
  connection_dummy_a(dispatcher_a::ptr p_dispatcher)
      : tnctl::connection_a<dispatcher_a>(p_dispatcher) {}

protected:
  tnctl::communication_status_t do_write(const uint8_t *,
                                         const uint8_t *) override {
    return tnctl::communication_status_t::NO_ERROR;
  }
  tnctl::communication_status_t do_read(uint8_t *, uint8_t *,
                                        uint8_t **) override {
    return tnctl::communication_status_t::NO_ERROR;
  }
  void do_set_write_timeout(std::chrono::milliseconds) override {}
  void do_set_read_timeout(std::chrono::milliseconds) override {}
};

struct run {
  using events_subscribed =
      std::tuple<tnctl::connection_error_unidentified_e,
                 tnctl::connection_error_writing_e,
                 tnctl::connection_error_reading_e,
                 tnctl::connection_error_timeout_writing_e>;

  run() {
    m_dispatcher->subscribe<run, tnctl::connection_error_writing_e>(
        [&](auto p_event) {
          TNCT_LOG_TST("error reading from connection ", p_event.connection_id);
        });

    m_dispatcher->subscribe<run, tnctl::connection_error_unidentified_e>(
        [&](auto p_event) {
          TNCT_LOG_TST("error unidentified in connection ",
                       p_event.connection_id);
        });
  }

  void operator()() {
    connection_dummy_a _connection(m_dispatcher);

    const std::string _msg{"hello"};
    _connection.write(&_msg[0], &_msg[_msg.size()]);

    std::this_thread::sleep_for(2s);
  }

private:
  dispatcher_a::ptr m_dispatcher{dispatcher_a::create()};
};

int main() { run()(); }

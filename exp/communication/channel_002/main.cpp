/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example communication_000

#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <tuple>
#include <typeindex>
#include <vector>

#include <tenacitas.lib/src/async/alg/sleeping_loop.h>
#include <tenacitas.lib/src/communication/alg/channel.h>
#include <tenacitas.lib/src/communication/alg/message.h>
#include <tenacitas.lib/src/communication/cpt/concepts.h>
#include <tenacitas.lib/src/communication/evt/new_connection.h>
#include <tenacitas.lib/src/communication/evt/send_timeout.h>
#include <tenacitas.lib/src/communication/typ/channel_id.h>
#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/number/typ/id.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

struct event_a {

  friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
    p_out << "event a";
    return p_out;
  }
};

struct serializer_a_s {
  using event = event_a;
  using message = std::string;

  message operator()(const event &) { return "301892"; }
};

struct deserializer_a_s {
  using event = event_a;
  using message = std::string;
  std::optional<event> operator()(const message &) { return {event{}}; }
};

struct io_s {
  using message = std::string;

  void send(communication::typ::connection_id, const message &p_msg) {
    std::this_thread::sleep_for(75ms);
    TNCT_LOG_INF("'io_s' will send message '", p_msg, "'");
  }

  message receive(communication::typ::connection_id) { return "092"; }

  const std::chrono::milliseconds receive_timeout{100ms};
  const std::chrono::milliseconds send_timeout{50ms};
};

int main() {
  log::alg::set_debug_level();
  async::alg::dispatcher::ptr _dispatcher{async::alg::dispatcher::create()};
  {
    constexpr communication::typ::channel_id _channel_id{12};
    using channel = communication::alg::channel<_channel_id, std::string, io_s,
                                                std::tuple<serializer_a_s>,
                                                std::tuple<deserializer_a_s>>;

    _dispatcher->subscribe<communication::evt::send_timeout<std::string>>(
        [](auto p_event) {
          TNCT_LOG_WAR("timeout in channel ", p_event.channel_id,
                       ", connection ", p_event.connection_id, ", message ",
                       communication::alg::msg2str(p_event.message));
        });

    channel _channel(_dispatcher);

    _dispatcher->publish<communication::evt::new_connection<_channel_id>>(
        communication::typ::connection_id{8});

    _dispatcher->publish<event_a>(event_a{});

    std::this_thread::sleep_for(3s);

    TNCT_LOG_DEB("exiting");
  }

  /*
    {
      constexpr communication::typ::channel_id _channel_id{55};
      using channel =
          communication::alg::channel<_channel_id, std::vector<int>, io_i,
                                  std::tuple<serializer_a_i>, std::tuple<>>;

      //  std::condition_variable _cond_exit;

      channel _channel(_dispatcher);

      _dispatcher->publish<communication::evt::new_connection<_channel_id>>(
          communication::typ::connection_id{8});

      std::this_thread::sleep_for(1s);

      TNCT_LOG_DEB("exiting");
    }

  main.cpp:184:25: Constraints not satisfied for class template 'channel' [with
  t_channel_id = 55, t_message = std::vector<int>, t_io = io_i, t_serializers =
  std::tuple<serializer_a_i>, t_deserializers = std::tuple<>, t_num_subscribers
  = 1] channel.h:23:41: because 'std::vector<int>' does not satisfy 'message'
  concepts.h:28:12: because 'sizeof(typename vector<int>::value_type) == 1' (4
  == 1) evaluated to false
  */
}

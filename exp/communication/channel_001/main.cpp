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

using variable_message = std::vector<std::byte>;

struct event_a {

  friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
    p_out << "event a";
    return p_out;
  }
};

struct serializer_a_v {
  using event = event_a;
  using message = variable_message;

  message operator()(const event &) {
    return {std::byte{7}, std::byte{28}, std::byte{9}, std::byte{174},
            std::byte{53}};
  }
};

struct deserializer_a_v {
  using event = event_a;
  using message = variable_message;
  std::optional<event> operator()(const message &) { return {event{}}; }
};

struct io_v {
  using message = variable_message;

  void send(communication::typ::connection_id, const message &p_msg) {
    auto _str{communication::alg::msg2str(p_msg)};
    TNCT_LOG_INF("message to send = '", _str, "'");
  }

  message receive(communication::typ::connection_id) {
    std::this_thread::sleep_for(100ms);
    return {std::byte{37}, std::byte{2}, std::byte{193}, std::byte{71}};
  }

  const std::chrono::milliseconds receive_timeout{200ms};
  const std::chrono::milliseconds send_timeout{50ms};
};

int main() {
  log::alg::set_debug_level();
  async::alg::dispatcher::ptr _dispatcher{async::alg::dispatcher::create()};

  {
    constexpr communication::typ::channel_id _channel_id{23};

    using message = variable_message;
    using io = io_v;
    using serializers = std::tuple<serializer_a_v>;
    using deserializers = std::tuple<deserializer_a_v>;

    using channel = communication::alg::channel<_channel_id, message, io,
                                                serializers, deserializers>;

    channel _channel(_dispatcher);

    _dispatcher->publish<communication::evt::new_connection<_channel_id>>(
        communication::typ::connection_id{11});

    _dispatcher->publish<event_a>(event_a{});

    std::this_thread::sleep_for(1500ms);

    TNCT_LOG_DEB("exiting");
  }
}

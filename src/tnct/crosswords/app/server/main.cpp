#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility> // Fix for std::exchange in Boost 1.74

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

namespace beast     = boost::beast;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = net::ip::tcp;
using nlohmann::json;

// WebSocket session
class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
public:
  explicit WebSocketSession(tcp::socket socket) : ws_(std::move(socket))
  {
  }

  void start()
  {
    ws_.async_accept(
        [self = shared_from_this()](beast::error_code ec)
        {
          if (ec)
          {
            std::cerr << "Accept error: " << ec.message() << "\n";
            return;
          }
          self->read_message();
        });
  }

private:
  void read_message()
  {
    ws_.async_read(
        buffer_,
        [self = shared_from_this()](beast::error_code ec, std::size_t)
        {
          if (!ec)
          {
            std::string msg = beast::buffers_to_string(self->buffer_.data());
            self->buffer_.consume(self->buffer_.size());

            // auto _payload = json::parse(msg);
            // std::cerr << _payload << std::endl;
            //     auto _request{_payload["request"]};
            std::cout << "thread = " << std::this_thread::get_id()
                      << ", msg = " << msg << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(3));
            // Echo back
            self->ws_.text(self->ws_.got_text());
            self->ws_.async_write(net::buffer(msg),
                                  [self](beast::error_code ec, std::size_t)
                                  {
                                    if (ec)
                                      std::cerr
                                          << "Write error: " << ec.message()
                                          << "\n";
                                    else
                                      self->read_message();
                                  });
          }
        });
  }

  websocket::stream<tcp::socket> ws_;
  beast::flat_buffer             buffer_;
};

// Listener
class WebSocketListener : public std::enable_shared_from_this<WebSocketListener>
{
public:
  WebSocketListener(net::io_context &ioc, tcp::endpoint ep)
      : /*ioc_(ioc),*/ acceptor_(ioc, ep)
  {
  }

  void run()
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [self = shared_from_this()](beast::error_code ec, tcp::socket socket)
        {
          if (!ec)
            std::make_shared<WebSocketSession>(std::move(socket))->start();
          self->do_accept();
        });
  }

  //  net::io_context &ioc_;
  tcp::acceptor acceptor_;
};

int main()
{
  try
  {
    net::io_context ioc;
    auto            listener = std::make_shared<WebSocketListener>(
        ioc, tcp::endpoint{tcp::v4(), 9002});
    listener->run();
    std::cout << "WebSocket server running on port 9002\n";
    ioc.run();
  }
  catch (std::exception const &e)
  {
    std::cerr << "Fatal: " << e.what() << "\n";
  }
}

// #include <algorithm>
// #include <chrono>
// #include <functional>
// #include <memory>
// #include <mutex>
// #include <optional>
// #include <vector>

// #include <nlohmann/json.hpp>
// #include <websocketpp/config/asio_no_tls.hpp>
// #include <websocketpp/server.hpp>

// #include "tnct/async/handling_priority.h"
// #include "tnct/async/result.h"
// #include "tnct/container/circular_queue.h"
// #include "tnct/crosswords/bus/grid_creator.h"
// #include "tnct/crosswords/dat/entries.h"
// #include "tnct/crosswords/dat/grid.h"
// #include "tnct/crosswords/dat/index.h"
// #include "tnct/crosswords/dat/layout.h"
// #include "tnct/crosswords/evt/dispatcher.h"
// #include "tnct/crosswords/evt/grid_create_solved.h"
// #include "tnct/crosswords/evt/grid_create_start.h"
// // #include "tnct/crosswords/evt/grid_create_timeout.h"
// #include "tnct/crosswords/evt/grid_create_unsolved.h"
// #include "tnct/format/fmt.h"
// #include "tnct/log/cerr.h"

// using tnct::format::fmt;
// using logger = tnct::log::cerr;
// using tnct::async::handling_priority;
// using tnct::crosswords::evt::dispatcher;
// using grid_creator = tnct::crosswords::bus::grid_creator<logger, dispatcher>;
// using grid_index   = tnct::crosswords::dat::index;
// using grid_layout  = tnct::crosswords::dat::layout;
// using grid_attempt_configuration =
//     tnct::crosswords::evt::grid_attempt_configuration;
// using grid_create_solved = tnct::crosswords::evt::grid_create_solved;
// using grid_create_start  = tnct::crosswords::evt::grid_create_start;
// using grid_create_stop   = tnct::crosswords::evt::grid_create_stop;
// // using grid_create_timeout     =
// tnct::crosswords::evt::grid_create_timeout; using grid_create_unsolved    =
// tnct::crosswords::evt::grid_create_unsolved; using grid_permutations_tried =
// tnct::crosswords::evt::grid_permutations_tried; using grid_entries =
// tnct::crosswords::dat::entries; using tnct::async::result; using
// tnct::crosswords::dat::grid; using grid_ptr = std::shared_ptr<grid>; using
// json     = nlohmann::json; using websocketpp::connection_hdl; using server =
// websocketpp::server<websocketpp::config::asio>; using std::chrono::seconds;

// using msg_sender = std::function<void(connection_hdl, const json &)>;

// struct connection
// {

//   using ptr = std::shared_ptr<connection>;

//   static ptr create(logger &p_logger, connection_hdl p_hdl,
//                     msg_sender p_msg_sender)
//   {
//     return ptr{new connection{p_logger, p_hdl, p_msg_sender}};
//   }

//   connection(const connection &) = delete;
//   connection(connection &&)      = delete;

//   connection &operator=(const connection &p_) = delete;
//   connection &operator=(connection &&)        = delete;

//   bool operator==(const connection &p_connection) const
//   {
//     return m_hdl.lock().get() == p_connection.m_hdl.lock().get();
//   }

//   bool operator==(connection_hdl &p_hdl) const
//   {
//     return m_hdl.lock().get() == p_hdl.lock().get();
//   }

//   void handle_message(server::message_ptr p_msg)
//   {
//     auto _payload = json::parse(p_msg->get_payload());
//     auto _request{_payload["request"]};
//     if (_request == "create")
//     {
//       stop_creating();
//       start_creating(_payload);
//     }
//     else if (_request == "stop")
//     {
//       stop_creating();
//     }
//     else
//     {
//       TNCT_LOG_ERR(m_logger, fmt("request unrecognized: '", _request, '\''));
//     }
//   }

// protected:
//   connection(logger &p_logger, connection_hdl p_hdl, msg_sender p_msg_sender)
//       : m_logger{p_logger}, m_hdl(p_hdl), m_msg_sender(p_msg_sender)
//   {
//     configure_dispatcher();
//   }

//   void stop_creating()
//   {
//     if (auto _result = m_dispatcher.publish<grid_create_stop>();
//         _result != result::OK)
//     {
//       TNCT_LOG_ERR(m_logger, "error publishing 'grid_create_stop'");
//     }
//   }

//   void start_creating(const json &p_json)
//   {
//     try
//     {
//       grid_index   _rows{p_json["rows"]};
//       grid_index   _cols{p_json["cols"]};
//       grid_index   _max_rows{p_json["max_rows"]};
//       seconds      _interval{p_json["interval"]};
//       grid_entries _grid_entries;

//       deserialize_entries(p_json, _grid_entries);

//       TNCT_LOG_INF(m_logger, fmt("Received crossword request: ", _rows, 'x',
//                                  _cols, ", max rows = ", _max_rows,
//                                  ", interval = ", _interval.count(),
//                                  ", entries = ", _grid_entries));

//       if (auto _result = m_dispatcher.publish<grid_create_start>(
//               _grid_entries, _rows, _cols, _interval, _max_rows);
//           _result != result::OK)
//       {
//         TNCT_LOG_ERR(m_logger,
//                      fmt("error publishing 'grid_create_start': ", _result));
//         send_error("error when starting to create the grid");
//         return;
//       }
//     }
//     catch (const std::exception &e)
//     {
//       TNCT_LOG_ERR(m_logger, fmt("Error parsing request: '", e.what(),
//       '\''));
//     }
//   }

//   void send_error(std::string_view p_msg)
//   {
//     json _out;
//     _out["response"] = "error";
//     _out["content"]  = p_msg;
//     m_msg_sender(m_hdl, _out);
//   }

//   void send_unsolved()
//   {
//     json _out;
//     _out["response"] = "unsolved";
//     _out["content"]  = "grid was not assembled";
//     m_msg_sender(m_hdl, _out);
//   }

//   void send_attempt_configuration(const grid_attempt_configuration &p_event)
//   {
//     json _out;
//     _out["response"] = "configuration";
//     _out["rows"]     = "\"" + std::to_string(p_event.num_rows) + "\"";
//     _out["cols"]     = "\"" + std::to_string(p_event.num_cols) + "\"";
//     _out["mem_available"] =
//         "\"" + std::to_string(p_event.memory_available) + "\"";
//     _out["mem_used"] =
//         "\"" + std::to_string(p_event.max_memory_to_be_used) + "\"";
//     _out["num_permutations"] =
//         "\"" + std::to_string(p_event.number_of_permutations) + "\"";
//     m_msg_sender(m_hdl, _out);
//   }

//   void send_permutations_tried(const grid_permutations_tried &p_event)
//   {
//     json _out;
//     _out["response"]     = "tries";
//     _out["permutations"] = "\"" + std::to_string(p_event.permutations) +
//     "\""; m_msg_sender(m_hdl, _out);
//   }

//   json layout_to_json(const grid_layout &p_layout)
//   {
//     std::stringstream _stream;
//     _stream << p_layout.get_orientation();
//     return json{{"word", p_layout.get_word()},
//                 {"explanation", p_layout.get_explanation()},
//                 {"row", p_layout.get_row()},
//                 {"col", p_layout.get_col()},
//                 {"orientation", _stream.str()}};
//   }

//   json layouts_to_json(grid::const_layout_ite p_begin,
//                        grid::const_layout_ite p_end)
//   {
//     json arr = json::array();
//     std::transform(p_begin, p_end, std::back_inserter(arr),
//                    [this](const grid_layout &p_layout)
//                    { return layout_to_json(p_layout); });
//     return arr;
//   }

//   void send_create_solved(const grid_create_solved &p_event)
//   {
//     json _out;
//     _out["response"]   = "solved";
//     _out["total_time"] = "\"" + std::to_string(p_event.time.count()) + "\"";
//     _out["at_permutation"] =
//         "\"" + std::to_string(p_event.grid->get_permutation_number()) + "\"";
//     _out["rows"] = "\"" + std::to_string(p_event.grid->get_num_rows()) +
//     "\""; _out["cols"] = "\"" + std::to_string(p_event.grid->get_num_cols())
//     + "\""; _out["layouts"] =
//         layouts_to_json(p_event.grid->begin(), p_event.grid->end());
//     m_msg_sender(m_hdl, _out);
//   }

//   void configure_dispatcher()
//   {
//     using namespace tnct::container;

//     using grid_create_unsolved_queue =
//         circular_queue<logger, grid_create_unsolved, 2>;

//     using grid_create_solved_queue =
//         circular_queue<logger, grid_create_solved, 2>;

//     // using grid_create_timeout_queue =
//     //     circular_queue<logger, grid_create_timeout, 2>;

//     using grid_attempt_configuration_queue =
//         circular_queue<logger, grid_attempt_configuration, 2>;

//     using grid_permutations_tried_queue =
//         circular_queue<logger, grid_permutations_tried, 2>;

//     m_dispatcher.add_handling<grid_create_unsolved>(
//         "grid-create-unsolved", grid_create_unsolved_queue{m_logger},
//         [&](grid_create_unsolved &&) { send_unsolved(); });

//     m_dispatcher.add_handling<grid_create_solved>(
//         "grid-create-solved", grid_create_solved_queue{m_logger},
//         [&](grid_create_solved &&p_event) { send_create_solved(p_event); });

//     // m_dispatcher.add_handling<grid_create_timeout>(
//     //     "grid-create-timeout", grid_create_timeout_queue{m_logger},
//     //     [&](grid_create_timeout && /*p_event*/)
//     //     {
//     //       // m_cond_ended.notify_all();
//     //     });

//     m_dispatcher.template add_handling<grid_attempt_configuration>(
//         "grid-attempt-configuration",
//         grid_attempt_configuration_queue{m_logger},
//         [&](grid_attempt_configuration &&p_event)
//         { send_attempt_configuration(p_event); });

//     m_dispatcher.add_handling<grid_permutations_tried>(
//         "grid-permutations-tried", grid_permutations_tried_queue{m_logger},
//         [&](grid_permutations_tried &&p_event)
//         { send_permutations_tried(p_event); });
//   }

//   void deserialize_entries(const json &p_json, grid_entries &p_grid_entries)
//   {
//     for (const auto &item : p_json["entries"])
//     {
//       p_grid_entries.add_entry(item.at("word").get<std::string>(),
//                                item.at("explanation").get<std::string>());
//     }
//   }

// private:
//   logger        &m_logger;
//   connection_hdl m_hdl;
//   msg_sender     m_msg_sender;
//   dispatcher     m_dispatcher{m_logger};
//   grid_creator   m_grid_creator{m_logger, m_dispatcher};
// };

// class connections
// {
// public:
//   connections(logger &p_logger) : m_logger(p_logger)
//   {
//   }

//   connection::ptr add(connection_hdl hdl, msg_sender p_msg_sender)
//   {
//     std::lock_guard lock(m_mutex);

//     // reuse empty slot if available
//     for (std::size_t i = 0; i < m_connections.size(); ++i)
//     {
//       if (!m_connections[i])
//       {
//         m_connections[i] = connection::create(m_logger, hdl, p_msg_sender);
//         return m_connections[i];
//       }
//     }
//     // otherwise append new slot
//     m_connections.push_back(connection::create(m_logger, hdl, p_msg_sender));
//     return m_connections[m_connections.size() - 1];
//   }

//   connection::ptr get(connection_hdl p_hdl)
//   {
//     std::lock_guard lock(m_mutex);
//     return get_connection(p_hdl);
//   }

//   void remove(connection_hdl p_hdl)
//   {
//     std::lock_guard lock(m_mutex);
//     auto            _index{get_index(p_hdl)};
//     if (_index)
//     {
//       m_connections[_index.value()] = connection::ptr();
//     }
//   }

// private:
//   connection::ptr get_connection(connection_hdl p_hdl)
//   {
//     for (std::size_t i = 0; i < m_connections.size(); ++i)
//     {
//       if (m_connections[i] && (*m_connections[i] == p_hdl))
//       {
//         return m_connections[i];
//       }
//     }
//     return connection::ptr();
//   }

//   std::optional<std::size_t> get_index(connection_hdl p_connection)
//   {
//     for (std::size_t i = 0; i < m_connections.size(); ++i)
//     {
//       if (m_connections[i] && (*m_connections[i] == p_connection))
//       {
//         return i;
//       }
//     }
//     return std::nullopt;
//   }

// private:
//   std::vector<connection::ptr> m_connections;
//   std::mutex                   m_mutex;
//   logger                      &m_logger;
// };

// class crosswords_server
// {
// public:
//   crosswords_server()
//   {
//     // init ASIO transport
//     m_server.init_asio();

//     // set logging settings
//     m_server.set_access_channels(websocketpp::log::alevel::all);
//     m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

//     // register handlers
//     m_server.set_open_handler(
//         [this](connection_hdl p_hdl)
//         {
//           TNCT_LOG_DEB(m_logger, "new connection");
//           m_connections.add(p_hdl,
//                             [this](connection_hdl p_hdl, const json &p_json)
//                             {
//                               m_server.send(p_hdl, p_json.dump(),
//                                             websocketpp::frame::opcode::text);
//                             });
//         });

//     m_server.set_close_handler(
//         [this](connection_hdl p_hdl)
//         {
//           TNCT_LOG_DEB(m_logger, "connection closed");
//           m_connections.remove(p_hdl);
//         });

//     m_server.set_message_handler(
//         [this](connection_hdl p_hdl, server::message_ptr p_msg)
//         {
//           TNCT_LOG_DEB(m_logger, "new message");
//           connection::ptr _connection{m_connections.get(p_hdl)};
//           if (_connection)
//           {
//             _connection->handle_message(p_msg);
//           }
//         });
//   }

//   void run(std::uint16_t port)
//   {
//     m_server.listen(port);
//     m_server.start_accept();
//     m_server.run();
//   }

// private:
//   server      m_server;
//   logger      m_logger;
//   connections m_connections{m_logger};

//   // connection_dispatcher m_connection_dispatcher{m_logger};
// };

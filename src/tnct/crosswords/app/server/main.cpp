#include <iostream>

#include <ixwebsocket/IXWebSocketServer.h>
#include <nlohmann/json.hpp>

#include "tnct/crosswords/bus/grid_creator.h"
#include "tnct/crosswords/dat/entries.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/dat/index.h"
#include "tnct/crosswords/dat/layout.h"
#include "tnct/crosswords/evt/internal/grid_create_solved.h"
#include "tnct/crosswords/evt/internal/grid_create_start.h"
#include "tnct/crosswords/evt/internal/grid_create_unsolved.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/log/cpt/macros.h"

using nlohmann::json;

using std::chrono::seconds;

using logger = tnct::log::cerr;
using tnct::format::fmt;
using grid_creator = tnct::crosswords::bus::grid_creator<logger>;
using grid_index   = tnct::crosswords::dat::index;
using grid_layout  = tnct::crosswords::dat::layout;
using grid_entries = tnct::crosswords::dat::entries;
using tnct::crosswords::dat::grid;
using grid_ptr = std::shared_ptr<grid>;

struct connection
{
  connection(logger &p_logger)
      : m_logger{p_logger},
        m_grid_creator{
            m_logger,
            std::bind_front(&connection::send_solved, this),
            std::bind_front(&connection::send_unsolved, this),
        }
  {
    configure_callbacks();
  }

  connection(const connection &) = delete;
  connection(connection &)       = delete;

  connection &operator=(const connection &) = delete;
  connection &operator=(connection &)       = delete;

  void run(const std::string             &p_request,
           std::shared_ptr<ix::WebSocket> p_socket)
  {
    m_socket = p_socket;

    auto _payload = json::parse(p_request);
    if (_payload.value("request", "") == "create")
    {
      TNCT_LOG_INF(m_logger, "'create' request");
      start_creating(_payload);
    }
    else if (_payload.value("request", "") == "stop")
    {
      TNCT_LOG_INF(m_logger, "'stop' request");
      stop_creating();
    }
    else
    {
      TNCT_LOG_ERR(m_logger,
                   fmt("unrecognized 'request' in '", _payload.dump(), '\''));
    }
  }

  constexpr bool is_working() const
  {
    return m_working;
  }

private:
  void stop_creating()
  {
    m_grid_creator.stop();
    m_working = false;
  }

  void start_creating(const json &p_json)
  {
    try
    {
      grid_index   _rows{p_json.at("rows").get<grid_index>()};
      grid_index   _cols{p_json.at("cols").get<grid_index>()};
      grid_index   _max_rows{p_json.at("max_rows").get<grid_index>()};
      seconds      _interval{seconds{p_json.at("interval").get<std::size_t>()}};
      grid_entries _grid_entries;

      deserialize_entries(p_json, _grid_entries);

      TNCT_LOG_INF(m_logger, fmt("Received 'create' request: ", _rows, 'x',
                                 _cols, ", max rows = ", _max_rows,
                                 ", interval = ", _interval.count()));

      if (!m_grid_creator.start(_grid_entries, _rows, _cols, _interval,
                                _max_rows))
      {
        send_error("error when starting to create the grid");
        return;
      }
      m_working = true;
    }
    catch (const std::exception &e)
    {
      TNCT_LOG_ERR(m_logger, fmt("Error parsing request: '", e.what(), '\''));
    }
  }

  void send(const json &p_json)
  {
    TNCT_LOG_DEB(m_logger, fmt("about to send '", p_json.dump(), '\''));
    m_socket->send(p_json.dump());
  }

  void send_error(std::string_view p_msg)
  {
    json _out;
    _out["response"] = "error";
    _out["content"]  = p_msg;
    m_working        = false;
    send(_out);
  }

  void send_unsolved(grid_index p_num_rows, grid_index p_num_cols)
  {
    json _out;
    _out["response"] = "unsolved";
    _out["content"]  = "grid was not assembled";
    _out["rows"]     = "\"" + std::to_string(p_num_rows) + "\"";
    _out["cols"]     = "\"" + std::to_string(p_num_cols) + "\"";
    m_working        = false;
    send(_out);
  }

  void send_configuration(grid_index p_num_rows, grid_index p_num_cols,
                          std::size_t p_max_memory_to_be_used,
                          std::size_t p_memory_available,
                          std::size_t p_number_of_permutations)
  {
    json _out;
    _out["response"]      = "configuration";
    _out["rows"]          = "\"" + std::to_string(p_num_rows) + "\"";
    _out["cols"]          = "\"" + std::to_string(p_num_cols) + "\"";
    _out["mem_available"] = "\"" + std::to_string(p_memory_available) + "\"";
    _out["mem_used"] = "\"" + std::to_string(p_max_memory_to_be_used) + "\"";
    _out["num_permutations"] =
        "\"" + std::to_string(p_number_of_permutations) + "\"";
    send(_out);
  }

  void send_permutations(std::size_t p_permutations)
  {
    json _out;
    _out["response"]     = "tries";
    _out["permutations"] = "\"" + std::to_string(p_permutations) + "\"";
    send(_out);
  }

  void send_solved(grid_ptr p_grid, std::chrono::seconds p_time)
  {
    json _out;
    _out["response"]   = "solved";
    _out["total_time"] = "\"" + std::to_string(p_time.count()) + "\"";
    _out["at_permutation"] =
        "\"" + std::to_string(p_grid->get_permutation_number()) + "\"";
    _out["rows"]    = "\"" + std::to_string(p_grid->get_num_rows()) + "\"";
    _out["cols"]    = "\"" + std::to_string(p_grid->get_num_cols()) + "\"";
    _out["layouts"] = layouts_to_json(p_grid->begin(), p_grid->end());
    TNCT_LOG_INF(m_logger, "solved");
    m_working = false;
    send(_out);
  }

  void configure_callbacks()
  {
    m_grid_creator.on_configuration(
        std::bind_front(&connection::send_configuration, this));

    m_grid_creator.on_permutations_tried(
        std::bind_front(&connection::send_permutations, this));
  }

  void deserialize_entries(const json &p_json, grid_entries &p_grid_entries)
  {
    for (const auto &item : p_json["entries"])
    {
      p_grid_entries.add_entry(item.at("word").get<std::string>(),
                               item.at("explanation").get<std::string>());
    }
  }

  json layout_to_json(const grid_layout &p_layout)
  {
    std::stringstream _stream;
    _stream << p_layout.get_orientation();
    return json{{"word", p_layout.get_word()},
                {"explanation", p_layout.get_explanation()},
                {"row", p_layout.get_row()},
                {"col", p_layout.get_col()},
                {"orientation", _stream.str()}};
  }

  json layouts_to_json(grid::const_layout_ite p_begin,
                       grid::const_layout_ite p_end)
  {
    json arr = json::array();
    std::transform(p_begin, p_end, std::back_inserter(arr),
                   [this](const grid_layout &p_layout)
                   { return layout_to_json(p_layout); });
    return arr;
  }

private:
  logger                        &m_logger;
  std::shared_ptr<ix::WebSocket> m_socket;
  grid_creator                   m_grid_creator;
  bool                           m_working{false};
};

using connection_ptr = std::unique_ptr<connection>;
using connections    = std::vector<connection_ptr>;

connections::iterator create_connection(connections &p_connections,
                                        logger      &p_logger)
{
  for (connections::iterator _ite = p_connections.begin();
       _ite != p_connections.end(); ++_ite)
  {
    if (!(*_ite)->is_working())
    {
      TNCT_LOG_INF(p_logger, fmt("reusing connection # ",
                                 std::distance(p_connections.begin(), _ite)));
      return _ite;
    }
  }
  TNCT_LOG_INF(p_logger, "creating new connection");
  p_connections.emplace_back(std::make_unique<connection>(p_logger));
  return std::prev(p_connections.end());
}

int main()
{
  logger _logger;

  // _logger.set_deb();

  std::vector<connection_ptr> _connections;

  // Create a server on port 9002
  ix::WebSocketServer server(9002);

  // Set up connection handling
  server.setOnConnectionCallback(
      [&](std::weak_ptr<ix::WebSocket> wsWeak,
          std::shared_ptr<ix::ConnectionState> /*state*/)
      {
        if (auto ws = wsWeak.lock())
        {
          ws->setOnMessageCallback(
              [ws, &_logger, &_connections](const ix::WebSocketMessagePtr &msg)
              {
                if (msg->type == ix::WebSocketMessageType::Message)
                {
                  (*create_connection(_connections, _logger))
                      ->run(std::move(msg->str), ws);
                }
                else if (msg->type == ix::WebSocketMessageType::Open)
                {
                  TNCT_LOG_INF(_logger, "New client connected!");
                }
                else if (msg->type == ix::WebSocketMessageType::Close)
                {
                  TNCT_LOG_INF(_logger, "Client disconnected.");
                }
              });
        }
      });

  // Start the server
  auto res = server.listen();
  if (!res.first)
  {
    std::cerr << "Error starting server: " << res.second << std::endl;
    return 1;
  }

  server.start();
  std::cout << "WebSocket server listening on ws://localhost:9002" << std::endl;

  // Run forever
  server.wait();

  return 0;
}

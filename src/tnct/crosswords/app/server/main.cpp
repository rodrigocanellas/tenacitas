#include <iostream>

#include <ixwebsocket/IXWebSocketServer.h>
#include <nlohmann/json.hpp>

#include "tnct/async/handling_priority.h"
#include "tnct/async/result.h"
#include "tnct/container/circular_queue.h"
#include "tnct/crosswords/bus/grid_creator.h"
#include "tnct/crosswords/dat/entries.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/dat/index.h"
#include "tnct/crosswords/dat/layout.h"
#include "tnct/crosswords/evt/dispatcher.h"
#include "tnct/crosswords/evt/grid_create_solved.h"
#include "tnct/crosswords/evt/grid_create_start.h"
#include "tnct/crosswords/evt/grid_create_unsolved.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/log/cpt/macros.h"

using nlohmann::json;

using logger = tnct::log::cerr;
using tnct::crosswords::evt::dispatcher;
using grid_creator = tnct::crosswords::bus::grid_creator<logger, dispatcher>;
using tnct::format::fmt;
using logger = tnct::log::cerr;
using tnct::async::handling_priority;
using grid_creator = tnct::crosswords::bus::grid_creator<logger, dispatcher>;
using grid_index   = tnct::crosswords::dat::index;
using grid_layout  = tnct::crosswords::dat::layout;
using grid_attempt_configuration =
    tnct::crosswords::evt::grid_attempt_configuration;
using grid_create_solved      = tnct::crosswords::evt::grid_create_solved;
using grid_create_start       = tnct::crosswords::evt::grid_create_start;
using grid_create_stop        = tnct::crosswords::evt::grid_create_stop;
using grid_create_unsolved    = tnct::crosswords::evt::grid_create_unsolved;
using grid_permutations_tried = tnct::crosswords::evt::grid_permutations_tried;
using grid_entries            = tnct::crosswords::dat::entries;
using tnct::async::result;
using tnct::crosswords::dat::grid;
using grid_ptr = std::shared_ptr<grid>;
using std::chrono::seconds;

struct connection
{
  connection(logger &p_logger)
      : m_logger{p_logger}, m_dispatcher{m_logger},
        m_grid_creator{m_logger, m_dispatcher}
  {
    configure_dispatcher();
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
    if (auto _result = m_dispatcher.publish<grid_create_stop>();
        _result != result::OK)
    {
      TNCT_LOG_ERR(m_logger, "error publishing 'grid_create_stop'");
    }
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

      if (auto _result = m_dispatcher.publish<grid_create_start>(
              _grid_entries, _rows, _cols, _interval, _max_rows);
          _result != result::OK)
      {
        TNCT_LOG_ERR(m_logger,
                     fmt("error publishing 'grid_create_start': ", _result));
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

  void send_unsolved()
  {
    json _out;
    _out["response"] = "unsolved";
    _out["content"]  = "grid was not assembled";
    m_working        = false;
    send(_out);
  }

  void send_attempt_configuration(const grid_attempt_configuration &p_event)
  {
    json _out;
    _out["response"] = "configuration";
    _out["rows"]     = "\"" + std::to_string(p_event.num_rows) + "\"";
    _out["cols"]     = "\"" + std::to_string(p_event.num_cols) + "\"";
    _out["mem_available"] =
        "\"" + std::to_string(p_event.memory_available) + "\"";
    _out["mem_used"] =
        "\"" + std::to_string(p_event.max_memory_to_be_used) + "\"";
    _out["num_permutations"] =
        "\"" + std::to_string(p_event.number_of_permutations) + "\"";
    send(_out);
  }

  void send_permutations_tried(const grid_permutations_tried &p_event)
  {
    json _out;
    _out["response"]     = "tries";
    _out["permutations"] = "\"" + std::to_string(p_event.permutations) + "\"";
    send(_out);
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

  void send_create_solved(const grid_create_solved &p_event)
  {
    json _out;
    _out["response"]   = "solved";
    _out["total_time"] = "\"" + std::to_string(p_event.time.count()) + "\"";
    _out["at_permutation"] =
        "\"" + std::to_string(p_event.grid->get_permutation_number()) + "\"";
    _out["rows"] = "\"" + std::to_string(p_event.grid->get_num_rows()) + "\"";
    _out["cols"] = "\"" + std::to_string(p_event.grid->get_num_cols()) + "\"";
    _out["layouts"] =
        layouts_to_json(p_event.grid->begin(), p_event.grid->end());
    TNCT_LOG_INF(m_logger, "solved");
    m_working = false;
    send(_out);
  }

  void configure_dispatcher()
  {
    using namespace tnct::container;

    using grid_create_unsolved_queue =
        circular_queue<logger, grid_create_unsolved, 2>;

    using grid_create_solved_queue =
        circular_queue<logger, grid_create_solved, 2>;

    using grid_attempt_configuration_queue =
        circular_queue<logger, grid_attempt_configuration, 2>;

    using grid_permutations_tried_queue =
        circular_queue<logger, grid_permutations_tried, 2>;

    m_dispatcher.add_handling<grid_create_unsolved>(
        "grid-create-unsolved", grid_create_unsolved_queue{m_logger},
        [&](grid_create_unsolved &&) { send_unsolved(); });

    m_dispatcher.add_handling<grid_create_solved>(
        "grid-create-solved", grid_create_solved_queue{m_logger},
        [&](grid_create_solved &&p_event) { send_create_solved(p_event); });

    m_dispatcher.template add_handling<grid_attempt_configuration>(
        "grid-attempt-configuration",
        grid_attempt_configuration_queue{m_logger},
        [&](grid_attempt_configuration &&p_event)
        { send_attempt_configuration(p_event); });

    m_dispatcher.add_handling<grid_permutations_tried>(
        "grid-permutations-tried", grid_permutations_tried_queue{m_logger},
        [&](grid_permutations_tried &&p_event)
        { send_permutations_tried(p_event); });
  }

  void deserialize_entries(const json &p_json, grid_entries &p_grid_entries)
  {
    for (const auto &item : p_json["entries"])
    {
      p_grid_entries.add_entry(item.at("word").get<std::string>(),
                               item.at("explanation").get<std::string>());
    }
  }

private:
  logger                        &m_logger;
  std::shared_ptr<ix::WebSocket> m_socket;
  dispatcher                     m_dispatcher{m_logger};
  grid_creator                   m_grid_creator{m_logger, m_dispatcher};
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

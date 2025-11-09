#ifndef TNCT_CROSSWORDS_APP_SERVER_CONNECTION_H
#define TNCT_CROSSWORDS_APP_SERVER_CONNECTION_H

#include <functional>
#include <memory>
#include <sstream>

#include <ixwebsocket/IXWebSocketServer.h>
#include <nlohmann/json.hpp>

#include "tnct/crosswords/app/server/statistics.h"
#include "tnct/crosswords/bus/grid_creator.h"
#include "tnct/crosswords/dat/entries.h"
#include "tnct/crosswords/dat/error.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/dat/index.h"
#include "tnct/crosswords/dat/layout.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"

using nlohmann::json;

using std::chrono::seconds;

using logger = tnct::log::cerr;
using tnct::format::bus::fmt;
using grid_creator = tnct::crosswords::bus::grid_creator<logger>;
using grid_index   = tnct::crosswords::dat::index;
using grid_layout  = tnct::crosswords::dat::layout;
using grid_entries = tnct::crosswords::dat::entries;
using tnct::crosswords::dat::grid;
using grid_error = tnct::crosswords::dat::error;
using grid_ptr   = std::shared_ptr<grid>;

namespace tnct::crosswords::app::server
{

struct connection
{
  connection(logger &p_logger)
      : m_logger{p_logger},
        m_grid_creator{
            m_logger,
            std::bind_front(&connection::send_solved, this),
            std::bind_front(&connection::send_unsolved, this),
        },
        m_statistics{m_logger}
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
  // void send_translation(const json &p_json)
  // {
  //   json _out;
  //   _out["response"] = "translation";
  //   const auto _lang{p_json.at("lang").get<std::string>()};
  //   _out["lang"] = _lang;
  //   const auto _translation{m_translations.get(_lang)};
  //   if (_translation.has_value())
  //   {
  //     _out["translations"] = json::parse(_translation.value());
  //     send(_out);
  //   }
  // }

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

      TNCT_LOG_INF(m_logger, fmt("JSON: ", p_json.dump()));

      if (!m_grid_creator.start(_grid_entries, _rows, _cols, _interval,
                                _max_rows))
      {
        TNCT_LOG_ERR(m_logger, "error when starting to create the grid");
        return;
      }
      m_statistics.grid_creation();

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

  void send_error(const grid_error &p_error)
  {
    json _out;
    _out["response"] = "error";
    _out["code"]     = p_error;
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

  void send_solved(grid_ptr p_grid, std::chrono::seconds p_time,
                   std::uint64_t p_max_permutations)
  {
    json _out;
    _out["response"]   = "solved";
    _out["total_time"] = "\"" + std::to_string(p_time.count()) + "\"";
    _out["at_permutation"] =
        "\"" + std::to_string(p_grid->get_permutation_number()) + "\"";
    _out["rows"]    = "\"" + std::to_string(p_grid->get_num_rows()) + "\"";
    _out["cols"]    = "\"" + std::to_string(p_grid->get_num_cols()) + "\"";
    _out["layouts"] = layouts_to_json(p_grid->begin(), p_grid->end());
    _out["max_permutations"] = p_max_permutations;
    TNCT_LOG_INF(m_logger, "solved");
    m_working = false;
    send(_out);
  }

  void send_grid_error(grid_error p_error, const std::string &p_description)
  {
    json _out;
    _out["response"] = "error";
    _out["code"]     = p_error;
    _out["desc"]     = p_description;
    m_working        = false;
    TNCT_LOG_ERR(m_logger, fmt(_out.dump()));
    send(_out);
  }

  void configure_callbacks()
  {
    m_grid_creator.on_configuration(
        std::bind_front(&connection::send_configuration, this));

    m_grid_creator.on_permutations_tried(
        std::bind_front(&connection::send_permutations, this));

    m_grid_creator.on_error(
        std::bind_front(&connection::send_grid_error, this));
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
                {"orientation", _stream.str()},
                {"id", p_layout.get_id()}};
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
  statistics                     m_statistics;
  bool                           m_working{false};
};

} // namespace tnct::crosswords::app::server

#endif

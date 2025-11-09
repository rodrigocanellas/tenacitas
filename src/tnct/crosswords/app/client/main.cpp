#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <utility> // Fix for Boost 1.74

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

#include "tnct/crosswords/dat/index.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

namespace beast     = boost::beast;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = net::ip::tcp;
using tnct::program::bus::options;
using logger = tnct::log::cerr;
using nlohmann::json;
using std::chrono::seconds;
using grid_index = tnct::crosswords::dat::index;
using tnct::format::bus::fmt;

void syntax(std::string_view p_pgm)
{
  std::cout << "Syntax: " << p_pgm << std::endl;
}

std::optional<json> read_entries(logger            &p_logger,
                                 const std::string &p_entries_file)
{
  std::ifstream _file{p_entries_file};
  if (!_file.is_open())
  {
    TNCT_LOG_ERR(p_logger, fmt("Could not open file '", p_entries_file, '\''));
    return std::nullopt;
  }

  json _json;

  std::string _line;
  std::size_t _counter{1};

  auto trim = [](std::string &s)
  {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
      s.erase(s.begin());
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
      s.pop_back();
  };

  while (std::getline(_file, _line))
  {
    if (_file.bad())
    {
      TNCT_LOG_ERR(p_logger, fmt("Error reading file ", p_entries_file));
      return std::nullopt;
    }

    if (_line.empty())
    {
      continue;
    }

    std::string::size_type _pos{_line.find('|')};
    if (_pos == std::string::npos)
    {
      TNCT_LOG_ERR(p_logger, fmt("Could not find '|' at line ", _counter));
      return std::nullopt;
    }
    std::string _word{_line.substr(0, _pos)};
    std::string _explanation{_line.substr(_pos + 1)};

    trim(_word);
    trim(_explanation);

    _json.push_back({{"word", _word}, {"explanation", _explanation}});
  }
  return _json;
}

int main(int argc, char **argv)
{
  logger _logger;
  _logger.set_inf();
  try
  {

    options     _options;
    std::string _entries_file;
    grid_index  _rows{std::numeric_limits<grid_index>::max()};
    grid_index  _cols{std::numeric_limits<grid_index>::max()};
    grid_index  _max_rows{std::numeric_limits<grid_index>::max()};
    seconds     _interval{seconds::zero()};

    {
      _options.parse(argc, argv,
                     {"entries", "rows", "cols", "max_rows", "interval"});

      if (const std::optional<options::value> _maybe =
              _options.get_single_param("entries");
          !_maybe.has_value())
      {
        TNCT_LOG_ERR(_logger, "No 'entries' parameter provided");
        syntax(argv[0]);
        return 1;
      }
      else
      {
        _entries_file = std::move(_maybe.value());
      }

      if (std::optional<options::value> _maybe{
              _options.get_single_param("rows")};
          !_maybe.has_value())
      {
        TNCT_LOG_ERR(_logger, "No 'rows' parameter provided");
        syntax(argv[0]);
        return 1;
      }
      else
      {
        _rows = std::stoul(_maybe.value());
      }

      if (std::optional<options::value> _maybe{
              _options.get_single_param("cols")};
          !_maybe.has_value())
      {
        TNCT_LOG_ERR(_logger, "No 'cols' parameter provided");
        syntax(argv[0]);
        return 1;
      }
      else
      {
        _cols = std::stoul(_maybe.value());
      }

      if (std::optional<options::value> _maybe{
              _options.get_single_param("max_rows")};
          !_maybe.has_value())
      {
        TNCT_LOG_ERR(_logger, "No 'max_rows' parameter provided");
        syntax(argv[0]);
        return 1;
      }
      else
      {
        _max_rows = std::stoul(_maybe.value());
      }

      if (std::optional<options::value> _maybe{
              _options.get_single_param("interval")};
          !_maybe.has_value())
      {
        TNCT_LOG_ERR(_logger, "No 'interval' parameter provided");
        syntax(argv[0]);
        return 1;
      }
      else
      {
        _interval = seconds{std::stoul(_maybe.value())};
      }
    }

    std::optional<json> _entries_json{read_entries(_logger, _entries_file)};
    if (!_entries_json.has_value())
    {
      return 1;
    }
    json _request = {{"request", "create"},
                     {"rows", _rows},
                     {"cols", _cols},
                     {"max_rows", _max_rows},
                     {"interval", _interval.count()},
                     {"entries", *_entries_json}};

    std::string _msg{_request.dump()};
    TNCT_LOG_INF(_logger, _msg);

    net::io_context _ioc;

    tcp::resolver                  _resolver{_ioc};
    websocket::stream<tcp::socket> _websocket{_ioc};

    auto const _results = _resolver.resolve("127.0.0.1", "9002");
    net::connect(_websocket.next_layer(), _results.begin(), _results.end());

    _websocket.handshake("127.0.0.1", "/");

    _websocket.write(net::buffer(_msg));

    while (true)
    {

      beast::flat_buffer _buffer;
      _websocket.read(_buffer);

      const std::string _msg     = beast::buffers_to_string(_buffer.data());
      const auto        _payload = json::parse(_msg);
      const std::string _response{_payload.value("response", "")};
      if (_response == "solved")
      {
        TNCT_LOG_INF(_logger, fmt("SOLVED: ", _payload.dump(0)));
        break;
      }
      else if (_response == "unsolved")
      {
        TNCT_LOG_INF(_logger, fmt("UNSOLVED: ", _payload.dump()));
        break;
      }
      else if (_response == "tries")
      {
        TNCT_LOG_INF(_logger, fmt("TRIES: ", _payload.dump()));
      }
      else if (_response == "configuration")
      {
        TNCT_LOG_INF(_logger, fmt("CONFIGURATION: ", _payload.dump()));
      }
      else if (_response == "error")
      {
        TNCT_LOG_INF(_logger, fmt("ERROR: ", _payload.dump()));
      }
      else
      {
        TNCT_LOG_INF(_logger, fmt("UNKNOWN: ", _payload.dump()));
      }
    }

    _websocket.close(websocket::close_code::normal);
  }
  catch (std::exception const &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

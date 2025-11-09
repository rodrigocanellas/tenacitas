#ifndef TNCT_CROSSWORDS_APP_SERVER_STATISTICS_H
#define TNCT_CROSSWORDS_APP_SERVER_STATISTICS_H

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

#include "tnct/async/bus/dispatcher.h"
#include "tnct/async/dat/handling_priority.h"
#include "tnct/async/dat/result.h"
#include "tnct/container/dat/circular_queue.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"

using logger = tnct::log::cerr;

namespace tnct::crosswords::app::server
{

struct start_grid_creation
{
  start_grid_creation()                            = default;
  start_grid_creation(const start_grid_creation &) = default;
  start_grid_creation(start_grid_creation &&)      = default;
  ~start_grid_creation()                           = default;

  start_grid_creation &operator=(const start_grid_creation &) = default;
  start_grid_creation &operator=(start_grid_creation &&)      = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const start_grid_creation &)
  {
    p_out << "start_grid_creation";
    return p_out;
  }
};

struct statistics
{

  statistics(logger &p_logger) : m_logger(p_logger), m_dispatcher{m_logger}
  {
    configure_handlings();
  }

  void grid_creation()
  {
    if (auto _result{m_dispatcher.template publish<start_grid_creation>()};
        _result != async::dat::result::OK)
    {
      TNCT_LOG_ERR(m_logger, "error publishing start_grid_creation'");
      return;
    }
  }

private:
  using dispatcher = async::bus::dispatcher<logger, start_grid_creation>;

private:
  void configure_handlings()
  {
    using queue = container::dat::circular_queue<logger, start_grid_creation, 30>;

    auto _handler{[this](start_grid_creation &&) { on_grid_creation(); }};

    m_dispatcher.template add_handling<start_grid_creation>(
        "start_grid_creation", queue{m_logger}, std::move(_handler),
        async::dat::handling_priority::high);
  }

  void on_grid_creation()
  {
    try
    {
      using namespace std::chrono;

      const auto _now{system_clock::now()};

      const auto     _today = floor<days>(_now);
      year_month_day _ymd   = year_month_day{_today};

      const std::int32_t  _year  = static_cast<std::int32_t>(_ymd.year());
      const std::uint32_t _month = static_cast<std::uint32_t>(_ymd.month());
      const std::uint32_t _day   = static_cast<std::uint32_t>(_ymd.day());

      std::stringstream _stream;

      _stream << std::setfill('0') << _year << "-" << std::setw(2) << _month
              << "-" << std::setw(2) << _day << ".stats";

      std::ofstream _file{_stream.str(), std::ios::ate | std::ios::app};

      std::time_t t  = system_clock::to_time_t(_now);
      std::tm     tm = *std::localtime(&t);

      _stream.str("");
      _stream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

      _file << _stream.str() << '\n';
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, format::bus::fmt("ERROR: ", _ex.what()));
    }
  }

private:
  logger    &m_logger;
  dispatcher m_dispatcher;
};

} // namespace tnct::crosswords::app::server
#endif

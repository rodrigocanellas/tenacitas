/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_CFG_READER_H
#define TENACITAS_LIB_ASYNC_EXP_CFG_READER_H

#include <optional>
#include <utility>

#include "tenacitas.lib/async/exp/events_simulation/cfg/handlings.h"
#include "tenacitas.lib/async/exp/events_simulation/cfg/publishers.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/parser/ini_file.h"
#include "tenacitas.lib/traits/logger.h"

using namespace tenacitas::lib;

namespace tenacitas::lib::async::exp::events_simulation::cfg {

struct reader {
  template <traits::logger t_logger>
  std::optional<std::pair<publishers, handlings>>
  operator()(t_logger &p_logger, std::string_view p_ini_file_name) {
    parser::ini_file<t_logger> _ini_file(p_logger);

    auto _maybe_ini_file_sections{_ini_file.read(p_ini_file_name)};
    if (!_maybe_ini_file_sections) {
      TNCT_LOG_ERR(p_logger, format::fmt("error loading ", p_ini_file_name));
      return std::nullopt;
    }

    auto _ini_file_section{std::move(_maybe_ini_file_sections.value())};

    handlings _handlings;
    publishers _publishers;

    for (const auto &_section_value : _ini_file_section) {
      if (_section_value.first.starts_with("PUBLISHER")) {
        std::optional<publisher> _maybe{
            read_publisher(p_logger, _section_value)};
        if (_maybe) {
          _publishers.push_back(std::move(std::move(_maybe.value())));
        } else {
          return std::nullopt;
        }
      } else if (_section_value.first.starts_with("HANDLING")) {
        std::optional<handling> _maybe{read_handling(p_logger, _section_value)};
        if (_maybe) {
          _handlings.push_back(std::move(std::move(_maybe.value())));
        } else {
          return std::nullopt;
        }
      } else {
        TNCT_LOG_ERR(p_logger, format::fmt('\'', _section_value.first,
                                           "' is not a valid section name"));
        return std::nullopt;
      }
    }
    return {std::make_pair(std::move(_publishers), std::move(_handlings))};
  }

private:
  template <traits::logger t_logger>
  std::optional<handling>
  read_handling(t_logger &p_logger,
                const typename parser::ini_file<t_logger>::sections::value_type
                    &p_section_value) {
    handling _handling;

    for (const auto &_property : p_section_value.second) {
      if (_property.first == "Event") {
        _handling.m_event = _property.second;
      } else if (_property.first == "NumHandlers") {
        _handling.m_num_handlers =
            static_cast<decltype(handling::m_num_handlers)>(
                std::stoi(_property.second));
      } else if (_property.first == "SleepToFakeWorkInMilliSec") {
        _handling.m_sleep_to_fake_work =
            decltype(handling::m_sleep_to_fake_work)(
                static_cast<size_t>(std::stoi(_property.second)));
      } else {
        TNCT_LOG_ERR(p_logger, format::fmt("paragraph ", _property.first,
                                           " is not valid"));
        return std::nullopt;
      }
    }
    return {_handling};
  }

  template <traits::logger t_logger>
  std::optional<publisher>
  read_publisher(t_logger &p_logger,
                 const typename parser::ini_file<t_logger>::sections::value_type
                     &p_section_value) {
    publisher _publisher;

    for (const auto &_property : p_section_value.second) {
      if (_property.first == "Event") {
        _publisher.m_event = _property.second;
      } else if (_property.first == "IntervalInMilliSec") {
        _publisher.m_interval = decltype(publisher::m_interval)(
            static_cast<size_t>(std::stoi(_property.second)));
      } else if (_property.first == "NumEvents") {
        _publisher.m_num_events =
            static_cast<decltype(publisher::m_num_events)>(
                std::stoi(_property.second));
      } else {
        TNCT_LOG_ERR(p_logger, format::fmt("paragraph ", _property.first,
                                           " is not valid"));
        return std::nullopt;
      }
    }
    return {_publisher};
  }
};

} // namespace tenacitas::lib::async::exp::events_simulation::cfg

#endif

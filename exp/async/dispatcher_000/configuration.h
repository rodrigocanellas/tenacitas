/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_EXP_ASYNC_DISPATCHER_CONFIGURATION_H
#define TENACITAS_EXP_ASYNC_DISPATCHER_CONFIGURATION_H

#include <array>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <optional>
#include <string_view>

#include "tenacitas/src/parser/ini_file.h"
#include "tenacitas/src/log/traits/logger.h"

using namespace std::chrono_literals;

namespace tenacitas::exp::async {

template <src::log::traits::logger t_logger, size_t t_num_handlings = 5>
struct configuration {
  struct handling_cfg {
    bool use{false};
    size_t amount_handlers{0};
    std::chrono::milliseconds sleep_to_simulate_work{
        std::chrono::milliseconds::zero()};
  };

  configuration(t_logger &p_logger, std::string_view p_ini_file)
      : m_ini(p_logger) {
    std::optional<typename ini_file::sections> _maybe{m_ini.read(p_ini_file)};
    if (!_maybe) {
      throw std::runtime_error("error reading ini sections");
    }

    typename ini_file::sections _sections{std::move(_maybe.value())};

    typename ini_file::sections::const_iterator _ite_sections{
        _sections.find("PUBLISHER")};
    if (_ite_sections == _sections.end()) {
      throw std::runtime_error("no 'PUBLISHER' section found");
    }

    typename ini_file::properties::const_iterator _ite_properties{
        _ite_sections->second.find("amount_events_to_publish")};
    if (_ite_properties == _ite_sections->second.end()) {
      throw std::runtime_error("no 'amount_events_to_publish' property found");
    }
    if (_ite_properties->second.empty()) {
      throw std::runtime_error("'amount_events_to_publish' property is empty");
    }

    amount_events_to_publish = std::stoul(_ite_properties->second);

    _ite_properties =
        _ite_sections->second.find("interval_for_events_publishing");
    if (_ite_properties == _ite_sections->second.end()) {
      throw std::runtime_error(
          "'interval_for_events_publishing' property not found");
    }
    if (_ite_properties->second.empty()) {
      throw std::runtime_error(
          "'interval_for_events_publishing' property is empty");
    }

    interval_for_events_publishing =
        std::chrono::milliseconds(std::stoul(_ite_properties->second));

    for (decltype(t_num_handlings) _i = 0; _i < t_num_handlings; ++_i) {
      read_handling_cfg(_i, _sections);
    }
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const configuration &p_configuration) {
    p_out << "Publisher:"
          << "\n\tamount_events_to_publish = "
          << p_configuration.amount_events_to_publish
          << "\n\tinterval_for_events_publishing = "
          << p_configuration.interval_for_events_publishing.count() << "ms"
          << '\n';

    size_t _idx{0};
    for (const auto &_handling_cfg : p_configuration.handlings_cfg) {
      p_out << "Handler[" << _idx << "]"
            << "\n\tuse = " << (_handling_cfg.use ? "true" : "false")
            << "\n\tamount_handlers = " << _handling_cfg.amount_handlers
            << "\n\tsleep_to_simulate_work = "
            << _handling_cfg.sleep_to_simulate_work.count() << "ms" << '\n';
      ++_idx;
    }

    return p_out;
  }

  size_t amount_events_to_publish{0};
  std::chrono::milliseconds interval_for_events_publishing{
      std::chrono::milliseconds::zero()};
  std::array<handling_cfg, t_num_handlings> handlings_cfg;

private:
  using ini_file = src::parser::ini_file<t_logger>;

private:
  void read_handling_cfg(size_t p_index,
                         const typename ini_file::sections &p_sections) {
    const std::string _handler{"HANDLING_" + std::to_string(p_index)};
    typename ini_file::sections::const_iterator _ite_sections{
        p_sections.find(_handler)};
    if (_ite_sections == p_sections.end()) {
      throw std::runtime_error("no " + _handler + " section found");
    }

    typename ini_file::properties::const_iterator _ite_properties{
        _ite_sections->second.find("use")};
    if (_ite_properties == _ite_sections->second.end()) {
      throw std::runtime_error("no 'use' property found in " + _handler);
    }
    if (_ite_properties->second.empty()) {
      throw std::runtime_error("'use' property is empty");
    }
    if ((_ite_properties->second != "true") &&
        (_ite_properties->second != "false")) {
      throw std::runtime_error("'use' property is not 'true' neither 'false'");
    }
    bool _use{_ite_properties->second == "true" ? true : false};

    _ite_properties = _ite_sections->second.find("amount_handlers");
    if (_ite_properties == _ite_sections->second.end()) {
      throw std::runtime_error("no 'amount_handlers' property found in " +
                               _handler);
    }
    if (_ite_properties->second.empty()) {
      throw std::runtime_error("'amount_handlers' property is empty");
    }
    size_t _amount_handlers{std::stoul(_ite_properties->second)};

    _ite_properties = _ite_sections->second.find("sleep_to_simulate_work");
    if (_ite_properties == _ite_sections->second.end()) {
      throw std::runtime_error(
          "no 'sleep_to_simulate_work' property found in " + _handler);
    }
    if (_ite_properties->second.empty()) {
      throw std::runtime_error("'sleep_to_simulate_work' property is empty");
    }
    auto _sleep_to_simulate_work{
        std::chrono::milliseconds(std::stoul(_ite_properties->second))};

    handlings_cfg[p_index] = {_use, _amount_handlers, _sleep_to_simulate_work};
  }

private:
  ini_file m_ini;
};

} // namespace tenacitas::exp::async

#endif

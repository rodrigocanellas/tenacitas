/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_CFG_HANDLINGS_H
#define TENACITAS_LIB_ASYNC_EXP_CFG_HANDLINGS_H

#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "tenacitas.lib/async/exp/events_simulation/typ/handling_id.h"

namespace tenacitas::lib::async::exp::events_simulation::cfg {

struct handling final {

  handling() = default;
  handling(const handling &p_handling_cfg) = default;
  handling(handling &&p_handling_cfg) = default;

  handling(std::string_view p_event, size_t p_num_handlers,
           std::chrono::milliseconds p_sleep)
      : m_event(p_event), m_num_handlers(p_num_handlers),
        m_sleep_to_fake_work(p_sleep) {}

  ~handling() = default;

  handling &operator=(const handling &) = default;
  handling &operator=(handling &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const handling &p_handling_cfg) {
    p_out << "{\"id\": " << p_handling_cfg.m_id << ", \"event\": \""
          << p_handling_cfg.m_event << '\"'
          << ", \"num_handlers\": " << p_handling_cfg.m_num_handlers
          << ", \"sleep\": " << p_handling_cfg.m_sleep_to_fake_work.count()
          << '}';
    return p_out;
  }

  typ::handling_id m_id;
  std::string m_event;
  size_t m_num_handlers{0};
  std::chrono::milliseconds m_sleep_to_fake_work{
      std::chrono::milliseconds::zero()};
};

using handlings = std::vector<handling>;

} // namespace tenacitas::lib::async::exp::events_simulation::cfg

#endif

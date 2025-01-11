/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_CFG_PUBLISHERS_H
#define TENACITAS_LIB_ASYNC_EXP_CFG_PUBLISHERS_H

#include <chrono>
#include <iostream>
#include <vector>

namespace tenacitas::lib::async::exp::events_simulation::cfg {

struct publisher {
  using id = size_t;
  publisher(id p_id, std::chrono::milliseconds p_interval, size_t p_num_events)
      : m_id(p_id), m_interval(p_interval), m_num_events(p_num_events) {}
  publisher() = default;
  publisher(const publisher &) = default;
  publisher(publisher &&) = default;
  publisher &operator=(const publisher &) = default;
  publisher &operator=(publisher &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const publisher &p_publisher_cfg) {
    p_out << "{\"id\": " << p_publisher_cfg.m_id << ", \"event\": \""
          << p_publisher_cfg.m_event << '\"'
          << ", \"interval\": " << p_publisher_cfg.m_interval.count()
          << ", \"num_events\": " << p_publisher_cfg.m_num_events << '}';
    return p_out;
  }

  id m_id;
  std::string m_event;
  std::chrono::milliseconds m_interval{std::chrono::milliseconds(80)};
  size_t m_num_events{0};
};

using publishers = std::vector<publisher>;

} // namespace tenacitas::lib::async::exp::events_simulation::cfg

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <iostream>
#include <vector>

#include "tenacitas.lib/async/dispatcher.h"
#include "tenacitas.lib/async/sleeping_loop.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/traits/dispatcher.h"
#include "tenacitas.lib/traits/event.h"

using namespace std::chrono_literals;
using namespace tenacitas::lib;

using logger = log::cerr;

struct event_a {
  friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
    return p_out;
  }
};

template <traits::event t_event,
          traits::dispatcher<logger, t_event> t_dispacther>
struct publisher {

  publisher(t_dispacther &p_dispatcher, logger &p_logger,
            std::chrono::milliseconds p_interval, size_t p_total_events,
            std::string p_id)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_total_events(p_total_events), m_interval(p_interval), m_id(p_id),
        m_slepping_loop(
            m_logger, [this]() { sleeping_function(); }, m_interval, m_id)

  {}

  publisher(const publisher &) = delete;
  publisher(publisher &&p_publisher)
      : m_logger{p_publisher.m_logger}, m_dispatcher(p_publisher.m_dispatcher),
        m_total_events(p_publisher.m_total_events),
        m_num_events{p_publisher.m_num_events},
        m_interval(p_publisher.m_interval), m_id{p_publisher.m_id},
        m_slepping_loop(
            m_logger, [this]() { sleeping_function(); }, m_interval, m_id) {}
  ~publisher() { m_slepping_loop.stop(); }

  void start() {
    TNCT_LOG_TST(m_logger, format::fmt("starting pub ", m_id));
    m_slepping_loop.start();
  }

  constexpr size_t num_events() const { return m_num_events; }

private:
  void sleeping_function() {
    if (m_num_events >= m_total_events) {
      TNCT_LOG_TST(m_logger, format::fmt("stoping ", m_id));
      m_slepping_loop.stop();
      return;
    }
    // m_dispatcher.template publish<t_event>();
    ++m_num_events;
    TNCT_LOG_TST(m_logger,
                 format::fmt("publisher ", m_id, " event ", m_num_events));
  }

private:
  logger &m_logger;
  t_dispacther &m_dispatcher;
  size_t m_total_events{0};
  size_t m_num_events{0};
  std::chrono::milliseconds m_interval;
  std::string m_id{"not-assigned"};
  async::sleeping_loop<logger> m_slepping_loop;
};

template <traits::event t_event,
          traits::dispatcher<logger, t_event> t_dispacther>
using publishers = std::vector<publisher<t_event, t_dispacther>>;

using dispatcher = async::dispatcher<logger, event_a>;

int main() {
  logger _logger;
  // _logger.set_deb();
  dispatcher _dispatcher(_logger);

  publishers<event_a, dispatcher> _publishers;

  _publishers.push_back({_dispatcher, _logger, 1s, 10, "a"});
  _publishers.push_back({_dispatcher, _logger, 500ms, 20, "b"});

  // TNCT_LOG_TST(_logger, format::fmt("#pubs ", _publishers.size()));
  for (auto &_publisher : _publishers) {
    _publisher.start();
  }

  std::this_thread ::sleep_for(30s);

  for (auto &_publisher : _publishers) {
    std::cout << "# events " << _publisher.num_events() << std::endl;
  }

  // publisher<event_a, dispatcher> _publisher1{_dispatcher, _logger, 1s, 10};
  // publisher<event_a, dispatcher> _publisher2{_dispatcher, _logger, 500ms,
  // 20};

  // _publisher1.start();
  // _publisher2.start();

  // std::this_thread ::sleep_for(11s);

  // std::cout << _publisher1.num_events() << ", " << _publisher1.num_events()
  //           << std::endl;
}

#ifndef TENACITAS_TST_ASYNC_DISPATCHER_TYP_H
#define TENACITAS_TST_ASYNC_DISPATCHER_TYP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/src/async/cpt/event.h>
#include <tenacitas.lib/src/number/typ/id.h>

using namespace std::chrono_literals;

using namespace tenacitas::lib;

namespace typ {

using test_id = uint32_t;
using device_id = number::typ::id;
using pressure = float;

using total = uint32_t;
using total_list = std::vector<total>;
using amount = uint32_t;
using amount_list = std::vector<amount>;
using amount_increment = uint32_t;
using time = std::chrono::milliseconds;
using time_list = std::vector<time>;
using time_increment = std::chrono::milliseconds;

struct summary {
  summary() = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const summary &p_summary) {
    p_out << "sent expected = " << p_summary.total_sent_expected
          << ", sent = " << p_summary.total_sent
          << ", handled expected = " << p_summary.total_handled_expected
          << ", handled = " << p_summary.total_handled
          << ", duration = " << p_summary.duration.count() << " microsecs";
    return p_out;
  }

  amount total_sent_expected{0};
  amount total_handled_expected{0};
  amount total_sent{0};
  amount total_handled{0};
  std::chrono::microseconds duration{0ms};
};

struct generators_definitions {

  generators_definitions() = default;

  generators_definitions(total p_total, amount p_amount_events,
                         amount_increment p_amount_events_increment,
                         time p_interval, time_increment p_interval_increment)
      : m_total(p_total), m_amount_events(p_amount_events),
        m_amount_events_increment(p_amount_events_increment),
        m_interval(p_interval), m_interval_increment(p_interval_increment) {}

  generators_definitions(const generators_definitions &) = default;
  generators_definitions(generators_definitions &&) = default;
  generators_definitions &operator=(const generators_definitions &) = default;
  generators_definitions &operator=(generators_definitions &&) = default;
  ~generators_definitions() = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const generators_definitions &p_generators) {
    p_out << "total = " << p_generators.m_total
          << ", amount events = " << p_generators.m_amount_events
          << ", amount events increment = " << p_generators.m_amount_events
          << ", "
          << ", interval = " << p_generators.m_interval.count()
          << ", interval increment = "
          << p_generators.m_interval_increment.count();
    return p_out;
  }

  total m_total;
  amount m_amount_events;
  amount_increment m_amount_events_increment{0};
  time m_interval;
  time_increment m_interval_increment{0ms};
};

struct publishings_definitions {

  publishings_definitions() = default;

  publishings_definitions(total p_total, amount p_amount_subscribers,
                          amount_increment p_amount_subscribers_increment,
                          time p_sleep, time_increment p_sleep_increment)
      : m_total(p_total), m_amount_subscribers(p_amount_subscribers),
        m_amount_subscribers_increment(p_amount_subscribers_increment),
        m_sleep(p_sleep), m_sleep_increment(p_sleep_increment) {}

  publishings_definitions(const publishings_definitions &) = default;
  publishings_definitions(publishings_definitions &&) = default;
  publishings_definitions &operator=(const publishings_definitions &) = default;
  publishings_definitions &operator=(publishings_definitions &&) = default;
  ~publishings_definitions() = default;

  friend std::ostream &
  operator<<(std::ostream &p_out,
             const publishings_definitions &p_publishings) {
    p_out << "total = " << p_publishings.m_total
          << ", amount subscribers = " << p_publishings.m_amount_subscribers
          << ", amount subscribers increment = "
          << p_publishings.m_amount_subscribers_increment << ", "
          << ", sleep = " << p_publishings.m_sleep.count()
          << ", sleep increment = " << p_publishings.m_sleep_increment.count();
    return p_out;
  }

  total m_total;
  amount m_amount_subscribers{0};
  amount_increment m_amount_subscribers_increment{0};
  time m_sleep{0ms};
  time_increment m_sleep_increment{0ms};
};

struct generator {
  generator() = default;
  generator(number::typ::id p_id, amount p_generated, time p_interval)
      : id(p_id), to_generate(p_generated), interval(p_interval) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const generator &p_generator) {
    p_out << "id = " << p_generator.id
          << ", amount = " << p_generator.to_generate
          << ", interval = " << p_generator.interval.count();
    return p_out;
  }

  number::typ::id id;
  amount to_generate{0};
  time interval{0ms};
};

using generators = std::list<generator>;

using subscribers_results = std::map<number::typ::id, total>;

struct publishing_results {
  publishing_results() = default;
  publishing_results(time p_sleep, subscribers_results p_subscribers)
      : sleep(p_sleep), subscribers(p_subscribers) {}

  time sleep{0ms};
  subscribers_results subscribers;
};

using publishings_results = std::map<size_t, publishing_results>;

} // namespace typ

#endif // DAT_H

#ifndef TENACITAS_TST_ASYNC_DISPATCHER_BUS_H
#define TENACITAS_TST_ASYNC_DISPATCHER_BUS_H

#include <condition_variable>
#include <cstdint>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <thread>
#include <utility>
#include <vector>

#include "dispatcher.h"
#include "evt.h"
#include "typ.h"
#include <tenacitas.lib/async/alg/dispatcher.h>
#include <tenacitas.lib/async/alg/sleeping_loop.h>
#include <tenacitas.lib/calendar/cpt/chrono_convertible.h>
#include <tenacitas.lib/log/alg/logger.h>

using namespace tenacitas::lib;

namespace bus {

// pressure_generator
struct pressure_generator {

  pressure_generator(dispatcher::ptr p_dispatcher, typ::pressure p_initial,
                     typ::generator p_generator)
      : m_dispatcher(p_dispatcher), m_pressure(p_initial),
        m_generator(p_generator) {}

  pressure_generator() = delete;
  pressure_generator(pressure_generator &&) = default;
  pressure_generator(const pressure_generator &) = delete;
  pressure_generator &operator=(pressure_generator &&) = delete;
  pressure_generator &operator=(const pressure_generator &) = delete;
  ~pressure_generator() = default;

  void start() {
    if (!m_ticker) {
      m_ticker = std::make_unique<async::alg::sleeping_loop>(
          [this]() -> void { (*this)(); }, m_generator.interval);
    }
    m_ticker->start();
  }

  void stop() {
    if (m_ticker) {
      m_ticker->stop();
    }
  }

  std::pair<typ::amount, typ::time> get() const {
    return {m_generator.to_generate, m_generator.interval};
  }

private:
  void operator()() {
    if (m_counter >= (m_generator.to_generate - 1)) {
      stop();
    }
    ++m_counter;
    m_pressure += 0.5;
    TNCT_LOG_TST("generator ", m_generator.id, " generating pressure #",
                 m_counter, " = ", m_pressure);
    m_dispatcher->publish<evt::pressure_generated>(m_generator.id, m_pressure);
    TNCT_LOG_TST("evt::pressure_generated published by generator ",
                 m_generator.id, ", pressure #", m_counter, " = ", m_pressure);

    m_dispatcher->publish<evt::pressure_sent>(m_generator.id);
    TNCT_LOG_TST("evt::pressure_sent published for generator ", m_generator.id);
  }

private:
  dispatcher::ptr m_dispatcher;

  typ::pressure m_pressure;

  std::unique_ptr<async::alg::sleeping_loop> m_ticker;
  typ::amount m_counter{0};
  typ::device_id m_device;

  typ::generator m_generator;
};

using pressure_generators = std::vector<pressure_generator>;

using storager = std::function<std::optional<typ::test_id>(
    typ::summary &&p_summary, typ::generators &&p_generators,
    typ::publishings_results &&p_publishings)>;

struct pressure_tester {
  pressure_tester(dispatcher::ptr p_dispatcher, storager p_storager,
                  const typ::generators_definitions &p_generator_definition,
                  const typ::publishings_definitions &p_publishing_definition)
      : m_dispatcher(p_dispatcher), m_storager(p_storager) {
    m_dispatcher->stop();
    m_summary = typ::summary{};
    setup_generators(p_generator_definition);
    setup_publishings(p_publishing_definition);
  }

  ~pressure_tester() { stop_generators(); }

  bool run() {
    if (m_pressure_generators.size() == 0) {
      TNCT_LOG_ERR("no genenerator was created");
      return false;
    }

    if (m_num_subscribers == 0) {
      TNCT_LOG_ERR("no subscriber was created");
      return false;
    }

    TNCT_LOG_TST(
        "amount sent expected = ", m_summary.total_sent_expected,
        ", amount handled expected = ", m_summary.total_handled_expected);

    auto _start = std::chrono::high_resolution_clock::now();

    start_generators();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&]() -> bool {
        TNCT_LOG_TST("checking if finished");
        bool _finished = finished();
        TNCT_LOG_TST("_finished = ", _finished);
        return _finished;
      });
    }

    auto _finish = std::chrono::high_resolution_clock::now();

    m_summary.duration =
        std::chrono::duration_cast<std::chrono::microseconds>(_finish - _start);

    TNCT_LOG_INF("***** REPORT START *****");
    std::optional<typ::test_id> _maybe =
        m_storager(std::move(m_summary), std::move(m_generators),
                   std::move(m_publishings_results));
    if (!_maybe) {
      TNCT_LOG_ERR("error saving results");
    } else {
      TNCT_LOG_INF("test id = ", _maybe.value());
    }

    report_summary();
    report_generators();
    report_subscribers();
    TNCT_LOG_INF("***** REPORT END *****");

    return finished();
  }

private:
  void add_publishing(uint16_t p_num_subscribers,
                      calendar::cpt::convertible_to_ms auto p_sleep) {
    auto _queue_id = m_dispatcher->add_queue<evt::pressure_generated>();
    m_summary.total_handled_expected += m_summary.total_sent_expected;
    TNCT_LOG_TST("p_num_subscribers = ", p_num_subscribers,
                 ", p_sleep = ", p_sleep.count());

    for (uint16_t _i = 0; _i < p_num_subscribers; ++_i) {
      ++m_num_subscribers;

      number::typ::id _subscriber_id;

      m_publishings_results[_queue_id] =
          typ::publishing_results{p_sleep, typ::subscribers_results{}};

      TNCT_LOG_TST("subscriber #", _i, " for pressure_generated");
      m_dispatcher->subscribe<evt::pressure_generated>(
          _queue_id,
          [this, _queue_id, _subscriber_id, p_sleep](auto p_pressure) {
            TNCT_LOG_TST("publishing ", _queue_id, ", subscriber ",
                         _subscriber_id, ", pressure = ", p_pressure,
                         ", about to sleep ", p_sleep.count());

            if (p_sleep.count()) {
              std::this_thread::sleep_for(p_sleep);
            }

            TNCT_LOG_TST("publishing ", _queue_id, ", subscriber ",
                         _subscriber_id, ", pressure = ", p_pressure,
                         ", woke up");

            TNCT_LOG_TST("publishing ", _queue_id, ", subscriber ",
                         _subscriber_id, ", pressure = ", p_pressure,
                         ", publishing 'pressure_handled'");
            m_dispatcher->publish<evt::pressure_handled>(_queue_id,
                                                         _subscriber_id);
          });
    }
  }

  void add_publishing(uint16_t p_num_subscribers) {
    add_publishing(p_num_subscribers, 0s);
  }

  void add_generator(typ::time p_interval, typ::pressure p_initial,
                     typ::amount p_amount) {
    typ::generator _generator{number::typ::id{}, p_amount, p_interval};
    m_generators.push_back(_generator);
    m_pressure_generators.push_back({m_dispatcher, p_initial, _generator});
    m_summary.total_sent_expected += p_amount;
  }

  void setup_generators(const typ::generators_definitions &p_generator) {
    stop_generators();
    m_pressure_generators.clear();
    m_generators.clear();

    decltype(p_generator.m_amount_events) _current_amount_events{
        p_generator.m_amount_events};
    decltype(p_generator.m_interval) _current_interval{p_generator.m_interval};
    for (decltype(p_generator.m_total) _generator_count = 0;
         _generator_count < p_generator.m_total; ++_generator_count) {
      add_generator(_current_interval, 5, _current_amount_events);
      _current_interval += p_generator.m_interval_increment;
      _current_amount_events += p_generator.m_amount_events_increment;
    }
  }

  void setup_publishings(const typ::publishings_definitions &p_publishing) {

    // subscriber for pressure sent
    m_dispatcher->subscribe<evt::pressure_sent>(
        [this](evt::pressure_sent &&p_pressure_sent) -> void {
          std::lock_guard<std::mutex> _lock(m_pressure_sent_subscriber);

          ++m_summary.total_sent;
          TNCT_LOG_TST("pressure sent: ", p_pressure_sent,
                       ", sent so far: ", m_summary.total_sent);
          m_cond.notify_all();
        });

    // subscriber for pressure handled
    m_dispatcher->subscribe<evt::pressure_handled>([&](auto p_handled) -> void {
      std::lock_guard<std::mutex> _lock(m_pressure_handled_subscriber);

      ++m_publishings_results[p_handled.queue]
            .subscribers[p_handled.subscriber];

      ++m_summary.total_handled;
      TNCT_LOG_TST("handled so far: ", m_summary.total_handled);
      m_cond.notify_all();
    });

    m_publishings_results.clear();
    auto _current_amount_subscribers = p_publishing.m_amount_subscribers;
    auto _current_sleep = p_publishing.m_sleep;
    for (decltype(p_publishing.m_total) _publishing_count = 0;
         _publishing_count < p_publishing.m_total; ++_publishing_count) {
      TNCT_LOG_TST("sleep = ", p_publishing.m_sleep.count());
      add_publishing(_current_amount_subscribers, _current_sleep);
      _current_amount_subscribers +=
          p_publishing.m_amount_subscribers_increment;
      _current_sleep += p_publishing.m_sleep_increment;
    }
  }

  bool finished() const {
    return (m_summary.total_sent == m_summary.total_sent_expected) &&
           (m_summary.total_handled == m_summary.total_handled_expected);
  }

  void start_generators() {
    for (pressure_generator &p_pressure_generator : m_pressure_generators) {
      p_pressure_generator.start();
    }
  }

  void stop_generators() {
    for (pressure_generator &p_pressure_generator : m_pressure_generators) {
      p_pressure_generator.stop();
    }
  }

  void report_subscribers() const {
    for (const typ::publishings_results::value_type &_value_publishing :
         m_publishings_results) {
      TNCT_LOG_INF("publishing = ", _value_publishing.first,
                   ", sleep = ", _value_publishing.second.sleep.count());
      for (const typ::subscribers_results::value_type &_value_subscriber :
           _value_publishing.second.subscribers) {
        TNCT_LOG_INF('\t', "subscriber = ", _value_subscriber.first,
                     ", amount = ", _value_subscriber.second);
      }
    }
  }

  void report_generators() const {
    for (const typ::generator &_generator : m_generators) {
      TNCT_LOG_INF("generator: ", _generator);
    }
  }

  void report_summary() { TNCT_LOG_INF("summary: ", m_summary); }

private:
  dispatcher::ptr m_dispatcher;
  storager m_storager;
  pressure_generators m_pressure_generators;
  typ::generators m_generators;
  typ::publishings_results m_publishings_results;
  typ::summary m_summary;

  std::mutex m_pressure_sent_subscriber;
  std::mutex m_pressure_handled_subscriber;

  uint16_t m_num_subscribers{0};

  std::mutex m_mutex;
  std::condition_variable m_cond;
};

struct processor {
  processor(storager &&p_sto) : m_sto(p_sto) {}

  bool operator()(dispatcher::ptr p_dispatcher,
                  const typ::generators_definitions &p_generator,
                  const typ::publishings_definitions &p_publishing) {
    TNCT_LOG_TST("generators definitions: ", p_generator);
    TNCT_LOG_TST("publishings definitions: ", p_publishing);
    pressure_tester _pressure_tester(
        p_dispatcher, m_sto, std::move(p_generator), std::move(p_publishing));

    return _pressure_tester.run();
  }

private:
  storager m_sto;
};

} // namespace bus

#endif // BUS_H

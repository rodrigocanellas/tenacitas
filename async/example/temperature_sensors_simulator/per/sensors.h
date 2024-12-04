#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H

#include <algorithm>
#include <memory>
#include <set>

#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/add_sensor.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/set_temperature.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/per/sensor.h"
#include "tenacitas.lib/async/handling_priority.h"
#include "tenacitas.lib/async/result.h"
#include "tenacitas.lib/generic/fmt.h"
#include "tenacitas.lib/traits/dispatcher.h"
#include "tenacitas.lib/traits/logger.h"
#include "tenacitas.lib/traits/tuple_contains_tuple.h"

using namespace std::chrono_literals;

namespace temperature_sensors_simulator::per {
template <traits::logger t_logger, traits::dispatcher t_dispatcher>
requires(traits::tuple_contains_tuple<
         typename t_dispatcher::events,
         std::tuple<evt::add_sensor, evt::remove_sensor, evt::set_temperature>>)

    struct sensors {
  using events_subscribed =
      std::tuple<evt::add_sensor, evt::remove_sensor, evt::set_temperature>;

  sensors(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher) {

    auto _result(m_dispatcher.template subscribe<sensors, evt::add_sensor>(
        "add-sensor", *this, async::handling_priority::medium, 1));
    if (_result != async::result::OK) {
      m_logger.err(generic::fmt(_result));
    }

    _result = m_dispatcher.template subscribe<sensors, evt::remove_sensor>(
        "remove-sensor", *this, async::handling_priority::medium, 1);
    if (_result != async::result::OK) {
      m_logger.err(generic::fmt(_result));
    }

    _result = m_dispatcher.template subscribe<sensors, evt::set_temperature>(
        "set-temp", *this, async::handling_priority::medium, 1);
    if (_result != async::result::OK) {
      m_logger.err(generic::fmt(_result));
    }
  }

  void handle(evt::add_sensor &&p_evt) { on_add_sensor(std::move(p_evt)); }

  void handle(evt::remove_sensor &&p_evt) {
    on_remove_sensor(std::move(p_evt));
  }

  void handle(evt::set_temperature &&p_evt) {
    on_set_temperature(std::move(p_evt));
  }

private:
  using sensor_ptr = std::unique_ptr<sensor<t_logger, t_dispatcher>>;

  struct sensor_cmp {
    bool operator()(const sensor_ptr &p_s1, const sensor_ptr &p_s2) const {
      return (*p_s1) < (*p_s2);
    }
  };

  using collection = std::set<sensor_ptr, sensor_cmp>;
  using iterator = collection::iterator;
  using const_iterator = collection::const_iterator;

private:
  void on_add_sensor(evt::add_sensor &&p_evt) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    auto _sensor = std::make_unique<sensor<t_logger, t_dispatcher>>(
        m_logger, m_dispatcher, 500ms, p_evt.sensor_id, typ::temperature{25.5},
        typ::temperature{0.75});
    _sensor->start();
    m_collection.insert(std::move(_sensor));
  }

  void on_remove_sensor(evt::remove_sensor &&p_evt) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    iterator _ite = find(p_evt.sensor_id);
    if (_ite != m_collection.end()) {
      m_collection.erase(_ite);
    }
  }

  void on_set_temperature(evt::set_temperature &&p_evt) {
    iterator _ite = find(p_evt.sensor_id);
    if (_ite != m_collection.end()) {
      (*_ite)->reset_temperature(p_evt.temperature);
    }
  }

  iterator find(typ::sensor_id p_sensor_id) {
    return std::find_if(m_collection.begin(), m_collection.end(),
                        [&](const sensor_ptr &p_sensor) {
                          return p_sensor->get_id() == p_sensor_id;
                        });
  }

private:
  t_logger &m_logger;
  t_dispatcher &m_dispatcher;
  collection m_collection;
  std::mutex m_mutex;
};

} // namespace temperature_sensors_simulator::per

#endif // SENSORS_H

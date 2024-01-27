#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H

#include <algorithm>
#include <chrono>
#include <memory>
#include <set>

#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/dispatcher.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/add_sensor.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/remove_sensor.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/set_temperature.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/per/sensor.h>

using namespace std::chrono_literals;

namespace temperature_sensors_simulator::per {

struct sensors {
  using events_subscribed =
      std::tuple<asy::add_sensor, asy::remove_sensor, asy::set_temperature>;

  sensors(asy::dispatcher::ptr p_dispatcher) : m_dispatcher(p_dispatcher) {
    m_dispatcher->subscribe<sensors, asy::add_sensor>(
        [this](auto p_evt) { on_add_sensor(std::move(p_evt)); });

    m_dispatcher->subscribe<sensors, asy::remove_sensor>(
        [this](auto p_evt) { on_remove_sensor(std::move(p_evt)); });

    m_dispatcher->subscribe<sensors, asy::set_temperature>(
        [this](auto p_evt) { on_set_temperature(std::move(p_evt)); });
  }

private:
  using sensor_ptr = std::unique_ptr<sensor>;

  struct sensor_cmp {
    bool operator()(const sensor_ptr &p_s1, const sensor_ptr &p_s2) const {
      return (*p_s1) < (*p_s2);
    }
  };

  using collection = std::set<sensor_ptr, sensor_cmp>;
  using iterator = collection::iterator;
  using const_iterator = collection::const_iterator;

private:
  void on_add_sensor(asy::add_sensor &&p_evt) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    auto _sensor = std::make_unique<sensor>(
        m_dispatcher, 500ms, p_evt.sensor_id, dom::temperature{25.5},
        dom::temperature{0.75});
    _sensor->start();
    m_collection.insert(std::move(_sensor));
  }

  void on_remove_sensor(asy::remove_sensor &&p_evt) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    iterator _ite = find(p_evt.sensor_id);
    if (_ite != m_collection.end()) {
      m_collection.erase(_ite);
    }
  }

  void on_set_temperature(asy::set_temperature &&p_evt) {
    iterator _ite = find(p_evt.sensor_id);
    if (_ite != m_collection.end()) {
      (*_ite)->reset_temperature(p_evt.temperature);
    }
  }

  iterator find(dom::sensor_id p_sensor_id) {
    return std::find_if(m_collection.begin(), m_collection.end(),
                        [&](const sensor_ptr &p_sensor) {
                          return p_sensor->get_id() == p_sensor_id;
                        });
  }

private:
  asy::dispatcher::ptr m_dispatcher;
  collection m_collection;
  std::mutex m_mutex;
};

} // namespace temperature_sensors_simulator::per

#endif // SENSORS_H

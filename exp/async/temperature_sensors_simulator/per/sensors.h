#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H

#include <algorithm>
#include <chrono>
#include <memory>
#include <set>
#include <thread>

#include "../alg/dispatcher.h"
#include "../evt/add_sensor.h"
#include "../evt/remove_sensor.h"
#include "../evt/set_temperature.h"
#include "../per/sensor.h"

using namespace tenacitas::lib;
using namespace std::chrono_literals;

namespace temperature_sensors_simulator::per {

struct sensors {
  using events_subscribed =
      std::tuple<evt::add_sensor, evt::remove_sensor, evt::set_temperature>;

  sensors(alg::dispatcher::ptr p_dispatcher) : m_dispatcher(p_dispatcher) {
    m_dispatcher->subscribe<sensors, evt::add_sensor>(
        [this](auto p_evt) { on_add_sensor(std::move(p_evt)); });

    m_dispatcher->subscribe<sensors, evt::remove_sensor>(
        [this](auto p_evt) { on_remove_sensor(std::move(p_evt)); });

    m_dispatcher->subscribe<sensors, evt::set_temperature>(
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
  void on_add_sensor(evt::add_sensor &&p_evt) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    auto _sensor = std::make_unique<sensor>(
        m_dispatcher, 500ms, p_evt.sensor_id, dat::temperature{25.5},
        dat::temperature{0.75});
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

  iterator find(dat::sensor_id p_sensor_id) {
    return std::find_if(m_collection.begin(), m_collection.end(),
                        [&](const sensor_ptr &p_sensor) {
                          return p_sensor->get_id() == p_sensor_id;
                        });
  }

private:
  alg::dispatcher::ptr m_dispatcher;
  collection m_collection;
  std::mutex m_mutex;
};

} // namespace temperature_sensors_simulator::per

#endif // SENSORS_H

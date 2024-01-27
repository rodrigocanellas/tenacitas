#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H

#include <tuple>

#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/dispatcher.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/new_temperature.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/sensor_id.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/temperature.h>

#include <tenacitas.lib/asy/sleeping_loop.h>

namespace temperature_sensors_simulator::per {

struct sensor {
  using events_published = std::tuple<asy::new_temperature>;
  sensor() = delete;
  sensor(const sensor &) = delete;
  sensor(sensor &) = delete;
  sensor &operator=(const sensor &) = delete;
  sensor &operator=(sensor &) = delete;

  template <typename t_time>
  sensor(asy::dispatcher::ptr p_dispatcher, t_time p_interval,
         dom::sensor_id p_sensor_id, dom::temperature p_initial,
         dom::temperature p_increment)
      : m_dispatcher(p_dispatcher), m_sensor_id(p_sensor_id),
        m_current(p_initial), m_increment(p_increment),
        m_sleeping_loop(
            [this]() {
              {
                std::lock_guard<std::mutex> _lock(m_mutex);
                m_current += m_increment;
              }
              m_dispatcher->publish<sensor, asy::new_temperature>(m_sensor_id,
                                                                  m_current);
            },
            p_interval) {}

  ~sensor() = default;

  void start() { m_sleeping_loop.start(); }

  void stop() { m_sleeping_loop.stop(); }

  bool operator<(const sensor &p_sensor) const {
    return m_sensor_id < p_sensor.m_sensor_id;
  }

  bool operator>(const sensor &p_sensor) const {
    return m_sensor_id > p_sensor.m_sensor_id;
  }

  bool operator==(const sensor &p_sensor) const {
    return m_sensor_id == p_sensor.m_sensor_id;
  }

  bool operator!=(const sensor &p_sensor) const {
    return m_sensor_id != p_sensor.m_sensor_id;
  }

  void reset_temperature(dom::temperature p_temperature) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_current = p_temperature;
  }

  dom::sensor_id get_id() const { return m_sensor_id; }

private:
  asy::dispatcher::ptr m_dispatcher;
  dom::sensor_id m_sensor_id;
  dom::temperature m_current;
  dom::temperature m_increment;
  tenacitas::lib::asy::sleeping_loop m_sleeping_loop;
  std::mutex m_mutex;
};

} // namespace temperature_sensors_simulator::per

#endif // SENSOR_H

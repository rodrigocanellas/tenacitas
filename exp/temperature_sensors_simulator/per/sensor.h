#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H

#include <tuple>

#include "../alg/dispatcher.h"
#include "../evt/new_temperature.h"
#include "../typ/sensor_id.h"
#include "../typ/temperature.h"

#include <tenacitas.lib/src/alg/sleeping_loop.h>

namespace temperature_sensors_simulator::per {

struct sensor {
  using events_published = std::tuple<evt::new_temperature>;
  sensor() = delete;
  sensor(const sensor &) = delete;
  sensor(sensor &) = delete;
  sensor &operator=(const sensor &) = delete;
  sensor &operator=(sensor &) = delete;

  template <typename t_time>
  sensor(alg::dispatcher::ptr p_dispatcher, t_time p_interval,
         typ::sensor_id p_sensor_id, typ::temperature p_initial,
         typ::temperature p_increment)
      : m_dispatcher(p_dispatcher), m_sensor_id(p_sensor_id),
        m_current(p_initial), m_increment(p_increment),
        m_sleeping_loop(
            [this]() {
              {
                std::lock_guard<std::mutex> _lock(m_mutex);
                m_current += m_increment;
              }
              m_dispatcher->publish<sensor, evt::new_temperature>(m_sensor_id,
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

  void reset_temperature(typ::temperature p_temperature) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_current = p_temperature;
  }

  typ::sensor_id get_id() const { return m_sensor_id; }

private:
  alg::dispatcher::ptr m_dispatcher;
  typ::sensor_id m_sensor_id;
  typ::temperature m_current;
  typ::temperature m_increment;
  tla::sleeping_loop m_sleeping_loop;
  std::mutex m_mutex;
};

} // namespace temperature_sensors_simulator::per

#endif // SENSOR_H

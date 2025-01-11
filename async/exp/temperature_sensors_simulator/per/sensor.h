#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H

#include <tuple>

#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/new_temperature.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/typ/sensor_id.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/typ/temperature.h"
#include "tenacitas.lib/async/result.h"
#include "tenacitas.lib/async/sleeping_loop.h"
#include "tenacitas.lib/generic/fmt.h"
#include "tenacitas.lib/traits/dispatcher.h"
#include "tenacitas.lib/traits/logger.h"
#include "tenacitas.lib/traits/tuple_contains_tuple.h"

#include "tenacitas.lib/async/sleeping_loop.h"

using namespace tenacitas::lib;

namespace temperature_sensors_simulator::per {

template <traits::logger t_logger, traits::dispatcher t_dispatcher>
requires(traits::tuple_contains_tuple<typename t_dispatcher::events,
                                      std::tuple<evt::new_temperature>>)

    struct sensor {
  using events_published = std::tuple<evt::new_temperature>;
  sensor() = delete;
  sensor(const sensor &) = delete;
  sensor(sensor &) = delete;
  sensor &operator=(const sensor &) = delete;
  sensor &operator=(sensor &) = delete;

  template <typename t_time>
  sensor(t_logger &p_logger, t_dispatcher &p_dispatcher, t_time p_interval,
         typ::sensor_id p_sensor_id, typ::temperature p_initial,
         typ::temperature p_increment)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_sensor_id(p_sensor_id), m_current(p_initial),
        m_increment(p_increment),
        m_sleeping_loop(
            m_logger,
            [&]() {
              {
                std::lock_guard<std::mutex> _lock(m_mutex);
                m_current += m_increment;
              }
              auto _result(
                  m_dispatcher.template publish<sensor, evt::new_temperature>(
                      m_sensor_id, m_current));
              if (_result != async::result::OK) {
                m_logger.err(generic::fmt(_result));
              }
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
  t_logger &m_logger;
  t_dispatcher &m_dispatcher;
  typ::sensor_id m_sensor_id;
  typ::temperature m_current;
  typ::temperature m_increment;
  async::sleeping_loop<t_logger> m_sleeping_loop;
  std::mutex m_mutex;
};

} // namespace temperature_sensors_simulator::per

#endif // SENSOR_H

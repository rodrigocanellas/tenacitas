#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H

#include <tuple>

#include "tnct/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/typ/sensor_id.h"
#include "tnct/async/exp/temperature_sensors_simulator/typ/temperature.h"
#include "tnct/async/result.h"
#include "tnct/async/sleeping_loop.h"
#include "tnct/format/fmt.h"
#include "tnct/traits/async/dispatcher.h"
#include "tnct/traits/async/publish_method.h"
#include "tnct/traits/log/logger.h"

using namespace tnct;

namespace tnct::async::exp::temperature_sensors_simulator::per {

template <traits::log::logger t_logger, traits::async::dispatcher t_dispatcher>
requires(

    traits::async::dispatcher<t_dispatcher, evt::new_temperature>

        &&

            traits::async::has_const_lvalue_publish_method<
                t_dispatcher, async::result, evt::new_temperature>

                &&

                    traits::async::has_variadic_params_publish_method<
                        t_dispatcher, async::result, evt::new_temperature>

    )

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
              auto _result(m_dispatcher.template publish<evt::new_temperature>(
                  m_sensor_id, m_current));
              if (_result != async::result::OK) {
                m_logger.err(format::fmt(_result));
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

} // namespace tnct::async::exp::temperature_sensors_simulator::per

#endif // SENSOR_H

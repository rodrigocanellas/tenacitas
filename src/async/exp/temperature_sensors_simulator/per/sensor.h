#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H

#include <tuple>

#include "tenacitas/src/async/exp/temperature_sensors_simulator/dat/sensor_id.h"
#include "tenacitas/src/async/exp/temperature_sensors_simulator/dat/temperature.h"
#include "tenacitas/src/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/async/sleeping_loop.h"
#include "tenacitas/src/async/traits/has_publish_method.h"
#include "tenacitas/src/async/traits/is_dispatcher.h"
#include "tenacitas/src/format/fmt.h"
#include "tenacitas/src/log/traits/logger.h"

using namespace tenacitas;

namespace tenacitas::src::async::exp::temperature_sensors_simulator::per
{

template <src::log::traits::logger          t_logger,
          src::async::traits::is_dispatcher t_dispatcher>
requires(src::async::traits::is_dispatcher<t_dispatcher, evt::new_temperature>
         && src::async::traits::has_publish_method<t_dispatcher,
                                                   evt::new_temperature>)

struct sensor
{
  using events_published            = std::tuple<evt::new_temperature>;
  sensor()                          = delete;
  sensor(const sensor &)            = delete;
  sensor(sensor &)                  = delete;
  sensor &operator=(const sensor &) = delete;
  sensor &operator=(sensor &)       = delete;

  template <typename t_time>
  sensor(t_logger &p_logger, t_dispatcher &p_dispatcher, t_time p_interval,
         dat::sensor_id p_sensor_id, dat::temperature p_initial,
         dat::temperature p_increment)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_sensor_id(p_sensor_id), m_current(p_initial),
        m_increment(p_increment),
        m_sleeping_loop(
            m_logger,
            [&]()
            {
              {
                std::lock_guard<std::mutex> _lock(m_mutex);
                m_current += m_increment;
              }
              auto _result(m_dispatcher.template publish<evt::new_temperature>(
                  m_sensor_id, m_current));
              if (_result != src::async::result::OK)
              {
                m_logger.err(src::format::fmt(_result));
              }
            },
            p_interval)
  {
  }

  ~sensor() = default;

  void start()
  {
    m_sleeping_loop.start();
  }

  void stop()
  {
    m_sleeping_loop.stop();
  }

  bool operator<(const sensor &p_sensor) const
  {
    return m_sensor_id < p_sensor.m_sensor_id;
  }

  bool operator>(const sensor &p_sensor) const
  {
    return m_sensor_id > p_sensor.m_sensor_id;
  }

  bool operator==(const sensor &p_sensor) const
  {
    return m_sensor_id == p_sensor.m_sensor_id;
  }

  bool operator!=(const sensor &p_sensor) const
  {
    return m_sensor_id != p_sensor.m_sensor_id;
  }

  void reset_temperature(dat::temperature p_temperature)
  {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_current = p_temperature;
  }

  dat::sensor_id get_id() const
  {
    return m_sensor_id;
  }

private:
  t_logger                           &m_logger;
  t_dispatcher                       &m_dispatcher;
  dat::sensor_id                      m_sensor_id;
  dat::temperature                    m_current;
  dat::temperature                    m_increment;
  src::async::sleeping_loop<t_logger> m_sleeping_loop;
  std::mutex                          m_mutex;
};

} // namespace tenacitas::src::async::exp::temperature_sensors_simulator::per

#endif // SENSOR_H

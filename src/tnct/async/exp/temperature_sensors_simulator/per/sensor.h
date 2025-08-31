#ifndef TNCT_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_PER_SENSOR_H
#define TNCT_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_PER_SENSOR_H

#include <tuple>

#include "tnct/async/cpt/has_publish_method.h"
#include "tnct/async/cpt/is_dispatcher.h"
#include "tnct/async/exp/temperature_sensors_simulator/dat/sensor_id.h"
#include "tnct/async/exp/temperature_sensors_simulator/dat/temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tnct/async/result.h"
#include "tnct/async/sleeping_loop.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"

using namespace tnct;

namespace tnct::async::exp::temperature_sensors_simulator::per
{

template <log::cpt::logger t_logger, async::cpt::is_dispatcher t_dispatcher>
requires(async::cpt::is_dispatcher<t_dispatcher, evt::new_temperature>
         && async::cpt::has_publish_method<t_dispatcher, evt::new_temperature>)

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
              if (_result != async::result::OK)
              {
                m_logger.err(format::fmt(_result));
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
  t_logger                      &m_logger;
  t_dispatcher                  &m_dispatcher;
  dat::sensor_id                 m_sensor_id;
  dat::temperature               m_current;
  dat::temperature               m_increment;
  async::sleeping_loop<t_logger> m_sleeping_loop;
  std::mutex                     m_mutex;
};

} // namespace tnct::async::exp::temperature_sensors_simulator::per

#endif // SENSOR_H

#ifndef TNCT_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_EVT_REMOVE_SENSOR_H
#define TNCT_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_EVT_REMOVE_SENSOR_H

#include <iostream>

#include "tnct/async/exp/temperature_sensors_simulator/dat/sensor_id.h"

namespace tnct::async::exp::temperature_sensors_simulator::evt {

struct remove_sensor {
  remove_sensor() = default;
  explicit remove_sensor(dat::sensor_id p_sensor_id) : sensor_id(p_sensor_id) {}

  remove_sensor(const remove_sensor &) = default;
  remove_sensor(remove_sensor &&) = default;
  remove_sensor &operator=(const remove_sensor &) = default;
  remove_sensor &operator=(remove_sensor &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const remove_sensor &p_remove_sensor) {
    p_out << p_remove_sensor.sensor_id;
    return p_out;
  }

  dat::sensor_id sensor_id;
};
} // namespace tnct::async::exp::temperature_sensors_simulator::evt

#endif // remove_sensor_H

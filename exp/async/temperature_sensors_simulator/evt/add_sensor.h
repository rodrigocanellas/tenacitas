#ifndef TENACITAS_EXP_ASYNC_TEMPERATURE_SENSORS_SIMULATOR_EVT_ADD_SENSOR_H
#define TENACITAS_EXP_ASYNC_TEMPERATURE_SENSORS_SIMULATOR_EVT_ADD_SENSOR_H

#include <iostream>

#include "tenacitas/exp/async/temperature_sensors_simulator/dat/sensor_id.h"

namespace tenacitas::exp::async::temperature_sensors_simulator::evt {

struct add_sensor {
  add_sensor() = default;
  explicit add_sensor(dat::sensor_id p_sensor_id) : sensor_id(p_sensor_id) {}

  add_sensor(const add_sensor &) = default;
  add_sensor(add_sensor &&) = default;
  add_sensor &operator=(const add_sensor &) = default;
  add_sensor &operator=(add_sensor &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const add_sensor &p_add_sensor) {
    p_out << p_add_sensor.sensor_id;
    return p_out;
  }

  dat::sensor_id sensor_id;
};

} // namespace tenacitas::exp::async::temperature_sensors_simulator::evt

#endif // ADD_SENSOR_H

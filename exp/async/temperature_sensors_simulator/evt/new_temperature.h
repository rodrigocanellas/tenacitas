#ifndef TENACITAS_EXP_ASYNC_TEMPERATURE_SENSORS_SIMULATOR_EVT_NEW_TEMPERATURE_H
#define TENACITAS_EXP_ASYNC_TEMPERATURE_SENSORS_SIMULATOR_EVT_NEW_TEMPERATURE_H

#include <iostream>

#include "tenacitas/exp/async/temperature_sensors_simulator/dat/sensor_id.h"
#include "tenacitas/exp/async/temperature_sensors_simulator/dat/temperature.h"

namespace tenacitas::exp::async::temperature_sensors_simulator::evt {

struct new_temperature {
  new_temperature() = default;
  explicit new_temperature(dat::sensor_id p_sensor_id,
                           dat::temperature p_temperature)
      : sensor_id(p_sensor_id), temperature(p_temperature) {}

  new_temperature(const new_temperature &) = default;
  new_temperature(new_temperature &&) = default;
  new_temperature &operator=(const new_temperature &) = default;
  new_temperature &operator=(new_temperature &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const new_temperature &p_new_temperature) {
    p_out << '(' << p_new_temperature.sensor_id << ','
          << p_new_temperature.temperature << ')';
    return p_out;
  }

  dat::sensor_id sensor_id;
  dat::temperature temperature;
};

} // namespace tenacitas::exp::async::temperature_sensors_simulator::evt

#endif // NEW_TEMPERATURE_H

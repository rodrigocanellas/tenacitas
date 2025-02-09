#ifndef TEMPERATURE_SENSORS_SIMULATOR_EVT_SET_TEMPERATURE_H
#define TEMPERATURE_SENSORS_SIMULATOR_EVT_SET_TEMPERATURE_H

#include <iostream>

#include "tenacitas.lib/async/exp/temperature_sensors_simulator/typ/sensor_id.h"
#include "tenacitas.lib/async/exp/temperature_sensors_simulator/typ/temperature.h"

namespace temperature_sensors_simulator::evt {

struct set_temperature {
  set_temperature() = default;
  explicit set_temperature(typ::sensor_id p_sensor_id,
                           typ::temperature p_temperature)
      : sensor_id(p_sensor_id), temperature(p_temperature) {}

  set_temperature(const set_temperature &) = default;
  set_temperature(set_temperature &&) = default;
  set_temperature &operator=(const set_temperature &) = default;
  set_temperature &operator=(set_temperature &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const set_temperature &p_set_temperature) {
    p_out << '(' << p_set_temperature.sensor_id << ','
          << p_set_temperature.temperature << ')';
    return p_out;
  }

  typ::sensor_id sensor_id;
  typ::temperature temperature;
};

} // namespace temperature_sensors_simulator::evt

#endif // set_temperature_H

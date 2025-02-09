#ifndef TEMPERATURE_SENSORS_SIMULATOR_EVT_NEW_TEMPERATURE_H
#define TEMPERATURE_SENSORS_SIMULATOR_EVT_NEW_TEMPERATURE_H

#include <iostream>

#include "tnct/async/exp/temperature_sensors_simulator/typ/sensor_id.h"
#include "tnct/async/exp/temperature_sensors_simulator/typ/temperature.h"

namespace tnct::async::exp::temperature_sensors_simulator::evt {

struct new_temperature {
  new_temperature() = default;
  explicit new_temperature(typ::sensor_id p_sensor_id,
                           typ::temperature p_temperature)
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

  typ::sensor_id sensor_id;
  typ::temperature temperature;
};

} // namespace tnct::async::exp::temperature_sensors_simulator::evt

#endif // NEW_TEMPERATURE_H

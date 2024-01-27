#ifndef TEMPERATURE_SENSORS_SIMULATOR__EVT__SET_TEMPERATURE_H
#define TEMPERATURE_SENSORS_SIMULATOR__EVT__SET_TEMPERATURE_H

#include <iostream>

#include <tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/sensor_id.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/dom/dat/temperature.h>

namespace temperature_sensors_simulator::asy {

struct set_temperature {
  set_temperature() = default;
  explicit set_temperature(dom::sensor_id p_sensor_id,
                           dom::temperature p_temperature)
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

  dom::sensor_id sensor_id;
  dom::temperature temperature;
};

} // namespace temperature_sensors_simulator::asy

#endif // set_temperature_H

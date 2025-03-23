/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TEMPERATURE_SENSORS_SIMULATOR_PER_SENSORS_BASE_H
#define TEMPERATURE_SENSORS_SIMULATOR_PER_SENSORS_BASE_H

#include "tnct/async/exp/temperature_sensors_simulator/evt/add_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/set_temperature.h"

namespace tnct::async::exp::temperature_sensors_simulator::per {

struct sensors_base {
  virtual void on_add_sensor(evt::add_sensor &&) = 0;
  virtual void on_remove_sensor(evt::remove_sensor &&) = 0;
  virtual void on_set_temperature(evt::set_temperature &&) = 0;
};

} // namespace tnct::async::exp::temperature_sensors_simulator::per
#endif

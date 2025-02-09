#ifndef TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H
#define TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H

#include "tnct/async/dispatcher.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/add_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/set_temperature.h"
#include "tnct/log/cerr.h"

namespace temperature_sensors_simulator {

using logger = tnct::log::cerr;

using dispatcher =
    tnct::async::dispatcher<logger, evt::add_sensor,
                                      evt::new_temperature, evt::remove_sensor,
                                      evt::set_temperature>;

} // namespace temperature_sensors_simulator

#endif

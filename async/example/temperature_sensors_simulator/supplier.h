#ifndef TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H
#define TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H

#include "tenacitas.lib/async/dispatcher.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/add_sensor.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/new_temperature.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tenacitas.lib/async/example/temperature_sensors_simulator/evt/set_temperature.h"
#include "tenacitas.lib/log/cerr.h"

namespace temperature_sensors_simulator {

using logger = tenacitas::lib::log::cerr;

using dispatcher =
    tenacitas::lib::async::dispatcher<logger, evt::add_sensor,
                                      evt::new_temperature, evt::remove_sensor,
                                      evt::set_temperature>;

} // namespace temperature_sensors_simulator

#endif

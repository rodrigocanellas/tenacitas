#ifndef TENACITAS_SRC_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H
#define TENACITAS_SRC_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H

#include "tenacitas/src/async/exp/temperature_sensors_simulator/evt/add_sensor.h"
#include "tenacitas/src/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tenacitas/src/async/exp/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tenacitas/src/async/exp/temperature_sensors_simulator/evt/set_temperature.h"
#include "tenacitas/src/async/dispatcher.h"
#include "tenacitas/src/log/cerr.h"

namespace tenacitas::src::async::exp::temperature_sensors_simulator {

using logger = tenacitas::src::log::cerr;

using dispatcher =
    tenacitas::src::async::dispatcher<logger, evt::add_sensor,
                                      evt::new_temperature, evt::remove_sensor,
                                      evt::set_temperature>;

} // namespace tenacitas::src::async::exp::temperature_sensors_simulator

#endif

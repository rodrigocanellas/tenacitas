#ifndef TNCT_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H
#define TNCT_ASYNC_EXP_TEMPERATURE_SENSORS_SIMULATOR_SUPPLIER_H

#include "tnct/async/exp/temperature_sensors_simulator/evt/add_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/set_temperature.h"
#include "tnct/async/bus/dispatcher.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"

namespace tnct::async::exp::temperature_sensors_simulator {

using logger = tnct::log::cerr;

using dispatcher =
    tnct::async::bus::dispatcher<logger, evt::add_sensor,
                                      evt::new_temperature, evt::remove_sensor,
                                      evt::set_temperature>;

} // namespace tnct::async::exp::temperature_sensors_simulator

#endif

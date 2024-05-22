/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/async/dispatcher.h>

#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/add_sensor.h>
#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/new_temperature.h>
#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/remove_sensor.h>
#include <tenacitas.lib/async/example/temperature_sensors_simulator/evt/set_temperature.h>

namespace temperature_sensors_simulator::asy {

using dispatcher =
    tenacitas::lib::async::dispatcher<async::add_sensor, async::new_temperature,
                                    async::remove_sensor, async::set_temperature>;

}

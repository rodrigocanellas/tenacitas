/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/asy/dispatcher.h>

#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/add_sensor.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/new_temperature.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/remove_sensor.h>
#include <tenacitas.lib/exp/temperature_sensors_simulator/asy/set_temperature.h>

namespace temperature_sensors_simulator::asy {

using dispatcher =
    tenacitas::lib::asy::dispatcher<asy::add_sensor, asy::new_temperature,
                                    asy::remove_sensor, asy::set_temperature>;

}

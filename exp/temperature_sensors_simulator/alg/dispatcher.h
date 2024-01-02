/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.h>

#include "../evt/add_sensor.h"
#include "../evt/new_temperature.h"
#include "../evt/remove_sensor.h"
#include "../evt/set_temperature.h"

namespace temperature_sensors_simulator::alg {

using dispatcher = tnctl::dispatcher_a<evt::add_sensor, evt::new_temperature,
                                     evt::remove_sensor, evt::set_temperature>;

}

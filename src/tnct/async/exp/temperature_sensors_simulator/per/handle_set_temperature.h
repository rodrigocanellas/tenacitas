/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TEMPERATURE_SENSORS_SIMULATOR_PER_HANDLE_SET_TEMPERATURE_H
#define TEMPERATURE_SENSORS_SIMULATOR_PER_HANDLE_SET_TEMPERATURE_H

#include "tnct/async/exp/temperature_sensors_simulator/evt/set_temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/per/sensors_base.h"
#include "tnct/async/handling_definition.h"
#include "tnct/container/circular_queue.h"
#include "tnct/traits/log/logger.h"

namespace tnct::async::exp::temperature_sensors_simulator::per {

template <traits::log::logger t_logger>
using queue_set_temperature =
    container::circular_queue<t_logger, evt::set_temperature, 10>;

struct handle_set_temperature {
  handle_set_temperature(sensors_base &p_sensors) : m_sensors(p_sensors) {}

  using events_handled = std::tuple<evt::set_temperature>;

  void operator()(evt::set_temperature &&p_evt) {
    m_sensors.on_set_temperature(std::move(p_evt));
  }

private:
  sensors_base &m_sensors;
};

template <traits::log::logger t_logger>
using handling_definition_set_temperature =
    tnct::async::handling_definition<evt::set_temperature,
                                     handle_set_temperature,
                                     queue_set_temperature<t_logger>>;

} // namespace tnct::async::exp::temperature_sensors_simulator::per

#endif

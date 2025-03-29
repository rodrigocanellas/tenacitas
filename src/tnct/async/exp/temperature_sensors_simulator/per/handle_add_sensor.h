/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TEMPERATURE_SENSORS_SIMULATOR_PER_HANDLE_ADD_SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR_PER_HANDLE_ADD_SENSOR_H

#include "tnct/async/exp/temperature_sensors_simulator/evt/add_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/per/sensors_base.h"
#include "tnct/async/handling_definition.h"
#include "tnct/container/circular_queue.h"
#include "tnct/traits/log/logger.h"

namespace tnct::async::exp::temperature_sensors_simulator::per {

template <traits::log::logger t_logger>
using queue_add_sensor =
    container::circular_queue<t_logger, evt::add_sensor, 10>;

struct handle_add_sensor {
  handle_add_sensor(sensors_base &p_sensors) : m_sensors(p_sensors) {}

  using events_handled = std::tuple<evt::add_sensor>;

  void operator()(evt::add_sensor &&p_evt) {
    m_sensors.on_add_sensor(std::move(p_evt));
  }

private:
  sensors_base &m_sensors;
};

template <traits::log::logger t_logger>
using handling_definition_add_sensor =
    async::handling_definition<evt::add_sensor, handle_add_sensor,
                               queue_add_sensor<t_logger>>;

} // namespace tnct::async::exp::temperature_sensors_simulator::per

#endif

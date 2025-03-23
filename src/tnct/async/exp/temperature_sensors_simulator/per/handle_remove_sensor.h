/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TEMPERATURE_SENSORS_SIMULATOR_PER_HANDLE_REMOVE_SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR_PER_HANDLE_REMOVE_SENSOR_H

#include "tnct/async/exp/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/per/sensors_base.h"
#include "tnct/container/circular_queue.h"
#include "tnct/traits/log/logger.h"

namespace tnct::async::exp::temperature_sensors_simulator::per {

template <traits::log::logger t_logger>
using queue_remove_sensor =
    container::circular_queue<t_logger, evt::remove_sensor, 10>;

struct handle_remove_sensor {
  handle_remove_sensor(sensors_base &p_sensors) : m_sensors(p_sensors) {}

  using events_handled = std::tuple<evt::remove_sensor>;

  void operator()(evt::remove_sensor &&p_evt) {
    m_sensors.on_remove_sensor(std::move(p_evt));
  }

private:
  sensors_base &m_sensors;
};

} // namespace tnct::async::exp::temperature_sensors_simulator::per

#endif

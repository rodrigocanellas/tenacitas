/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSORS_H

#include <algorithm>
#include <memory>
#include <set>

#include "tnct/async/exp/temperature_sensors_simulator/evt/add_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/new_temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/remove_sensor.h"
#include "tnct/async/exp/temperature_sensors_simulator/evt/set_temperature.h"
#include "tnct/async/exp/temperature_sensors_simulator/per/sensor.h"
#include "tnct/async/handling_definition.h"
#include "tnct/async/result.h"
#include "tnct/container/circular_queue.h"
#include "tnct/traits/async/add_handling_method.h"
#include "tnct/traits/async/dispatcher.h"
#include "tnct/traits/log/logger.h"

using namespace tnct;
using namespace std::chrono_literals;

namespace tnct::async::exp::temperature_sensors_simulator::per {

template <traits::log::logger t_logger, typename t_dispatcher>
requires(

    traits::async::dispatcher<t_dispatcher, evt::add_sensor, evt::remove_sensor,
                              evt::set_temperature, evt::new_temperature>

    )

    struct sensors {

  using events_handled =
      std::tuple<evt::remove_sensor, evt::add_sensor, evt::set_temperature>;

  sensors(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher) {

    if (define_add_sensor_handling() != async::result::OK) {
      constexpr const char *_msg{"error creating handling for 'add_sensor'"};
      TNCT_LOG_ERR(m_logger, _msg);
      throw std::runtime_error(_msg);
    }

    if (define_remove_sensor_handling() != async::result::OK) {
      constexpr const char *_msg{"error creating handling for 'remove_sensor'"};
      TNCT_LOG_ERR(m_logger, _msg);
      throw std::runtime_error(_msg);
    }

    if (define_set_temperature_handling() != async::result::OK) {
      constexpr const char *_msg{
          "error creating handling for 'set_temperature'"};
      TNCT_LOG_ERR(m_logger, _msg);
      throw std::runtime_error(_msg);
    }
  }

  using sensor_ptr = std::unique_ptr<sensor<t_logger, t_dispatcher>>;

  struct sensor_cmp {
    bool operator()(const sensor_ptr &p_s1, const sensor_ptr &p_s2) const {
      return (*p_s1) < (*p_s2);
    }
  };

  using collection = std::set<sensor_ptr, sensor_cmp>;
  using iterator = collection::iterator;
  using const_iterator = collection::const_iterator;

private:
  void on_add_sensor(evt::add_sensor &&p_evt) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    auto _sensor = std::make_unique<sensor<t_logger, t_dispatcher>>(
        m_logger, m_dispatcher, 500ms, p_evt.sensor_id, typ::temperature{25.5},
        typ::temperature{0.75});
    _sensor->start();
    m_collection.insert(std::move(_sensor));
  }

  void on_remove_sensor(evt::remove_sensor &&p_evt) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    iterator _ite = find(p_evt.sensor_id);
    if (_ite != m_collection.end()) {
      m_collection.erase(_ite);
    }
  }

  void on_set_temperature(evt::set_temperature &&p_evt) {
    iterator _ite = find(p_evt.sensor_id);
    if (_ite != m_collection.end()) {
      (*_ite)->reset_temperature(p_evt.temperature);
    }
  }

  iterator find(typ::sensor_id p_sensor_id) {
    return std::find_if(m_collection.begin(), m_collection.end(),
                        [&](const sensor_ptr &p_sensor) {
                          return p_sensor->get_id() == p_sensor_id;
                        });
  }

  async::result define_add_sensor_handling() {
    using queue = container::circular_queue<t_logger, evt::add_sensor, 10>;

    auto _handler = [this](evt::add_sensor &&p_evt) mutable {
      this->on_add_sensor(std::move(p_evt));
    };

    using handling =
        async::handling_definition<evt::add_sensor, decltype(_handler), queue>;

    static_assert(
        traits::async::has_add_handling_method<t_dispatcher,
                                               tnct::async::result, handling>,
        "Invalid handling definition for 'evt::add_sensor'");

    return m_dispatcher.template add_handling<handling>(
        {"add-sensor", std::move(_handler), queue{m_logger}, 1});
  }

  async::result define_remove_sensor_handling() {
    using queue = container::circular_queue<t_logger, evt::remove_sensor, 10>;

    auto _handler = [this](evt::remove_sensor &&p_evt) {
      this->on_remove_sensor(std::move(p_evt));
    };

    using handling = async::handling_definition<evt::remove_sensor,
                                                decltype(_handler), queue>;

    static_assert(
        traits::async::has_add_handling_method<t_dispatcher, async::result,
                                               handling>);

    return m_dispatcher.template add_handling<handling>(
        {"remove-sensor", std::move(_handler), queue{m_logger}, 1});
  }

  async::result define_set_temperature_handling() {
    using queue = container::circular_queue<t_logger, evt::set_temperature, 10>;

    auto _handler = [this](evt::set_temperature &&p_evt) {
      this->on_set_temperature(std::move(p_evt));
    };

    using handling =
        tnct::async::handling_definition<evt::set_temperature,
                                         decltype(_handler), queue>;

    static_assert(
        traits::async::has_add_handling_method<t_dispatcher,
                                               tnct::async::result, handling>);

    return m_dispatcher.template add_handling<handling>(
        {"set-temperature", std::move(_handler), queue{m_logger}, 1});
  }

private:
  t_logger &m_logger;
  t_dispatcher &m_dispatcher;
  collection m_collection;
  std::mutex m_mutex;
};

} // namespace tnct::async::exp::temperature_sensors_simulator::per

#endif // SENSORS_H

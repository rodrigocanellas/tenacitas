#ifndef TENACITAS_TST_ASYNC_DISPATCHER_UIX_H
#define TENACITAS_TST_ASYNC_DISPATCHER_UIX_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <vector>

#include "cfg.h"
#include "dispatcher.h"
#include "typ.h"
#include <tenacitas/async/alg/dispatcher.h>
#include <tenacitas/log/alg/logger.h>
#include <tenacitas/program/alg/options.h>

using namespace std::chrono_literals;

namespace uix {

bool start(const cfg::options &p_options, bus::dispatcher::ptr p_dispatcher,
           std::function<bool(bus::dispatcher::ptr,
                              const typ::generators_definitions &,
                              const typ::publishings_definitions &)>
               p_bus) {
  try {

    typ::total_list _total_generators{p_options.get_total_generators()};

    typ::amount_list _amounts_events{p_options.get_amounts_events()};

    typ::amount_increment _amount_events_increment{
        p_options.get_amount_events_increment()};

    typ::time_list _intervals{p_options.get_intervals()};

    typ::time_increment _interval_increment{p_options.get_interval_increment()};

    typ::total_list _total_publishings{p_options.get_total_publishings()};

    typ::amount_list _amounts_subscribers{p_options.get_amounts_subscribers()};

    typ::amount_increment _amount_subscribers_increment{
        p_options.get_amount_subscribers_increment()};

    typ::time_list _sleeps{p_options.get_sleeps()};

    typ::time_increment _sleep_increment{p_options.get_sleep_increment()};

    for (typ::total _total_generator : _total_generators) {
      for (typ::amount _amount_events : _amounts_events) {
        for (typ::time _interval : _intervals) {
          typ::generators_definitions _generator{
              _total_generator, _amount_events, _amount_events_increment,
              _interval, _interval_increment};

          for (typ::total _total_publishing : _total_publishings) {
            for (typ::amount _amount_subscribers : _amounts_subscribers) {
              for (typ::time _sleep : _sleeps) {
                typ::publishings_definitions _publishing{
                    _total_publishing, _amount_subscribers,
                    _amount_subscribers_increment, _sleep, _sleep_increment};
                if (!p_bus(p_dispatcher, _generator, _publishing)) {
                  TNCT_LOG_TST("'p_bus' returned 'false'");
                  return false;
                } else {
                  TNCT_LOG_TST("'p_bus' returned 'true'");
                }
              }
            }
          }
        }
      }
    }
  } catch (std::runtime_error &_ex) {
    TNCT_LOG_ERR("ERROR: ", _ex.what());
    return false;
  }
  return true;
}

} // namespace uix

#endif // UIX_H

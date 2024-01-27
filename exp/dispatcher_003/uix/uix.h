#ifndef TNCT_TST_ASYNC_DISPATCHER_UIX_H
#define TNCT_TST_ASYNC_DISPATCHER_UIX_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <functional>

#include <tenacitas.lib/exp/dispatcher_003/asy/asy.h>
#include <tenacitas.lib/exp/dispatcher_003/cfg/cfg.h>
#include <tenacitas.lib/exp/dispatcher_003/dom/dat.h>

using namespace std::chrono_literals;

namespace uix {

bool start(const cfg::options &p_options, asy::dispatcher::ptr p_dispatcher,
           std::function<bool(asy::dispatcher::ptr,
                              const dom::generators_definitions &,
                              const dom::publishings_definitions &)>
               p_bus) {
  try {

    dom::total_list _total_generators{p_options.get_total_generators()};

    dom::amount_list _amounts_events{p_options.get_amounts_events()};

    dom::amount_increment _amount_events_increment{
        p_options.get_amount_events_increment()};

    dom::time_list _intervals{p_options.get_intervals()};

    dom::time_increment _interval_increment{p_options.get_interval_increment()};

    dom::total_list _total_publishings{p_options.get_total_publishings()};

    dom::amount_list _amounts_subscribers{p_options.get_amounts_subscribers()};

    dom::amount_increment _amount_subscribers_increment{
        p_options.get_amount_subscribers_increment()};

    dom::time_list _sleeps{p_options.get_sleeps()};

    dom::time_increment _sleep_increment{p_options.get_sleep_increment()};

    for (dom::total _total_generator : _total_generators) {
      for (dom::amount _amount_events : _amounts_events) {
        for (dom::time _interval : _intervals) {
          dom::generators_definitions _generator{
              _total_generator, _amount_events, _amount_events_increment,
              _interval, _interval_increment};

          for (dom::total _total_publishing : _total_publishings) {
            for (dom::amount _amount_subscribers : _amounts_subscribers) {
              for (dom::time _sleep : _sleeps) {
                dom::publishings_definitions _publishing{
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

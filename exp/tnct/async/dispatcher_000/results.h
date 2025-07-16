/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_EXP_DISPATCHER_RESULTS_H
#define TNCT_ASYNC_EXP_DISPATCHER_RESULTS_H

#include <map>
#include <sstream>
#include <string>
#include <tuple>

#include "tnct/async/dispatcher_000/event_id.h"
#include "tnct/async/dispatcher_000/handler_id.h"
#include "tnct/async/dispatcher_000/handler_type_id.h"
#include "tnct/async/handling_id.h"

namespace tnct::async::exp {

struct results {
  void increment(event_id p_event_id, handling_id p_handling_id,
                 handler_type_id p_handler_type_id, handler_id p_handler_id) {
    const key _key{p_event_id, p_handling_id, p_handler_type_id, p_handler_id};
    map::iterator _ite{m_map.find(_key)};
    if (_ite != m_map.end()) {
      ++_ite->second;
    } else {
      m_map[_key] = 1;
    }
  }

  std::string report() const {
    if (m_map.empty()) {
      return "";
    }
    events_totals _events_totals;

    std::stringstream _stream;

    map::const_iterator _end{m_map.end()};

    map::const_iterator _ite{m_map.begin()};
    while (_ite != _end) {
      event_id _event_prior{get_event_id(_ite)};

      while ((_ite != _end) && (_event_prior == get_event_id(_ite))) {
        handling_id _handling_prior{get_handling_id(_ite)};

        while ((_ite != _end) && (_event_prior == get_event_id(_ite)) &&
               (_handling_prior == get_handling_id(_ite))) {
          handler_type_id _handler_type_prior{get_handler_type_id(_ite)};

          while ((_ite != _end) && (_event_prior == get_event_id(_ite)) &&
                 (_handling_prior == get_handling_id(_ite)) &&
                 (_handler_type_prior == get_handler_type_id(_ite))) {
            increment(_events_totals, _event_prior, _handling_prior,
                      get_handler_type_id(_ite), get_handler_id(_ite),
                      get_value(_ite));
            ++_ite;
          }
        }
      }
    }

    for (const auto &_event_value : _events_totals) {
      _stream << "event " << _event_value.first << " : "
              << _event_value.second.total;
      for (const auto &_handing_value : _event_value.second.handlings) {
        _stream << "\n\t handling " << _handing_value.first << " : "
                << _handing_value.second.total;
        for (const auto &_handler_type_value :
             _handing_value.second.handlers_types) {
          _stream << "\n\t\t handler type " << _handler_type_value.first
                  << " : " << _handler_type_value.second.total;
          for (const auto &_handler : _handler_type_value.second.handlers) {
            _stream << "\n\t\t\t handler " << _handler.first << " : "
                    << _handler.second;
          }
        }
      }
    }

    return _stream.str();
  }

private:
  using key = std::tuple<event_id, handling_id, handler_type_id, handler_id>;
  using value = size_t;
  using map = std::map<key, value>;

  using handlers_total = std::map<handler_id, size_t>;

  struct handlers_type_total {
    size_t total{0};
    handlers_total handlers;
  };
  using handlers_types_total = std::map<handler_type_id, handlers_type_total>;

  struct handling_total {
    size_t total{0};
    handlers_types_total handlers_types;
  };
  using handlings_totals = std::map<handling_id, handling_total>;

  struct event_total {
    size_t total{0};
    handlings_totals handlings;
  };
  using events_totals = std::map<event_id, event_total>;

private:
  void increment(events_totals &p_events_totals, event_id p_event_id,
                 const handling_id &p_handling_id,
                 handler_type_id p_handler_type_id, handler_id p_handler_id,
                 size_t p_value) const {
    events_totals::iterator _event_ite{p_events_totals.find(p_event_id)};
    if (_event_ite == p_events_totals.end()) {
      _event_ite = p_events_totals.insert({p_event_id, event_total{}}).first;
    }
    _event_ite->second.total += p_value;

    handlings_totals::iterator _handling_ite{
        _event_ite->second.handlings.find(p_handling_id)};
    if (_handling_ite == _event_ite->second.handlings.end()) {
      _handling_ite =
          _event_ite->second.handlings.insert({p_handling_id, handling_total{}})
              .first;
    }
    _handling_ite->second.total += p_value;

    handlers_types_total::iterator _handler_type_ite{
        _handling_ite->second.handlers_types.find(p_handler_type_id)};
    if (_handler_type_ite == _handling_ite->second.handlers_types.end()) {
      _handler_type_ite =
          _handling_ite->second.handlers_types
              .insert({p_handler_type_id, handlers_type_total{}})
              .first;
    }
    _handler_type_ite->second.total += p_value;

    handlers_total::iterator _handler_ite{
        _handler_type_ite->second.handlers.find(p_handler_id)};
    if (_handler_ite == _handler_type_ite->second.handlers.end()) {
      _handler_ite = {
          _handler_type_ite->second.handlers.insert({p_handler_id, 0}).first};
    }
    _handler_ite->second += p_value;
  }

  event_id get_event_id(map::const_iterator p_ite) const {
    return std::get<0>(p_ite->first);
  }

  handling_id get_handling_id(map::const_iterator p_ite) const {
    return std::get<1>(p_ite->first);
  }

  handler_type_id get_handler_type_id(map::const_iterator p_ite) const {
    return std::get<2>(p_ite->first);
  }

  handler_id get_handler_id(map::const_iterator p_ite) const {
    return std::get<3>(p_ite->first);
  }

  size_t get_value(map::const_iterator p_ite) const { return p_ite->second; }

  event_id get_event_id(events_totals::const_iterator p_ite) const {
    return p_ite->first;
  }

  const event_total &
  get_event_total(events_totals::const_iterator p_ite) const {
    return p_ite->second;
  }

  size_t get_event_total_value(events_totals::const_iterator p_ite) const {
    return p_ite->second.total;
  }

  const handlings_totals &
  get_handlings_totals(events_totals::const_iterator p_ite) const {
    return p_ite->second.handlings;
  }

  handlings_totals &get_handlings_totals(events_totals::iterator p_ite) const {
    return p_ite->second.handlings;
  }

  handling_id get_handling_id(handlings_totals::const_iterator p_ite) const {
    return p_ite->first;
  }

  const handling_total &
  get_handling_total(handlings_totals::const_iterator p_ite) const {
    return p_ite->second;
  }

  handling_total &get_handling_total(handlings_totals::iterator p_ite) const {
    return p_ite->second;
  }

  size_t
  get_handling_total_value(handlings_totals::const_iterator p_ite) const {
    return p_ite->second.total;
  }

  void increment_handling_total_value(handlings_totals::iterator p_ite) const {
    ++p_ite->second.total;
  }

  const handlers_types_total &
  get_handlers_type_totals(handlings_totals::const_iterator p_ite) const {
    return p_ite->second.handlers_types;
  }

  handlers_types_total &
  get_handlers_type_totals(handlings_totals::iterator p_ite) const {
    return p_ite->second.handlers_types;
  }

  handler_type_id
  get_handler_type_id(handlers_types_total::const_iterator p_ite) const {
    return p_ite->first;
  }

  size_t get_handler_total(handlers_types_total::const_iterator p_ite) const {
    return p_ite->second.total;
  }

  void increment_handler_total(handlers_total::iterator p_ite) const {
    ++p_ite->second;
  }

private:
  map m_map;
};

} // namespace tnct::async::exp

#endif

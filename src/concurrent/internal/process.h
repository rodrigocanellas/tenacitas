#ifndef TENACITAS_CONCURRENT_PROCESS_H
#define TENACITAS_CONCURRENT_PROCESS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <utility>

#include <concurrent/result.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_data, typename t_timeout> struct process {

  status::result operator()(typename traits_t<t_data>::worker p_work,
                            typename traits_t<t_data>::provider p_provide,
                            t_timeout p_timeout, bool &p_stopped) {
    status::result _result = status::ok;
    t_data _data;
    _result = p_provide(_data);

    if (_result != status::ok) {
      if (_result == concurrent::stopped_by_provider) {
        _result = status::ok;
      }
    } else {
      std::future<status::result> _future = std::async(
          std::launch::async,
          [&p_work](t_data &&p_data) { return p_work(std::move(p_data)); },
          std::move(_data));
      std::future_status _future_status = _future.wait_for(p_timeout);
      if (_future_status == std::future_status::timeout) {
        _result = concurrent::stopped_by_timeout;
        p_stopped = true;
      } else {
        if (_future_status == std::future_status::ready) {
          _result = _future.get();
          if (_result != status::ok) {
            p_stopped = true;
          }
        }
      }
    }
    return _result;
  }
};

template <typename t_timeout> struct process<void, t_timeout> {
  status::result operator()(typename traits_t<void>::worker p_work,
                            typename traits_t<void>::provider /*p_provide*/,
                            t_timeout p_timeout, bool &p_stopped) {
    status::result _result = status::ok;
    std::future<status::result> _future =
        std::async(std::launch::async, [&p_work]() { return p_work(); });

    std::future_status _future_status = _future.wait_for(p_timeout);
    if (_future_status == std::future_status::timeout) {
      _result = concurrent::stopped_by_timeout;
      p_stopped = true;
    } else if (_future_status == std::future_status::ready) {
      _result = _future.get();
      if (_result != status::ok) {
        p_stopped = true;
      }
    }

    return _result;
  }
};

} // namespace concurrent
} // namespace tenacitas

#endif // WORK_H

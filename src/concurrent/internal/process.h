#ifndef TENACITAS_CONCURRENT_PROCESS_H
#define TENACITAS_CONCURRENT_PROCESS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <utility>

#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <concurrent/traits.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief process struct executes the core process for a \p loop object which
/// Work function receives data
///
/// \tparam t_data is the type of data that the Work function receives
///
/// \tparam t_timeout is the type of time to control the timeout for the Work
/// function execution
///
template <typename t_data, typename t_timeout, typename t_log> struct process {

  status::result operator()(typename traits_t<t_data>::worker &p_work,
                            typename traits_t<t_data>::provider &p_provide,
                            t_timeout p_timeout) {
    status::result _result = status::ok;
    std::pair<status::result, t_data> _provided = p_provide();

    if (_provided.first != status::ok) {
      _result = _provided.first;
    } else {
      concurrent_log_debug(t_log,
                           "_provided.second = ",
                           _provided.second,
                           " ",
                           &(_provided.second));

      auto _f = [&p_work](t_data&& p_data) {
        concurrent_log_debug(t_log, "p_data = ", p_data, " ", &p_data);
        return p_work(std::move(p_data));
      };

      std::future<status::result> _future =
        std::async(std::launch::async, _f, std::move(_provided.second));

      std::future_status _future_status = _future.wait_for(p_timeout);
      if (_future_status == std::future_status::timeout) {
        _result = concurrent::stopped_by_timeout;
      } else {
        if (_future_status == std::future_status::ready) {
          _result = _future.get();
        }
      }
    }

    return _result;
  }
};

/// \brief process struct executes the core process for a \p loop object which
/// Work function does not receive data
///
/// \tparam t_timeout is the type of time to control the timeout for the Work
/// function execution
///
template <typename t_timeout, typename t_log>
struct process<void, t_timeout, t_log> {
  status::result operator()(typename traits_t<void>::worker &p_work,
                            typename traits_t<void>::provider /*p_provide*/,
                            t_timeout p_timeout) {
    status::result _result = status::ok;
    std::future<status::result> _future =
        std::async(std::launch::async, [&p_work]() { return p_work(); });

    std::future_status _future_status = _future.wait_for(p_timeout);
    if (_future_status == std::future_status::timeout) {
      _result = concurrent::stopped_by_timeout;
    } else if (_future_status == std::future_status::ready) {
      _result = _future.get();
    }

    return _result;
  }
};

} // namespace concurrent
} // namespace tenacitas

#endif // WORK_H

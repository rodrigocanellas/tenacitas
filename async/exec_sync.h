/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXEC_ASYNC_H
#define TENACITAS_LIB_ASYNC_EXEC_ASYNC_H

#include <condition_variable>
#include <functional>
#include <optional>

#include "tenacitas.lib/traits/chrono_convertible.h"

namespace tenacitas::lib::async  {

/// \brief Executes a function synchronoulsy with timeout control
/// The function may or may not return, and may or may not receive parameters
///
/// \tparam t_function the function to be executed. It may not return, as
///
/// <tt>void t_function(std::function<bool()> p_timeout, t_params...
/// p_params)</tt> or it may return, as in
///
/// <tt>some-ret t_function(std::function<bool()> p_timeout, t_params...
/// p_params)</tt>
///
/// where <tt>p_timeout</tt> informs that the function has exceeded the maximum
/// time of execution, and can stop executing
///
/// So, \p t_function should eventually call for <tt>p_timeout()</tt> to decide
/// if it must continue to exec_sync
///
/// \tparam t_params possible parameters of t_function
///
/// \param p_max_time is the maximum amount of time that \p p_function should
/// take to exec_sync
///
/// \param p_function is a function with one of the signatures described above
///
/// \param p_params are the possible arguments for \p p_function
///
/// \attention Please, take a look at
/// <tt>tenacitas.lib.async/exp/executer_000/main.cpp</tt> for examples
template <typename t_function, typename... t_params>
inline std::conditional_t<
    // if 't_function' return type is 'void'
    std::is_void_v<
        std::invoke_result_t<t_function, std::function<bool()>, t_params...>>,
    // the 'execute' wrapper will return 'bool', which will be 'true' if the
    // 'p_function' executes in less 'p_max_time', or 'false' otherwise
    bool,
    // else it will result a 'std::optional' with the return type of
    // 't_function', which will contain a value of that type, if the
    // 'p_function' executes in less 'p_max_time', or empty otherwise
    std::optional<
        std::invoke_result_t<t_function, std::function<bool()>, t_params...>>>
exec_sync(traits::convertible_to_nano auto p_max_time, t_function &p_function,
          t_params &&...p_params) {
  std::mutex _mutex;
  std::condition_variable _cond;
  bool _timeout{false};

  auto _is_timeout = [&_timeout]() { return _timeout; };

  using t_ret = typename std::invoke_result_t<t_function, std::function<bool()>,
                                              t_params...>;

  auto _ns{std::chrono::duration_cast<std::chrono::nanoseconds>(p_max_time)};

  if constexpr (std::is_void_v<t_ret>) {
    std::thread _th([&]() -> void {
      p_function(_is_timeout, std::forward<t_params>(p_params)...);
      _cond.notify_one();
    });

    std::unique_lock<std::mutex> _lock{_mutex};
    if (_cond.wait_for(_lock, _ns) != std::cv_status::timeout) {
      //      _th.join();
      _th.detach();
      return true;
    }
    _timeout = true;
    _th.join();
    return false;
  } else {
    t_ret _ret;

    std::thread _th([&]() -> void {
      _ret = p_function(_is_timeout, std::forward<t_params>(p_params)...);
      _cond.notify_one();
    });

    std::unique_lock<std::mutex> _lock{_mutex};
    if (_cond.wait_for(_lock, _ns) != std::cv_status::timeout) {
      //      _th.join();
      _th.detach();
      return {std::move(_ret)};
    }
    _timeout = true;
    _th.join();
    std::optional<t_ret> _aux{};
    return _aux;
  }
};

} // namespace tenacitas::lib::async

#endif

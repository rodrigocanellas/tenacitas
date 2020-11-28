#ifndef TENACITAS_CONCURRENT_WORK_WRAPPER_H
#define TENACITAS_CONCURRENT_WORK_WRAPPER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

/// \brief Wrapper for the worker function, when it returns something, and
/// receives many parameters
///
/// \tparam t_result is the type returned by the worker function
///
/// \tparam t_params are the parameters that the worker function receives
template <typename t_result, typename... t_params> struct work_wrapper_t {

  typedef std::function<t_result(t_params...)> worker;

  inline constexpr work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline constexpr void operator()() {
    m_result = std::apply(m_worker, std::move(m_params));
  }

  inline constexpr void set_params(t_params... p_params) {
    m_params = std::make_tuple(p_params...);
  }

  inline constexpr worker get_worker() const { return m_worker; }

  inline constexpr operator bool() const noexcept {
    return (m_worker ? true : false);
  }

  inline constexpr std::optional<t_result> get_result_ok() { return m_result; }

  inline constexpr std::optional<t_result> get_result_not_ok() { return {}; }

private:
  std::optional<t_result> m_result;

  std::tuple<t_params...> m_params;

  worker m_worker;
};

/// \brief Wrapper for the worker function, when it does not return, and
/// receives many parameters
///
/// \tparam t_result is the type returned by the worker function
template <typename... t_params> struct work_wrapper_t<void, t_params...> {

  typedef std::function<void(t_params...)> worker;

  inline constexpr work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline constexpr void operator()() {
    std::apply(m_worker, std::move(m_params));
  }

  inline constexpr void set_params(t_params... p_params) {
    m_params = std::make_tuple(p_params...);
  }

  inline constexpr worker get_worker() const { return m_worker; }

  inline constexpr operator bool() const noexcept {
    return (m_worker ? true : false);
  }

  inline constexpr void get_result_ok() {}

  inline constexpr void get_result_not_ok() {}

private:
  std::tuple<t_params...> m_params;

  worker m_worker;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_WORK_WRAPPER_H

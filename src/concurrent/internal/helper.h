#ifndef TENACITAS_CONCURRENT_HELPER_H
#define TENACITAS_CONCURRENT_HELPER_H

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

template <typename t_result, typename... t_params> struct work_wrapper_t {

  typedef std::function<t_result(t_params...)> worker;

  inline work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline void operator()() {
    m_result = std::apply(m_worker, std::move(m_params));
  }

  inline void set_params(t_params... p_params) {
    m_params = std::make_tuple(p_params...);
  }

  inline std::optional<t_result> get_result_ok() { return m_result; }

  inline std::optional<t_result> get_result_not_ok() { return {}; }

  std::optional<t_result> m_result;

  std::tuple<t_params...> m_params;

  worker m_worker;
};

template <typename... t_params> struct work_wrapper_t<void, t_params...> {

  typedef std::function<void(t_params...)> worker;

  inline work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline void operator()() { std::apply(m_worker, std::move(m_params)); }

  inline void set_params(t_params... p_params) {
    m_params = std::make_tuple(p_params...);
  }

  inline void get_result_ok() {}

  inline void get_result_not_ok() {}

  std::tuple<t_params...> m_params;

  worker m_worker;
};

template <typename t_result, typename t_param>
struct work_wrapper_t<t_result, t_param> {

  typedef std::function<t_result(t_param)> worker;

  inline work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline void operator()() { m_result = m_worker(std::move(m_param)); }

  inline void set_params(t_param p_param) { m_param = std::move(p_param); }

  inline std::optional<t_result> get_result_ok() { return m_result; }

  inline std::optional<t_result> get_result_not_ok() { return {}; }

  std::optional<t_result> m_result;

  t_param m_param;

  worker m_worker;
};

template <typename t_param> struct work_wrapper_t<void, t_param> {

  typedef std::function<void(t_param)> worker;

  inline work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline void operator()() { m_worker(std::move(m_param)); }

  inline void set_params(t_param p_param) { m_param = std::move(p_param); }

  inline void get_result_ok() {}

  inline void get_result_not_ok() {}

  t_param m_param;

  worker m_worker;
};

template <typename t_result> struct work_wrapper_t<t_result> {

  typedef std::function<t_result()> worker;

  inline work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline void operator()() { m_result = m_worker(); }

  inline void set_params() {}

  inline std::optional<t_result> get_result_ok() { return m_result; }

  inline std::optional<t_result> get_result_not_ok() { return {}; }

  std::optional<t_result> m_result;

  worker m_worker;
};

template <> struct work_wrapper_t<void> {

  typedef std::function<void()> worker;

  inline work_wrapper_t(worker p_worker) : m_worker(p_worker) {}

  inline void operator()() { m_worker(); }

  inline void set_params() {}

  inline void get_result_ok() {}

  inline void get_result_not_ok() {}

  worker m_worker;
};

} // namespace concurrent
} // namespace tenacitas

#endif // HELPER_H

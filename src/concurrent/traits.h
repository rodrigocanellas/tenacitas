#ifndef TENACITAS_CONCURRENT_TRAITS_H
#define TENACITAS_CONCURRENT_TRAITS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <future>
#include <memory>
#include <optional>
#include <utility>

#include <concurrent/result.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace concurrent {

template <typename t_result, typename... t_params> struct traits_t;

template <typename t_result, typename t_param>
struct traits_t<t_result, t_param>;

template <typename t_param> struct traits_t<void, t_param>;

template <typename t_result> struct traits_t<t_result, void>;

template <typename... t_params> struct traits_t<void, t_params...>;

template <> struct traits_t<void, void>;

/// \brief traits_t defines types for the rest of the \p concurrent library
///
/// The types are a \p worker function, a \p provider function, and a \p breaker
/// function
///
/// \tparam t_result is the return type of the \p worker function
///
/// \tparam t_params... are the types of the parameters the \p worker function
/// receives
///
template <typename t_result, typename... t_params> struct traits_t {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \return a optional return, with the value calculated by the \p worker
  /// function
  typedef std::function<std::optional<t_result>(t_params...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief function that will be called to stop the loop, if it returns \p
  /// true
  typedef std::function<bool()> breaker;

  /// \brief function that will be called when a timeout of the worker function
  /// occurrs
  typedef std::function<void(std::thread::id)> timeout_callback;
};

/// \brief traits_t defines types for the rest of the \p concurrent library
///
/// The types are a \p worker function, a \p provider function, and a \p breaker
/// function
///
/// This specialization defines \p worker function that takes one parameter
///
/// \tparam t_result is the return type of the \p worker function
///
/// \tparam t_param is the type of the parameter the \p worker function
/// receives
///
template <typename t_result, typename t_param>
struct traits_t<t_result, t_param> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \return a optional return, with the value calculated by the \p worker
  /// function
  typedef std::function<std::optional<t_result>(t_param)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional object needed by the \p worker
  typedef std::function<std::optional<t_param>()> provider;

  /// \brief \p function that will be called to stop the loop, if it returns \p
  /// true
  typedef std::function<bool()> breaker;

  /// \brief function that will be called when a timeout of the worker function
  /// occurrs
  typedef std::function<void(std::thread::id)> timeout_callback;
};

/// \brief traits_t defines types for the rest of the \p concurrent library
///
/// The types are a \p worker function, a \p provider function, and a \p breaker
/// function
///
/// This specialization defines \p worker function that does not return and take
/// one parameter
///
/// \tparam t_param is the type of the parameter the \p worker function
/// receives
///
template <typename t_param> struct traits_t<void, t_param> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  typedef std::function<void(t_param)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional object needed by the \p worker
  typedef std::function<std::optional<t_param>()> provider;

  /// \brief \p function that will be called to stop the loop, if it returns \p
  /// true
  typedef std::function<bool()> breaker;

  /// \brief function that will be called when a timeout of the worker function
  /// occurrs
  typedef std::function<void(std::thread::id)> timeout_callback;
};

/// \brief traits_t defines types for the rest of the \p concurrent library
///
/// The types are a \p worker function, a \p provider function, and a \p breaker
/// function
///
/// This specialization defines \p worker function that takes no one parameters
///
/// \tparam t_result is the return type of the \p worker function
///
template <typename t_result> struct traits_t<t_result, void> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \return a optional return, with the value calculated by the \p worker
  /// function
  typedef std::function<std::optional<t_result>()> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// As the \p worker does not need parameter, this provider is not called
  typedef typename std::function<void(void)> provider;

  /// \brief \p function that will be called to stop the loop, if it returns \p
  /// true
  typedef std::function<bool()> breaker;

  /// \brief function that will be called when a timeout of the worker function
  /// occurrs
  typedef std::function<void(std::thread::id)> timeout_callback;
};

/// \brief traits_t defines types for the rest of the \p concurrent library
///
/// The types are a \p worker function, a \p provider function, and a \p breaker
/// function
///
/// This specialization defines \p worker function that does not return
///
/// \tparam t_params... are the types of the parameters the \p worker function
/// receives
///
template <typename... t_params> struct traits_t<void, t_params...> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void(t_params...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// \return \p an optional tuple of objects needed by the \p worker
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief \p function that will be called to stop the loop, if it returns \p
  /// true
  typedef std::function<bool()> breaker;

  /// \brief function that will be called when a timeout of the worker function
  /// occurrs
  typedef std::function<void(std::thread::id)> timeout_callback;
};

/// \brief traits_t defines types for the rest of the \p concurrent library
///
/// The types are a \p worker function, a \p provider function, and a \p breaker
/// function
///
/// This specialization defines \p worker function that does not return, and
/// takes no parameters
///
template <> struct traits_t<void, void> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  typedef std::function<void()> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function
  ///
  /// As the \p worker does not need parameter, this provider is not called
  typedef typename std::function<void(void)> provider;

  /// \brief \p function that will be called to stop the loop, if it returns \p
  /// true
  typedef std::function<bool()> breaker;

  /// \brief function that will be called when a timeout of the worker function
  /// occurrs
  typedef std::function<void(std::thread::id)> timeout_callback;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TRAITS_H

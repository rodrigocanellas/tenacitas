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

/// \brief traits_t defines types when dealing with loop with data
///
/// \param t_data is the type of the data to be handled; it must be:
///    - default constructible
///    - move constructible
///
template <typename t_result, typename... t_params> struct traits_t {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return \p true if the execution was ok;
  ///         \p false if the \p worker should not be called again
  typedef std::function<std::optional<t_result>(t_params &&...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return \p {true, a valid data} if data was provided
  ///         \p {false, an default data} if no data was provided
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief breaker is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return \p true, if the execution should stop
  ///         \p false, if the execution shoud continue
  typedef std::function<bool()> breaker;
};

/// \brief traits_t defines types when dealing with loop with data
///
/// \param t_data is the type of the data to be handled; it must be:
///    - default constructible
///    - move constructible
///
template <typename t_result, typename t_param>
struct traits_t<t_result, t_param> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return \p true if the execution was ok;
  ///         \p false if the \p worker should not be called again
  typedef std::function<std::optional<t_result>(t_param &&)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return \p {true, a valid data} if data was provided
  ///         \p {false, an default data} if no data was provided
  typedef std::function<std::optional<t_param>()> provider;

  /// \brief breaker is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return \p true, if the execution should stop
  ///         \p false, if the execution shoud continue
  typedef std::function<bool()> breaker;
};

/// \brief traits_t<void> is a specialization that defines types when dealing
/// with a work function that does not receive any data
///
template <typename t_result> struct traits_t<t_result, void> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \return \p true if the execution was ok;
  ///         \p false if the \p worker should not be called again
  typedef std::function<std::optional<t_result>()> worker;

  /// \brief in this specialization there is no provider, as \p worker
  /// requires no data
  typedef typename std::function<void(void)> provider;

  /// \return \p true, if the execution should stop
  ///         \p false, if the execution shoud continue
  typedef std::function<bool()> breaker;
};

template <typename... t_params> struct traits_t<void, t_params...> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return \p true if the execution was ok;
  ///         \p false if the \p worker should not be called again
  typedef std::function<void(t_params &&...)> worker;

  /// \brief provider is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return \p {true, a valid data} if data was provided
  ///         \p {false, an default data} if no data was provided
  typedef std::function<std::optional<std::tuple<t_params...>>()> provider;

  /// \brief breaker is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return \p true, if the execution should stop
  ///         \p false, if the execution shoud continue
  typedef std::function<bool()> breaker;
};

template <> struct traits_t<void, void> {

  /// \brief worker is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \return \p true if the execution was ok;
  ///         \p false if the \p worker should not be called again
  typedef std::function<void()> worker;

  /// \brief in this specialization there is no provider, as \p worker
  /// requires no data
  typedef typename std::function<bool(void)> provider;

  /// \return \p true, if the execution should stop
  ///         \p false, if the execution shoud continue
  typedef std::function<bool()> breaker;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TRAITS_H

#ifndef TENACITAS_CONCURRENT_TRAITS_H
#define TENACITAS_CONCURRENT_TRAITS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <future>
#include <memory>
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
template <typename t_data> struct traits_t {

  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return \p {status::result::ok if the execution was ok, or any other value
  /// otherwise
  typedef std::function<status::result(t_data)> worker;

  /// \brief provide_t is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return:
  /// \p status::ok, the parameter \p t_data has a meaningful data;
  ///
  /// \p status::stopped_by_provider, then there is no more data
  /// to provide, and the parameter \p t_data has a default \p t_data value;
  ///
  /// anything else, it means there was an error in the provider
  /// function, and the parameter \p t_data has a default \p t_data value;
  typedef std::function<status::result(t_data &)> provider;

  /// \brief break_t is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return
  /// \p status::ok if the loop where this function is being called should
  /// continue
  ///
  /// \p status::stopped_by_breaker if the loop should stop
  ///
  /// anything else, it means there was an error in the breaker function
  typedef std::function<status::result()> breaker;
};

/// \brief loop_traits is a specialization that defines types when dealing
/// with a work function that does not receive any data
template <> struct traits_t<void> {

  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \return \p status::ok if the execution was ok, or any other value
  /// otherwise
  typedef std::function<status::result()> worker;

  /// \brief in this specialization there is no provider, as \p worker
  /// requires no data
  typedef typename std::function<status::result(void)> provider;

  /// \brief break_t is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return
  /// \p status::ok if the loop where this function is being called should
  /// continue
  ///
  /// \p status::stopped_by_breaker if the loop should stop
  ///
  /// anything else, it means there was an error in the breaker function
  typedef std::function<status::result()> breaker;
};

} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_CONCURRENT_TRAITS_H

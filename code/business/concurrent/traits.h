#ifndef TENACITAS_BUSINESS_CONCURRENT_TRAITS_H
#define TENACITAS_BUSINESS_CONCURRENT_TRAITS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <utility>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace business {
/// \brief namespace of the class group
namespace concurrent {

enum class result : uint8_t
{
  stop = 0,
  dont_stop = 1
};

///
/// \brief loop_traits defines types when dealing with loop with data
///
/// \param t_data is the type of the data to be handled; it must be:
///    - default constructible
///    - move constructible
///
template<typename t_data>
struct traits_t
{
  ///
  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \param t_data is an instance of the data to be handled
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or result::dont_stop if it should continue
  typedef std::function<result(t_data&&)> worker;

  ///
  /// \brief provide_t is the type of function that provides data to the work
  /// function during the loop execution
  ///
  /// \return a pair, where if \p first is \p true, the \p second has a
  /// meaningful data; if \p first is \p false, then \p second has a default
  /// value of \p t_data
  ///
  typedef std::function<std::pair<bool, t_data>()> provider;

  ///
  /// \brief break_t is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or result::dont_stop if it should continue
  typedef std::function<result()> breaker;
};

///
/// \brief loop_traits is a specialization that defines types when dealing
/// with loop with no data
///
template<>
struct traits_t<void>
{
  ///
  /// \brief work_t is the type of work function, i.e., the function that will
  /// be called in a loop in order to execute some work
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or result::dont_stop if it should continue
  typedef std::function<result()> worker;

  ///
  /// \brief provide_t in the case of a \p void data has no effect
  ///
  typedef std::function<void()> provider;

  ///
  /// \brief break_t is the type of function that indicates if the loop should
  /// stop
  ///
  /// \return result::stop if the loop where this function is being
  /// called should stop, or result::dont_stop
  typedef std::function<result()> breaker;
};

} // namespace business
} // namespace concurrent
} // namespace tenacitas

#endif // TENACITAS_BUSINESS_CONCURRENT_TRAITS_H

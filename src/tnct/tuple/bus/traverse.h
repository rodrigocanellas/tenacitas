/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_TRAVERSE_H
#define TNCT_TUPLE_TRAVERSE_H

#include <cstddef>
#include <tuple>
#include <type_traits>

#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/cpt/visit.h"

namespace tnct::tuple::bus
{

/// \brief Traverses the values of a non const tuple while the visitor function
/// returns true
/// \note a little help from ChatGPT was used here
template <tuple::cpt::is_tuple t_tuple, typename t_func, size_t t_idx = 0>
requires(tuple::cpt::visit_value_in_constant_tuple<t_func, t_tuple, t_idx>)

constexpr void traverse(const std::remove_cvref_t<t_tuple> &p_tuple,
                        t_func                              p_function)
{

  if constexpr (t_idx >= std::tuple_size_v<t_tuple>)
  {
    return;
  }

  if constexpr (requires {
                  p_function.template operator()<t_tuple, t_idx>(
                      std::declval<const t_tuple &>());
                })
  {
    if (!p_function.template operator()<t_tuple, t_idx>(p_tuple))
      return;
  }

  if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>)
  {
    traverse<t_tuple, t_func, t_idx + 1>(p_tuple, p_function);
  }
}

/// \brief Traverses the values of a const tuple while the visitor function
/// returns true
/// \note a little help from ChatGPT was used here
template <tuple::cpt::is_tuple t_tuple, typename t_func, size_t t_idx = 0>
requires(tuple::cpt::visit_value_in_mutable_tuple<t_func, t_tuple, t_idx>)

constexpr void traverse(std::remove_cvref_t<t_tuple> &p_tuple,
                        t_func                        p_function)
{

  if constexpr (t_idx >= std::tuple_size_v<t_tuple>)
  {
    return;
  }

  if constexpr (requires {
                  p_function.template operator()<t_tuple, t_idx>(
                      std::declval<t_tuple &>());
                })
  {
    if (!p_function.template operator()<t_tuple, t_idx>(p_tuple))
      return;
  }

  if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>)
  {
    traverse<t_tuple, t_func, t_idx + 1>(p_tuple, p_function);
  }
}

/// \brief Traverses the types of a tuple while the visitor function returns
/// true
/// \note a little help from ChatGPT was used here
template <tuple::cpt::is_tuple t_tuple, typename t_func, std::size_t t_idx = 0>
requires(tuple::cpt::visit_type_in_tuple<t_func, t_tuple, t_idx>)

constexpr void traverse(t_func p_function)
{

  if constexpr (t_idx >= std::tuple_size_v<t_tuple>)
  {
    return;
  }

  if constexpr (requires { p_function.template operator()<t_tuple, t_idx>(); })
  {
    if (!p_function.template operator()<t_tuple, t_idx>())
      return;
  }

  if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>)
  {
    return traverse<t_tuple, t_func, t_idx + 1>(p_function);
  }
}

} // namespace tnct::tuple::bus

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_GET_INDEX_H
#define TNCT_TUPLE_GET_INDEX_H

#include <optional>
#include <tuple>

#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/cpt/visit.h"

namespace tnct::tuple
{

template <tuple::cpt::is_tuple t_tuple, typename t_type, std::size_t t_idx = 0>
constexpr std::optional<std::size_t> get_type_index()
{
  if (std::tuple_size_v<t_tuple> == 0)
  {
    return std::nullopt;
  }
  else if constexpr (t_idx >= std::tuple_size_v<t_tuple>)
  {
    return std::nullopt;
  }
  else if constexpr (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>,
                                    t_type>)
  {
    return t_idx;
  }
  else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>)
  {
    return get_type_index<t_tuple, t_type, t_idx + 1>();
  }
  return std::nullopt;
}

/// \tparam t_func must return /p true if there is a match
template <tuple::cpt::is_tuple t_tuple, typename t_func, std::size_t t_idx = 0>
requires(tuple::cpt::visit_type_in_tuple<t_func, t_tuple, t_idx>)
constexpr std::optional<std::size_t> get_type_index(t_func p_func)
{
  if (std::tuple_size_v<t_tuple> == 0)
  {
    return std::nullopt;
  }
  else if constexpr (t_idx >= std::tuple_size_v<t_tuple>)
  {
    return std::nullopt;
  }
  else if constexpr (p_func.template operator()<t_tuple, t_idx>())
  {
    return t_idx;
  }
  else if constexpr ((t_idx + 1) < std::tuple_size_v<t_tuple>)
  {
    return get_type_index<t_tuple, t_func, t_idx + 1>(p_func);
  }
  return std::nullopt;
}

// template <like t_tuple, typename t_type, std::size_t t_idx = 0>
// constexpr std::optional<std::size_t> find() {
//   if constexpr (std::tuple_size_v<t_tuple> == 0) {
//     return std::nullopt;
//   } else {
//     std::optional<std::size_t> _maybe_idx;
//     auto _visitor =
//         [&]<tuple::like t_tuple_aux, std::size_t t_idx_aux>() -> bool {
//       using element_type = std::tuple_element_t<t_idx_aux, t_tuple_aux>;
//       if constexpr (std::is_same_v<t_type, element_type>) {
//         _maybe_idx = t_idx_aux;
//         return false;
//       }
//       return true;
//     };

//     tuple::traverse<t_tuple>(_visitor);

//     return _maybe_idx;
//   }
// }

} // namespace tnct::tuple

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_INTERNAL_BUS_CREAT_MATRIX_FOR_MULTIPLY_H
#define TNCT_CONTAINER_INTERNAL_BUS_CREAT_MATRIX_FOR_MULTIPLY_H

#include <concepts>
#include <optional>

#include "tnct/container/dat/matrix.h"

namespace tnct::container::internal::bus
{

template <std::unsigned_integral t_index, typename t_data>
requires(std::copyable<t_data>
         && (std::integral<t_data> || std::floating_point<t_data>))

std::optional<container::dat::matrix<t_index, t_data>>
create_matrix_for_multiply(
    const container::dat::matrix<t_index, t_data> &p_matrix_a,
    const container::dat::matrix<t_index, t_data> &p_matrix_b)
{
  auto _matrix_c{container::dat::matrix<t_index, t_data>::create(
      p_matrix_a.get_num_rows(), p_matrix_b.get_num_cols(), t_data{0})};
  if (!_matrix_c)
  {
    return std::nullopt;
  }
  return _matrix_c;
}

} // namespace tnct::container::internal::bus

#endif

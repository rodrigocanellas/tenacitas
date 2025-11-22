/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_INTERNAL_EVT_MULTIPLY_MATRIX_CELL_H
#define TNCT_CONTAINER_INTERNAL_EVT_MULTIPLY_MATRIX_CELL_H

#include <functional>
#include <iostream>

#include "tnct/container/dat/matrix.h"

namespace tnct::container::internal::evt
{

template <std::unsigned_integral t_index, typename t_data>
requires(std::copyable<t_data>
         && (std::integral<t_data> || std::floating_point<t_data>))
struct multiply_matrix_cell
{
  using index         = t_index;
  using data          = t_data;
  using matrix        = dat::matrix<index, data>;
  using const_opt_ref = std::reference_wrapper<const matrix>;
  using opt_ref       = std::reference_wrapper<matrix>;

  multiply_matrix_cell(const const_opt_ref &p_matrix_a,
                       const const_opt_ref &p_matrix_b, opt_ref &p_matrix_c,
                       index p_row_start, index p_row_end)
      : m_matrix_a(p_matrix_a), m_matrix_b(p_matrix_b), m_matrix_c(p_matrix_c),
        m_row_begin(p_row_start), m_row_end(p_row_end)
  {
  }

  multiply_matrix_cell()                                        = default;
  multiply_matrix_cell(const multiply_matrix_cell &)            = default;
  multiply_matrix_cell(multiply_matrix_cell &&)                 = default;
  multiply_matrix_cell &operator=(const multiply_matrix_cell &) = default;
  multiply_matrix_cell &operator=(multiply_matrix_cell &&)      = default;
  ~multiply_matrix_cell()                                       = default;

  friend std::ostream &
  operator<<(std::ostream               &p_out,
             const multiply_matrix_cell &p_multiply_matrix_cel)
  {
    p_out << "multiply_matrix_row for (" << p_multiply_matrix_cel.m_row_begin
          << ',' << p_multiply_matrix_cel.m_row_end << ')';
    return p_out;
  }

  const_opt_ref m_matrix_a;
  const_opt_ref m_matrix_b;
  opt_ref       m_matrix_c;
  index         m_row_begin{0};
  index         m_row_end{0};
};

} // namespace tnct::container::internal::evt
#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_MATRIX_H
#define TNCT_CONTAINER_CPT_MATRIX_H

#include <concepts>
#include <optional>
#include <utility>

#include "tnct/ostream/cpt/has_output_operator.h"

namespace tnct::container::cpt
{

template <typename t, typename t_index, typename t_data>
concept matrix =
    std::integral<t_index> &&

    std::copyable<t_data> &&

    ostream::cpt::has_output_operator<t_data> &&

    ostream::cpt::has_output_operator<t> &&

    std::copyable<t> &&

    std::same_as<typename t::data, t_data> &&

    std::same_as<typename t::index, t_index> &&

    requires(t p_t, t_index p_num_rows, t_index p_num_cols, t_data p_initial) {
      {
        t::create(p_num_rows, p_num_cols, p_initial)
      } -> std::same_as<std::optional<t>>;
    } &&

    requires(t p_t, t_index p_row, t_index p_col) {
      {
        p_t(p_row, p_col)
      } -> std::same_as<t_data &>;

      {
        std::as_const(p_t)(p_row, p_col)
      } -> std::same_as<const t_data &>;

      {
        p_t.get_cell(p_row, p_col)
      } -> std::same_as<std::optional<std::reference_wrapper<const t_data>>>;
    } &&

    requires(t p_t, t_index p_row, t_index p_col, const t_data &p_data) {
      {
        p_t.set_cell(p_row, p_col, p_data)
      } -> std::same_as<bool>;
    } &&

    requires(t p_t, t_index p_row, t_index p_col, t_data &&p_data) {
      {
        p_t.set_cell(p_row, p_col, std::move(p_data))
      } -> std::same_as<bool>;
    } &&

    requires(t p_t) {
      {
        p_t.get_num_rows()
      } -> std::same_as<t_index>;

      {
        p_t.get_num_cols()
      } -> std::same_as<t_index>;

      {
        p_t.reset()
      } -> std::same_as<void>;
    };

} // namespace tnct::container::cpt

#endif

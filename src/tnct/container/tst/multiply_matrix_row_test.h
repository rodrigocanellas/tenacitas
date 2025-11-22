/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_MULTIPLY_MATRIX_ROW_TEST_H
#define TNCT_CONTAINER_TST_MULTIPLY_MATRIX_ROW_TEST_H

#include <optional>
#include <string>

#include "tnct/async/cpt/is_event.h"
#include "tnct/container/dat/matrix.h"
#include "tnct/container/internal/evt/multiply_matrix_cell.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

namespace tnct::container::tst
{

struct multiply_matrix_row_000
{

  static std::string desc()
  {
    return "";
  }

  bool operator()(const program::bus::options &)
  {
    using log    = log::cerr;
    using index  = std::uint16_t;
    using data   = std::uint32_t;
    using matrix = container::dat::matrix<index, data>;
    using multiply_matrix_row =
        container::internal::evt::multiply_matrix_cell<index, data>;

    log _log;

    auto        _a{matrix::create(4, 2, 0)};
    auto        _b{matrix::create(2, 8, 0)};
    auto        _c{matrix::create(4, 8, 0)};
    const index _row_start{0};
    const index _row_end{1};

    std::reference_wrapper<matrix> _ref_a{*_a};
    std::reference_wrapper<matrix> _ref_b{*_b};
    std::reference_wrapper<matrix> _ref_c{*_c};

    multiply_matrix_row _multiply_matrix_row{_ref_a, _ref_b, _ref_c, _row_start,
                                             _row_end};

    multiply_matrix_row _copy{_multiply_matrix_row};

    TNCT_LOG_TST(_log, format::bus::fmt("copy = ", _copy));

    return true;
  }
};

struct multiply_matrix_row_001
{

  static std::string desc()
  {
    return "";
  }

  bool operator()(const program::bus::options &)
  {
    using index = std::uint16_t;
    using data  = std::uint32_t;
    using multiply_matrix_row =
        container::internal::evt::multiply_matrix_cell<index, data>;

    static_assert(async::cpt::is_event<multiply_matrix_row>,
                  "multiply_matrix_row should be an event");

    return true;
  }
};

} // namespace tnct::container::tst

#endif

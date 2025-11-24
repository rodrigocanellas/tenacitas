/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_MULTIPLY_MATRIX_TEST_H
#define TNCT_CONTAINER_TST_MULTIPLY_MATRIX_TEST_H

#include <cstdint>
#include <optional>
#include <random>
#include <string>

#include "tnct/container/bus/multiply_matrix.h"
#include "tnct/container/dat/matrix.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using namespace tnct;

namespace tnct::container::tst
{

using log    = log::cerr;
using index  = std::uint32_t;
using data   = std::uint64_t;
using matrix = container::dat::matrix<index, data>;

std::uint16_t random_u16()
{
  static std::mt19937                          rng(std::random_device{}());
  std::uniform_int_distribution<std::uint16_t> dist(
      0, std::numeric_limits<std::uint16_t>::max());
  return dist(rng);
}

void fill_matrix(matrix &m)
{
  for (index _r = 0; _r < m.get_num_rows(); ++_r)
  {
    for (index _c = 0; _r < m.get_num_cols(); ++_c)
    {
      m.set_cell(_r, _c, random_u16());
    }
  }
}

struct multiply_matrix_000
{
  static std::string desc()
  {
    return "Multiply 2 matrixes sequentially";
  }

  bool operator()(const program::bus::options &)
  {

    log _log;
    _log.set_deb();

    auto _matrix_a{matrix::create(2, 3, 4)};
    if (!_matrix_a)
    {
      TNCT_LOG_ERR(_log, "error creating first matrix");
      return false;
    }

    if (!_matrix_a->set_cell(0, 0, 1) || !_matrix_a->set_cell(0, 1, 2)
        || !_matrix_a->set_cell(0, 2, 3) || !_matrix_a->set_cell(1, 0, 4)
        || !_matrix_a->set_cell(1, 1, 5) || !_matrix_a->set_cell(1, 2, 6))
    {
      TNCT_LOG_ERR(_log, "error filling first matrix");
      return false;
    }

    auto _matrix_b{matrix::create(3, 2, 5)};
    if (!_matrix_b)
    {
      TNCT_LOG_ERR(_log, "error creating second matrix");
      return false;
    }

    if (!_matrix_b->set_cell(0, 0, 7) || !_matrix_b->set_cell(0, 1, 8)
        || !_matrix_b->set_cell(1, 0, 9) || !_matrix_b->set_cell(1, 1, 10)
        || !_matrix_b->set_cell(2, 0, 11) || !_matrix_b->set_cell(2, 1, 12))
    {
      TNCT_LOG_ERR(_log, "error filling first matrix");
      return false;
    }

    auto _matrix_expected{matrix::create(2, 2, 0)};
    if (!_matrix_expected)
    {
      TNCT_LOG_ERR(_log, "error creating expected matrix");
      return false;
    }

    if (!_matrix_expected->set_cell(0, 0, 58)
        || !_matrix_expected->set_cell(0, 1, 64)
        || !_matrix_expected->set_cell(1, 0, 139)
        || !_matrix_expected->set_cell(1, 1, 154))
    {
      TNCT_LOG_ERR(_log, "error filling expected matrix");
      return false;
    }

    bus::mutiply_matrix<index, data, log> _mutiply_matrix(_log);

    auto _should_multipy_sequentially = [](const matrix &, const matrix &)
    { return true; };

    auto _matrix_c{
        _mutiply_matrix(*_matrix_a, *_matrix_b, _should_multipy_sequentially)};
    if (!_matrix_c)
    {
      return false;
    }

    TNCT_LOG_TST(_log, format::bus::fmt("\nmatrix_a: ", *_matrix_a,
                                        "\nmatrix_b: ", *_matrix_b,
                                        "\nexpected:", *_matrix_expected,
                                        "\ncalculated:", *_matrix_c));
    return _matrix_expected == _matrix_c;
  }
};

struct multiply_matrix_001
{
  static std::string desc()
  {
    return "Multiply 2 matrixes asynchronously";
  }

  bool operator()(const program::bus::options &)
  {

    log _log;
    _log.set_deb();

    auto _matrix_a{matrix::create(2, 3, 4)};
    if (!_matrix_a)
    {
      TNCT_LOG_ERR(_log, "error creating first matrix");
      return false;
    }

    if (!_matrix_a->set_cell(0, 0, 1) || !_matrix_a->set_cell(0, 1, 2)
        || !_matrix_a->set_cell(0, 2, 3) || !_matrix_a->set_cell(1, 0, 4)
        || !_matrix_a->set_cell(1, 1, 5) || !_matrix_a->set_cell(1, 2, 6))
    {
      TNCT_LOG_ERR(_log, "error filling first matrix");
      return false;
    }

    auto _matrix_b{matrix::create(3, 2, 5)};
    if (!_matrix_b)
    {
      TNCT_LOG_ERR(_log, "error creating second matrix");
      return false;
    }

    if (!_matrix_b->set_cell(0, 0, 7) || !_matrix_b->set_cell(0, 1, 8)
        || !_matrix_b->set_cell(1, 0, 9) || !_matrix_b->set_cell(1, 1, 10)
        || !_matrix_b->set_cell(2, 0, 11) || !_matrix_b->set_cell(2, 1, 12))
    {
      TNCT_LOG_ERR(_log, "error filling first matrix");
      return false;
    }

    auto _matrix_expected{matrix::create(2, 2, 0)};
    if (!_matrix_expected)
    {
      TNCT_LOG_ERR(_log, "error creating expected matrix");
      return false;
    }

    if (!_matrix_expected->set_cell(0, 0, 58)
        || !_matrix_expected->set_cell(0, 1, 64)
        || !_matrix_expected->set_cell(1, 0, 139)
        || !_matrix_expected->set_cell(1, 1, 154))
    {
      TNCT_LOG_ERR(_log, "error filling expected matrix");
      return false;
    }

    bus::mutiply_matrix<index, data, log> _mutiply_matrix(_log);

    auto _matrix_c{_mutiply_matrix(*_matrix_a, *_matrix_b)};
    if (!_matrix_c)
    {
      return false;
    }

    TNCT_LOG_TST(_log, format::bus::fmt("\nmatrix_a: ", *_matrix_a,
                                        "\nmatrix_b: ", *_matrix_b,
                                        "\nexpected:", *_matrix_expected,
                                        "\ncalculated:", *_matrix_c));
    return _matrix_expected == _matrix_c;
  }
};

} // namespace tnct::container::tst

#endif

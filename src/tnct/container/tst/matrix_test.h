/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_MATRIX_TEST_H
#define TNCT_CONTAINER_TST_MATRIX_TEST_H

#include <cstdint>
#include <string>

#include "tnct/container/dat/matrix.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/program/bus/options.h"

using namespace tnct;

namespace tnct::container::tst
{
struct matrix_000
{
  static std::string desc()
  {
    return "simple square container::dat::matrix creation and element access";
  }

  bool operator()(const program::bus::options &)
  {
    log::cerr _log;
    using matrix = container::dat::matrix<uint16_t, char>;

    if (auto _matrix{matrix::create(3, 3, 'z')}; _matrix)
    {

      _log.tst(format::bus::fmt(*_matrix));

      if (_matrix->get_cell(1, 2) != 'z')
      {
        _log.err(format::bus::fmt(
            "container::dat::matrix(1,2) should be 'z', but it is '",
            *_matrix->get_cell(1, 2), '\''));
        return false;
      }
      _log.tst(format::bus::fmt("container::dat::matrix(1,2) is '",
                                *_matrix->get_cell(1, 2), '\''));
      return true;
    }
    _log.err("error creating matrix");
    return false;
  }
};

struct matrix_001
{
  static std::string desc()
  {
    return "create square container::dat::matrix, modify and element access";
  }

  bool operator()(const program::bus::options &)
  {
    log::cerr _log;
    using matrix = container::dat::matrix<uint16_t, char>;

    if (auto _matrix{matrix::create(3, 3, 'z')}; _matrix)
    {

      _log.tst(format::bus::fmt(*_matrix));

      if (_matrix->get_cell(1, 2) != 'z')
      {
        _log.err(format::bus::fmt(
            "container::dat::matrix(1,2) should be 'z', but it is '",
            *_matrix->get_cell(1, 2), '\''));
        return false;
      }

      _matrix->set_cell(1, 2, 'e');

      if (_matrix->get_cell(1, 2) != 'e')
      {
        _log.err(format::bus::fmt(
            "container::dat::matrix(1,2) should be 'e', but it is '",
            *_matrix->get_cell(1, 2), '\''));
        return false;
      }

      _log.tst(format::bus::fmt(*_matrix));

      _log.tst(format::bus::fmt("container::dat::matrix(1,2) is '",
                                *_matrix->get_cell(1, 2), '\''));
      return true;
    }
    _log.err("error creating matrix");
    return false;
  }
};

struct matrix_002
{
  static std::string desc()
  {
    return "create non square container::dat::matrix creation, fill it, and "
           "element access";
  }

  bool operator()(const program::bus::options &)
  {
    log::cerr _log;
    using matrix = container::dat::matrix<uint16_t, int16_t>;
    if (auto _matrix{matrix::create(3, 4, -1)}; _matrix)
    {
      _log.tst(format::bus::fmt(*_matrix));

      if (_matrix->get_cell(1, 2) != -1)
      {
        _log.err(format::bus::fmt(
            "container::dat::matrix(1,2) should be -1, but it is '",
            *_matrix->get_cell(1, 2), '\''));
        return false;
      }
      _log.tst(format::bus::fmt("container::dat::matrix(1,2) is '",
                                *_matrix->get_cell(1, 2), '\''));

      uint16_t _i = 0;
      for (uint16_t _row = 0; _row < _matrix->get_num_rows(); ++_row)
      {
        for (uint16_t _col = 0; _col < _matrix->get_num_cols(); ++_col)
        {
          _log.tst(format::bus::fmt('[', _row, ',', _col, "] = ", _i));
          _matrix->set_cell(_row, _col, _i);
          ++_i;
        }
      }

      _log.tst(format::bus::fmt(*_matrix));

      if (_matrix->get_cell(1, 2) != 6)
      {
        _log.err(format::bus::fmt(
            "container::dat::matrix(1,2) should be 6, but it is '",
            *_matrix->get_cell(1, 2), '\''));
        return false;
      }

      _log.tst(format::bus::fmt("container::dat::matrix(1,2) is '",
                                *_matrix->get_cell(1, 2), '\''));

      return true;
    }
    _log.err("error creating matrix");
    return false;
  }
};

struct matrix_003
{
  static std::string desc()
  {
    return "test filling horizontally";
  }

  bool operator()(const program::bus::options &)
  {
    log::cerr _log;
    using matrix = container::dat::matrix<std::uint16_t, char>;
    auto _matrix{matrix::create(11, 8, '-')};
    if (!_matrix)
    {
      _log.err("error creating matrix");
      return false;
    }
    std::uint16_t _row{0};
    std::uint16_t _col{0};
    std::uint16_t _count{0};
    _log.tst(format::bus::fmt(*_matrix));
    std::string _s0{"open"};
    for (char _c : _s0)
    {
      _matrix->set_cell(_row, _col + _count++, _c);
    }
    _log.tst(format::bus::fmt(*_matrix));

    return ((_matrix->get_cell(0, 0) == 'o') && (_matrix->get_cell(0, 1) == 'p')
            && (_matrix->get_cell(0, 2) == 'e')
            && (_matrix->get_cell(0, 3) == 'n'));
  }
};

struct matrix_004
{
  static std::string desc()
  {
    return "test filling vertically";
  }

  bool operator()(const program::bus::options &)
  {
    log::cerr _log;
    using matrix = container::dat::matrix<uint16_t, char>;
    auto _matrix{matrix::create(11, 8, '-')};
    {
      _log.err("error creating matrix");
      return false;
    }
    uint16_t _row{0};
    uint16_t _col{0};
    uint16_t _count{0};
    _log.tst(format::bus::fmt(*_matrix));
    std::string _s0{"open"};
    for (char _c : _s0)
    {
      _matrix->set_cell(_row + _count++, _col, _c);
    }
    _log.tst(format::bus::fmt(*_matrix));

    return ((_matrix->get_cell(0, 0) == 'o') && (_matrix->get_cell(1, 0) == 'p')
            && (_matrix->get_cell(2, 0) == 'e')
            && (_matrix->get_cell(3, 0) == 'n'));
  }
};

} // namespace tnct::container::tst

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_BUS_MULTIPLY_MATRIX_H
#define TNCT_CONTAINER_BUS_MULTIPLY_MATRIX_H

#include <functional>
#include <optional>

#include "tnct/container/dat/matrix.h"
#include "tnct/container/internal/bus/create_matrix_for_multiply.h"
#include "tnct/container/internal/bus/multiply_matrix_async.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"

namespace tnct::container::bus
{

template <std::unsigned_integral t_index, typename t_data>
requires(std::copyable<t_data>
         && (std::integral<t_data> || std::floating_point<t_data>))
using should_multipy_sequentially =
    std::function<bool(const dat::matrix<t_index, t_data> &,
                       const dat::matrix<t_index, t_data> &)>;

template <std::unsigned_integral t_index, std::copyable t_data,
          log::cpt::logger t_logger>
struct mutiply_matrix
{

  using index  = t_index;
  using data   = t_data;
  using logger = t_logger;

  using matrix = container::dat::matrix<index, data>;

  mutiply_matrix()                       = delete;
  mutiply_matrix(const mutiply_matrix &) = delete;
  mutiply_matrix(mutiply_matrix &&)      = delete;

  mutiply_matrix(logger &p_logger)

      : m_logger(p_logger)
  {
  }

  std::optional<matrix> operator()(
      const matrix &_matrix_a, const matrix &_matrix_b,
      should_multipy_sequentially<index, data> &&p_should_multipy_sequentially =
          [](const matrix &, const matrix &) { return false; }) noexcept
  {
    try
    {
      if (_matrix_a.get_num_cols() != _matrix_b.get_num_rows())
      {
        TNCT_LOG_ERR(
            m_logger,
            format::bus::fmt(
                "Could not mutiply matrixes because number of columns (",
                _matrix_a.get_num_cols(),
                ") of the first matrix is not equal to the number of rows (",
                _matrix_b.get_num_rows(), ") of the second matrix"));
        return std::nullopt;
      }

      if (p_should_multipy_sequentially(_matrix_a, _matrix_b))
      {
        return multipy_sequentially(_matrix_a, _matrix_b);
      }

      return multiply_asynchronously(_matrix_a, _matrix_b);
    }
    catch (...)
    {
    }
    return std::nullopt;
  }

  mutiply_matrix &operator=(const mutiply_matrix &) = delete;
  mutiply_matrix &operator=(mutiply_matrix &&)      = delete;

  void *operator new(std::size_t)   = delete;
  void *operator new[](std::size_t) = delete;

private:
  std::optional<matrix> multipy_sequentially(const matrix &p_matrix_a,
                                             const matrix &p_matrix_b)
  {
    // std::size_t _num_cells_calculated{0};
    if (std::optional<matrix> _matrix_c{
            internal::bus::create_matrix_for_multiply(p_matrix_a, p_matrix_b)};
        _matrix_c)
    {
      TNCT_LOG_DEB(m_logger, "multipy_sequentially starting");
      for (index _r = 0; _r < p_matrix_a.get_num_rows(); ++_r)
      {
        for (index _c = 0; _c < p_matrix_b.get_num_cols(); ++_c)
        {

          for (index _k = 0; _k < p_matrix_a.get_num_cols(); _k++)
          {
            (*_matrix_c)(_r, _c) += p_matrix_a(_r, _k) * p_matrix_b(_k, _c);
            // _matrix_c->set_cell(_r, _c,
            //                     *_matrix_c->get_cell(_r, _c)
            //                         + (*p_matrix_a.get_cell(_r, _k))
            //                               * (*p_matrix_b.get_cell(_k, _c)));
          }
          // TNCT_LOG_DEB(m_logger, format::bus::fmt("_num_cells_calculated = ",
          //                                         ++_num_cells_calculated,
          //                                         ", C[", _r, ",", _c, "]"));
        }
      }

      TNCT_LOG_DEB(m_logger, "multipy_sequentially ending");

      return _matrix_c;
    }

    return std::nullopt;
  }

  std::optional<matrix> multiply_asynchronously(const matrix &p_matrix_a,
                                                const matrix &p_matrix_b)
  {
    TNCT_LOG_DEB(m_logger, "multiply_asynchronously starting");
    internal::bus::multiply_matrix_async<index, data, logger>
        _multiply_matrix_async{m_logger};

    auto _matric_c{_multiply_matrix_async(p_matrix_a, p_matrix_b)};

    TNCT_LOG_DEB(m_logger, "multiply_asynchronously ending");

    return std ::move(_matric_c);
  }

private:
  logger &m_logger;
};

} // namespace tnct::container::bus

#endif

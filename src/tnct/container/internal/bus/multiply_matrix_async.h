/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_INTERNAL_BUS_MULTIPLY_MATRIX_H
#define TNCT_CONTAINER_INTERNAL_BUS_MULTIPLY_MATRIX_H

#include <cmath>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include "tnct/async/bus/dispatcher.h"
#include "tnct/async/dat/handling_priority.h"
#include "tnct/container/dat/circular_queue.h"
#include "tnct/container/dat/matrix.h"
#include "tnct/container/internal/bus/create_matrix_for_multiply.h"
#include "tnct/container/internal/evt/multiply_matrix_cell.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"

namespace tnct::container::internal::bus
{

template <std::unsigned_integral t_index, typename t_data,
          log::cpt::logger       t_logger>
requires(std::copyable<t_data>
         && (std::integral<t_data> || std::floating_point<t_data>))
struct multiply_matrix_async
{
  using index  = t_index;
  using data   = t_data;
  using matrix = dat::matrix<index, data>;
  using logger = t_logger;

  multiply_matrix_async(logger &p_logger)
      : m_logger(p_logger), m_dispatcher(p_logger),
        m_num_threads{
            static_cast<std::size_t>(std::thread::hardware_concurrency())}
  {

    {
      using queue =
          container::dat::circular_queue<logger, multiply_matrix_cell, 10000>;
      m_dispatcher.template add_handling<multiply_matrix_cell>(
          "multiply_matrix_cell", queue{m_logger},
          std::bind_front(&multiply_matrix_async::handle_multiply_matrix_cell,
                          this),
          async::dat::handling_priority::highest, m_num_threads);
    }
  }

  std::optional<matrix> operator()(const matrix &p_matrix_a,
                                   const matrix &p_matrix_b)
  {

    std::optional<matrix> _opt_matrix_c{
        create_matrix_for_multiply(p_matrix_a, p_matrix_b)};
    if (!_opt_matrix_c)
    {
      TNCT_LOG_ERR(m_logger, "Error creating resulting matrix");
      return std::nullopt;
    }

    std::reference_wrapper<const matrix> _ref_matrix_a{p_matrix_a};
    std::reference_wrapper<const matrix> _ref_matrix_b{p_matrix_b};
    std::reference_wrapper<matrix>       _ref_matrix_c{*_opt_matrix_c};

    const index _num_rows_a{p_matrix_a.get_num_rows()};
    const index _chunk_size =
        ceil(static_cast<float>(_num_rows_a) / m_num_threads);

    for (index _i = 0; _i < m_num_threads; _i++)
    {
      index _start_row_c = std::min(_i * _chunk_size, _num_rows_a);
      index _end_row_c   = std::min((_i + 1) * _chunk_size, _num_rows_a);

      const auto _result = m_dispatcher.template publish<multiply_matrix_cell>(
          _ref_matrix_a, _ref_matrix_b, _ref_matrix_c, _start_row_c,
          _end_row_c);
      if (_result != async::dat::result::OK)
      {
        TNCT_LOG_ERR(
            m_logger,
            format::bus::fmt("Error publishing multiply_matrix_cell for row ",
                             _start_row_c, ": ", _result));
        return std::nullopt;
      }
    }

    // wait for results...
    // TNCT_LOG_DEB(m_logger, "############# WAITING");
    {
      std::unique_lock<std::mutex> _lock{m_mutex};
      m_cond.wait(_lock,
                  [&]() -> bool
                  { return (m_num_rows_processed == _num_rows_a); });
    }
    // TNCT_LOG_DEB(m_logger, "############# DONE WAITING");
    return std::move(_opt_matrix_c);
  }

private:
  using multiply_matrix_cell = internal::evt::multiply_matrix_cell<index, data>;
  using dispatcher = async::bus::dispatcher<t_logger, multiply_matrix_cell>;

private:
  void handle_multiply_matrix_cell(multiply_matrix_cell &&p_event)
  {
    // TNCT_LOG_DEB(m_logger, format::bus::fmt("event = ", p_event));
    const matrix &_matrix_a{p_event.m_matrix_a.get()};
    const matrix &_matrix_b{p_event.m_matrix_b.get()};
    matrix       &_matrix_c{p_event.m_matrix_c.get()};
    const index   _row_start{p_event.m_row_begin};
    const index   _row_end{p_event.m_row_end};

    for (index _r = _row_start; _r < _row_end; ++_r)
    {
      for (index _c = 0; _c < _matrix_b.get_num_cols(); ++_c)
      {
        _matrix_c(_r, _c) = static_cast<data>(0);

        for (index _k = 0; _k < _matrix_a.get_num_cols(); _k++)
        {
          _matrix_c(_r, _c) += _matrix_a(_r, _k) * _matrix_b(_k, _c);
        }
      }
    }
    {
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_num_rows_processed += (_row_end - _row_start);
      m_cond.notify_one();
    }
  }

private:
  logger                 &m_logger;
  dispatcher              m_dispatcher;
  std::size_t             m_num_threads;
  std::atomic_size_t      m_num_rows_processed{0};
  std::condition_variable m_cond;
  std::mutex              m_mutex;
};

} // namespace tnct::container::internal::bus

#endif

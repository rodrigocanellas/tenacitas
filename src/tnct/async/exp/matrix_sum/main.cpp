/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example matrix_sum/main.cpp

#include <chrono>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <optional>
#include <string_view>
#include <vector>

#include "tnct/async/dispatcher.h"
#include "tnct/async/result.h"
#include "tnct/container/circular_queue.h"
#include "tnct/container/matrix.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"

using namespace tnct;

using logger = log::cerr;

using matrix = container::matrix<std::size_t, std::size_t>;

void syntax(std::string_view p_pgm)
{
  std::cout << "Syntax: " << p_pgm << " <matrix-size>\n"
            << "Example: " << p_pgm << " 100 " << std::endl;
}

std::optional<std::size_t> get_matriz_size(logger          &p_logger,
                                           std::string_view p_param)
{
  try
  {
    auto _param{std::string(p_param)};
    return std::stoul(_param);
  }
  catch (std::exception *_ex)
  {
    TNCT_LOG_ERR(p_logger, format::fmt(p_param, " is not a valid number"));
  }
  return std::nullopt;
}

matrix fill_matrix(std::size_t p_matrix_size)
{
  matrix::data _data{1};
  matrix       _matrix{p_matrix_size, p_matrix_size, matrix::data{0}};
  for (matrix::index _r = 0; _r < _matrix.get_num_rows(); ++_r)
  {
    for (matrix::index _c = 0; _c < _matrix.get_num_cols(); ++_c)
    {
      _matrix(_r, _c) = _data++;
    }
  }
  return _matrix;
}

matrix::data sum_matrix(const matrix &p_matrix)
{
  matrix::data _sum{0};

  for (matrix::index _r = 0; _r < p_matrix.get_num_rows(); ++_r)
  {
    for (matrix::index _c = 0; _c < p_matrix.get_num_cols(); ++_c)
    {
      _sum += p_matrix(_r, _c);
    }
  }

  return _sum;
}

void sync_sum_matrix_wrapper(const matrix &p_matrix, logger &p_logger)
{
  const auto _start = std::chrono::high_resolution_clock::now();

  const auto _sum{sum_matrix(p_matrix)};

  const auto _end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> _diff = _end - _start;

  TNCT_LOG_TST(p_logger, format::fmt("SYNC time = ", _diff.count(),
                                     " seconds, sum: ", _sum));
}

namespace evt
{

struct matrix_summed
{
  matrix_summed() = default;
  matrix_summed(matrix::data p_sum) : m_sum(p_sum)
  {
  }

  matrix_summed(const matrix_summed &p_matrix_summed)
      : m_sum(p_matrix_summed.m_sum)
  {
  }

  matrix_summed(matrix_summed &&p_matrix_summed) : m_sum(p_matrix_summed.m_sum)
  {
  }

  matrix_summed &operator=(const matrix_summed &p_matrix_summed)
  {
    m_sum = p_matrix_summed.m_sum;
    return *this;
  }

  matrix_summed &operator=(matrix_summed &&p_matrix_summed)
  {
    m_sum = p_matrix_summed.m_sum;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream        &p_out,
                                  const matrix_summed &p_matrix_summed)
  {
    p_out << "matrix sum " << p_matrix_summed.m_sum << std::endl;
    return p_out;
  }

  matrix::data m_sum;
};

} // namespace evt

using dispatcher = async::dispatcher<logger, evt::matrix_summed>;

struct async_sum_matrix
{
  async_sum_matrix(dispatcher &p_dispatcher, logger &p_logger, matrix &p_matrix)
      : m_dispatcher(p_dispatcher), m_logger(p_logger), m_matrix(p_matrix)
  {
    define_handlers();
  }

  async::result operator()()
  {
    for (matrix::index _r = 0; _r < m_matrix.get_num_rows(); ++_r)
    {
      auto _result{m_internal_dispatcher.publish<sum_line>(_r)};
      if (_result != async::result::OK)
      {
        TNCT_LOG_ERR(m_logger,
                     format::fmt("error publishing 'sum_line': ", _result));
        return _result;
      }
    }
    return async::result::OK;
  }

private:
  struct sum_line
  {
    sum_line(matrix::index p_row) : m_row{p_row}
    {
    }

    sum_line()                            = default;
    sum_line(const sum_line &)            = default;
    sum_line(sum_line &&p_sum_line)       = default;
    sum_line &operator=(const sum_line &) = default;
    sum_line &operator=(sum_line &&)      = default;

    friend std::ostream &operator<<(std::ostream   &p_out,
                                    const sum_line &p_sum_line)
    {
      p_out << "sum line " << p_sum_line.m_row << std::endl;
      return p_out;
    }

    matrix::index m_row;
  };

  struct line_summed
  {
    line_summed(matrix::index p_row, matrix::data p_sum)
        : m_row(p_row), m_sum(p_sum)
    {
    }

    line_summed()                               = default;
    line_summed(const line_summed &)            = default;
    line_summed(line_summed &&)                 = default;
    line_summed &operator=(const line_summed &) = default;
    line_summed &operator=(line_summed &&)      = default;

    friend std::ostream &operator<<(std::ostream      &p_out,
                                    const line_summed &p_line_summed)
    {
      p_out << "line summed " << p_line_summed.m_row << ", "
            << p_line_summed.m_sum << std::endl;
      return p_out;
    }

    matrix::index m_row;
    matrix::data  m_sum;
  };

  using internal_dispatcher = async::dispatcher<logger, sum_line, line_summed>;

  void define_handlers()
  {
    using sum_line_queue = container::circular_queue<logger, sum_line, 200>;
    m_internal_dispatcher.add_handling<sum_line>(
        "sum-line", sum_line_queue{m_logger},
        line_summer{m_internal_dispatcher, m_logger, m_matrix},
        async::handling_priority::medium, 8);

    using line_summed_queue =
        container::circular_queue<logger, line_summed, 10>;
    m_internal_dispatcher.add_handling<line_summed>(
        "line-summed", line_summed_queue{m_logger},
        matrix_summer{m_dispatcher, m_logger, m_matrix.get_num_rows()},
        async::handling_priority::medium, 1);
  }

  struct line_summer
  {
    line_summer(internal_dispatcher &p_dispatcher, logger &p_logger,
                matrix &p_matrix)
        : m_internal_dispatcher(p_dispatcher), m_logger(p_logger),
          m_matrix(p_matrix)
    {
    }

    void operator()(sum_line &&p_sum_line)
    {
      matrix::data _sum{0};
      for (matrix::index _c = 0; _c < m_matrix.get_num_cols(); ++_c)
      {
        _sum += m_matrix(p_sum_line.m_row, _c);
      }
      auto _result{
          m_internal_dispatcher.publish<line_summed>(p_sum_line.m_row, _sum)};
      if (_result != async::result::OK)
      {
        TNCT_LOG_ERR(m_logger,
                     format::fmt("error publishing 'line_summed': ", _result));
      }
    }

  private:
    internal_dispatcher &m_internal_dispatcher;
    logger              &m_logger;
    matrix              &m_matrix;
  };

  struct matrix_summer
  {
    matrix_summer(dispatcher &p_dispatcher, logger &p_logger,
                  matrix::index p_num_rows)
        : m_dispatcher(p_dispatcher), m_logger(p_logger), m_num_rows(p_num_rows)
    {
      if (m_num_rows == 0)
      {
        throw std::runtime_error("number of rows can not be 0");
      }
    }

    void operator()(line_summed &&p_line_summed)
    {
      m_sum += p_line_summed.m_sum;
      if (all_rows_summed())
      {
        auto _result{m_dispatcher.publish<evt::matrix_summed>(m_sum)};
        if (_result != async::result::OK)
        {
          TNCT_LOG_ERR(
              m_logger,
              format::fmt("error publishing 'matrix_summed': ", _result));
        }
      }
    }

  private:
    bool all_rows_summed()
    {
      ++m_num_rows_summed;
      if (m_num_rows_summed == m_num_rows)
      {
        return true;
      }
      return false;
    }

  private:
    dispatcher &m_dispatcher;
    logger     &m_logger;

    matrix::index m_num_rows{0};
    matrix::index m_num_rows_summed{0};
    matrix::data  m_sum{0};
  };

private:
  dispatcher         &m_dispatcher;
  logger             &m_logger;
  matrix             &m_matrix;
  internal_dispatcher m_internal_dispatcher{m_logger};
};

void async_sum_matrix_wrapper(matrix &p_matrix, logger &p_logger)
{
  dispatcher _dispatcher(p_logger);

  std::mutex              _mutex;
  std::condition_variable _cond;
  std::size_t             _sum{std::numeric_limits<std::size_t>::max()};

  using matrix_summed_queue =
      container::circular_queue<logger, evt::matrix_summed, 1>;
  auto _end_of_pgm = [&](evt::matrix_summed &&p_event)
  {
    _sum = p_event.m_sum;
    _cond.notify_one();
  };

  _dispatcher.add_handling<evt::matrix_summed>(
      "matrix-summed", matrix_summed_queue{p_logger}, std::move(_end_of_pgm),
      async::handling_priority::medium, 1);

  async_sum_matrix _async_sum_matrix(_dispatcher, p_logger, p_matrix);

  const auto _start = std::chrono::high_resolution_clock::now();

  _async_sum_matrix();

  std::unique_lock<std::mutex> _lock{_mutex};
  _cond.wait(_lock);

  const auto _end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> _diff = _end - _start;

  TNCT_LOG_TST(p_logger, format::fmt("ASYNC time = ", _diff.count(),
                                     " seconds, sum: ", _sum));
}
// clang-format off
//rodrigo@wayne:~/development/prd/linux-release-64/exp$ ./tnct.async.exp.matrix_sum 60260
//TST|2025-04-06 11:58:08,584990|140374769088320|main.cpp                           |00079|time = 1.73989 seconds, sum: 6593052193220513800
//TST|2025-04-06 11:58:09,747472|140374769088320|main.cpp                           |00308|time = 0.975312 seconds, sum: 6593052193220513800
//  clang-format on

int main(int argc, char **argv) {
  if (argc != 2) {
    syntax(argv[0]);
    return -1;
  }

  logger _logger;
  _logger.set_war();
  std::size_t _matriz_size{get_matriz_size(_logger, argv[1]).value_or(0)};

  if (_matriz_size == 0) {
    syntax(argv[0]);
    return -2;
  }

  matrix _matrix{fill_matrix(_matriz_size)};

  sync_sum_matrix_wrapper(_matrix, _logger);

  async_sum_matrix_wrapper(_matrix, _logger);
}

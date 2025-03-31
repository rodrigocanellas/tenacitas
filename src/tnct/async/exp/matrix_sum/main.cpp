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
#include "tnct/async/handling_definition.h"
#include "tnct/async/result.h"
#include "tnct/container/circular_queue.h"
#include "tnct/container/matrix.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/traits/log/logger.h"

using namespace tnct;

using logger = log::cerr;

using matrix = container::matrix<std::size_t, std::size_t>;

void syntax(std::string_view p_pgm) {
  std::cout << "Syntax: " << p_pgm << " <matrix-size>\n"
            << "Example: " << p_pgm << " 100 " << std::endl;
}

std::optional<std::size_t> get_matriz_size(logger &p_logger,
                                           std::string_view p_param) {
  try {
    auto _param{std::string(p_param)};
    return std::stoul(_param);
  } catch (std::exception *_ex) {
    TNCT_LOG_ERR(p_logger, format::fmt(p_param, " is not a valid number"));
  }
  return std::nullopt;
}

matrix fill_matrix(std::size_t p_matrix_size) {
  matrix::data _data{1};
  matrix _matrix{p_matrix_size, p_matrix_size, matrix::data{0}};
  for (matrix::index _r = 0; _r < _matrix.get_num_rows(); ++_r) {
    for (matrix::index _c = 0; _c < _matrix.get_num_cols(); ++_c) {
      _matrix(_r, _c) = _data++;
    }
  }
  return _matrix;
}

matrix::data sum_matrix(const matrix &p_matrix) {
  matrix::data _sum{0};

  for (matrix::index _r = 0; _r < p_matrix.get_num_rows(); ++_r) {
    for (matrix::index _c = 0; _c < p_matrix.get_num_cols(); ++_c) {
      _sum += p_matrix(_r, _c);
    }
  }

  return _sum;
}

namespace evt {

struct sum_line {
  sum_line() = default;
  sum_line(matrix *p_matrix, matrix::index p_row)
      : m_matrix{p_matrix}, m_row{p_row} {}

  sum_line(const sum_line &p_sum_line)
      : m_matrix(p_sum_line.m_matrix), m_row(p_sum_line.m_row) {}

  sum_line(sum_line &&p_sum_line)
      : m_matrix(p_sum_line.m_matrix), m_row(p_sum_line.m_row) {}

  sum_line &operator=(const sum_line &p_matrix_summed) {
    m_matrix = p_matrix_summed.m_matrix;
    m_row = p_matrix_summed.m_row;

    return *this;
  }

  sum_line &operator=(sum_line &&p_matrix_summed) {
    m_matrix = p_matrix_summed.m_matrix;
    m_row = p_matrix_summed.m_row;

    return *this;
  }

  matrix *m_matrix;
  matrix::index m_row;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const sum_line &p_sum_line) {
    p_out << "sum line " << p_sum_line.m_row << std::endl;
    return p_out;
  }
};

struct line_summed {
  line_summed() = default;
  line_summed(matrix::index p_row, matrix::data p_sum)
      : m_row(p_row), m_sum(p_sum) {}

  line_summed(const line_summed &p_line_summed)
      : m_row(p_line_summed.m_row), m_sum(p_line_summed.m_sum) {}

  line_summed(line_summed &&p_line_summed)
      : m_row(p_line_summed.m_row), m_sum(p_line_summed.m_sum) {}

  line_summed &operator=(const line_summed &p_matrix_summed) {
    m_row = p_matrix_summed.m_row;
    m_sum = p_matrix_summed.m_sum;
    return *this;
  }

  line_summed &operator=(line_summed &&p_matrix_summed) {
    m_row = p_matrix_summed.m_row;
    m_sum = p_matrix_summed.m_sum;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const line_summed &p_line_summed) {
    p_out << "line summed " << p_line_summed.m_row << ", "
          << p_line_summed.m_sum << std::endl;
    return p_out;
  }

  matrix::index m_row;
  matrix::data m_sum;
};

struct matrix_summed {
  matrix_summed() = default;
  matrix_summed(matrix::data p_sum) : m_sum(p_sum) {}

  matrix_summed(const matrix_summed &p_matrix_summed)
      : m_sum(p_matrix_summed.m_sum) {}

  matrix_summed(matrix_summed &&p_matrix_summed)
      : m_sum(p_matrix_summed.m_sum) {}

  matrix_summed &operator=(const matrix_summed &p_matrix_summed) {
    m_sum = p_matrix_summed.m_sum;
    return *this;
  }

  matrix_summed &operator=(matrix_summed &&p_matrix_summed) {
    m_sum = p_matrix_summed.m_sum;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const matrix_summed &p_matrix_summed) {
    p_out << "matrix sum " << p_matrix_summed.m_sum << std::endl;
    return p_out;
  }

  matrix::data m_sum;
};

} // namespace evt

using dispatcher = async::dispatcher<logger, evt::sum_line, evt::line_summed,
                                     evt::matrix_summed>;

namespace dom {

struct line_summer {
  line_summer(dispatcher &p_dispatcher, logger &p_logger)
      : m_dispatcher(p_dispatcher), m_logger(p_logger) {}

  void operator()(evt::sum_line &&p_sum_line) {
    matrix::data _sum{0};
    for (matrix::index _c = 0; _c < p_sum_line.m_matrix->get_num_cols(); ++_c) {
      _sum += p_sum_line.m_matrix->operator()(p_sum_line.m_row, _c);
    }
    auto _result{
        m_dispatcher.publish<evt::line_summed>(p_sum_line.m_row, _sum)};
    if (_result != async::result::OK) {
      TNCT_LOG_ERR(m_logger,
                   format::fmt("error publishing 'line_summed': ", _result));
    }
  }

private:
  dispatcher &m_dispatcher;
  logger &m_logger;
};

struct matrix_summer {
  matrix_summer(dispatcher &p_dispatcher, logger &p_logger,
                matrix::index p_num_rows)
      : m_dispatcher(p_dispatcher), m_logger(p_logger),
        m_rows_summed(p_num_rows, false) {}

  void operator()(evt::line_summed &&p_line_summed) {
    m_sum += p_line_summed.m_sum;
    m_rows_summed[p_line_summed.m_row] = true;
    if (all_rows_summed()) {
      auto _result{m_dispatcher.publish<evt::matrix_summed>(m_sum)};
      if (_result != async::result::OK) {
        TNCT_LOG_ERR(m_logger, format::fmt("error publishing 'matrix_summed': ",
                                           _result));
      }
    }
  }

private:
  bool all_rows_summed() const {
    for (bool _b : m_rows_summed) {
      if (!_b) {
        return false;
      }
    }
    return true;
  }

private:
  dispatcher &m_dispatcher;
  logger &m_logger;

  std::vector<bool> m_rows_summed;
  matrix::data m_sum{0};
};

struct end_of_pgm {
  end_of_pgm(matrix::data &p_sum, std::condition_variable &p_cond)
      : m_sum(p_sum), m_cond(p_cond) {}

  void operator()(evt::matrix_summed &&p_matrix_summed) {
    m_sum = p_matrix_summed.m_sum;
    m_cond.notify_one();
  }

private:
  matrix::data &m_sum;
  std::condition_variable &m_cond;
};

} // namespace dom

void define_handlers(dispatcher &p_dispatcher, logger &p_logger,
                     matrix &p_matrix, std::condition_variable &p_cond_end,
                     matrix::data &p_sum) {
  using sum_line_queue = container::circular_queue<logger, evt::sum_line, 20>;

  p_dispatcher.add_handling(
      async::handling_definition<evt::sum_line, dom::line_summer,
                                 sum_line_queue>{
          "sum-line", dom::line_summer{p_dispatcher, p_logger},
          sum_line_queue{p_logger}, 4});

  using line_summed_queue =
      container::circular_queue<logger, evt::line_summed, 10>;

  p_dispatcher.add_handling(
      async::handling_definition<evt::line_summed, dom::matrix_summer,
                                 line_summed_queue>{
          "line-summed",
          dom::matrix_summer{p_dispatcher, p_logger, p_matrix.get_num_rows()},
          line_summed_queue{p_logger}, 1});

  using matrix_summed_queue =
      container::circular_queue<logger, evt::matrix_summed, 1>;

  p_dispatcher.add_handling(

      async::handling_definition<evt::matrix_summed, dom::end_of_pgm,
                                 matrix_summed_queue>{
          "matrix-summed", dom::end_of_pgm{p_sum, p_cond_end},
          matrix_summed_queue{p_logger}, 1});
}

int main(int argc, char **argv) {
  if (argc != 2) {
    syntax(argv[0]);
    return -1;
  }

  logger _logger;

  std::size_t _matriz_size{get_matriz_size(_logger, argv[1]).value_or(0)};

  if (_matriz_size == 0) {
    return -2;
  }

  matrix _matrix{fill_matrix(_matriz_size)};

  {
    const auto _start = std::chrono::high_resolution_clock::now();

    std::cout << "sync sum: " << sum_matrix(_matrix) << std::endl;

    const auto _end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double> _diff = _end - _start;

    TNCT_LOG_TST(_logger, format::fmt("time = ", _diff.count(), " seconds"));
  }

  matrix::data _sum{std::numeric_limits<matrix::data>::max()};
  dispatcher _dispatcher(_logger);

  std::mutex _mutex;
  std::condition_variable _cond;

  define_handlers(_dispatcher, _logger, _matrix, _cond, _sum);

  const auto _start = std::chrono::high_resolution_clock::now();

  for (matrix::index _r = 0; _r < _matrix.get_num_rows(); ++_r) {
    auto _result{_dispatcher.publish<evt::sum_line>(&_matrix, _r)};
    if (_result != async::result::OK) {
      TNCT_LOG_ERR(_logger,
                   format::fmt("error publishing 'sum_line': ", _result));
      return -3;
    }
  }

  std::unique_lock<std::mutex> _lock{_mutex};
  _cond.wait(_lock);

  const auto _end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> _diff = _end - _start;

  TNCT_LOG_TST(_logger, format::fmt("time = ", _diff.count(), " seconds"));

  std::cout << "async sum: " << _sum << std::endl;
}

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <optional>

#include "tnct/container/bus/multiply_matrix.h"
#include "tnct/container/dat/matrix.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using namespace tnct;

namespace
{
using logger = log::cerr;
using index  = std::uint32_t;
using data   = std::uint64_t;
using matrix = container::dat::matrix<index, data>;

// << " --num_runs <num-runs> --rows_a <rows-a> --cols_b <cols-b> "
//    "--same_index <same-index> --initial_value_a <initial-value-a> "
//    "--initial_value_b <initial-value-b>

struct params
{
  std::uint16_t num_runs{1};
  index         num_rows_a{1000};
  index         num_cols_b{1000};
  index         value_same_index{20};
  data          initial_value_a{1};
  data          initial_value_b{2};
};

std::optional<params> read_options(int p_argc, char **p_argv, logger &p_logger)
{

  program::bus::options _options;

  try
  {
    _options.parse(p_argc, p_argv, {"num_rows_a", "num_cols_b"});
  }
  catch (...)
  {
    std::cout
        << "Syntax: " << p_argv[0]
        << " --num_runs <num-runs> --num_rows_a <num-rows-a> "
           "--num_cols_b <num-cols-b> --value_same_index "
           "<value-same-index> --initial_value_a <initial-value-a> "
           "--initial_value_b <initial-value-b>"
        << "\n\nWhere\n\t"
        << "num_runs is the number of times the multiplication should run\n\t"
        << "num_rows_a is the number of rows in matrix A - mandatory\n\t"
        << "num_cols_b is the number of columns in matrix B - mandatory\n\t"
        << "value_same_index is number of columns in A which will be the "
           "same as the number of rows in B\n\t"
        << "initial_value_a is the initial value of all the cells in A\n\t"
        << "initial_value_b is the initial value of all the cells in B\n\t"
        << "sequential if present, indicates that the multiplication should "
           "be done sequentially"
        << std::endl;
    return std::nullopt;
  }

  params _params;

  {
    const auto _num_runs{_options.get_single_param("num_runs")};
    if (_num_runs)
    {
      _params.num_runs =
          static_cast<decltype(_params.num_runs)>(std::stoi(*_num_runs));
    }
  }

  {
    const auto _num_rows_a{_options.get_single_param("num_rows_a")};
    if (!_num_rows_a)
    {
      TNCT_LOG_WAR(p_logger,
                   format::bus::fmt("'num_rows_a' parameter not found"));
    }
    else
    {
      _params.num_rows_a =
          static_cast<decltype(_params.num_rows_a)>(std::stoi(*_num_rows_a));
    }
  }

  {
    const auto _num_cols_b{_options.get_single_param("num_cols_b")};
    if (!_num_cols_b)
    {
      TNCT_LOG_WAR(p_logger,
                   format::bus::fmt("'num_cols_b' parameter not found"));
    }
    else
    {
      _params.num_cols_b =
          static_cast<decltype(_params.num_rows_a)>(std::stoi(*_num_cols_b));
    }
  }

  {
    const auto _value_same_index{_options.get_single_param("value_same_index")};
    if (_value_same_index)
    {
      _params.value_same_index =
          static_cast<decltype(_params.value_same_index)>(
              std::stoi(*_value_same_index));
    }
  }

  {
    const auto _initial_value_a{_options.get_single_param("initial_value_a")};
    if (_initial_value_a)
    {
      _params.initial_value_a = static_cast<decltype(_params.initial_value_a)>(
          std::stoul(*_initial_value_a));
    }
  }

  {
    const auto _initial_value_b{_options.get_single_param("initial_value_b")};
    if (_initial_value_b)
    {
      _params.initial_value_b = static_cast<decltype(_params.initial_value_b)>(
          std::stoul(*_initial_value_b));
    }
  }

  return {_params};
}

std::optional<std::chrono::duration<double>>
multiply_big_matrixes(logger &p_logger, const params &p_params,
                      bool p_sequential)
{

  auto _should_multipy_sequentially = [&](const matrix &, const matrix &)
  { return p_sequential; };

  auto _matrix_a{matrix::create(p_params.num_rows_a, p_params.value_same_index,
                                p_params.initial_value_a)};
  if (!_matrix_a)
  {
    TNCT_LOG_ERR(p_logger, "error creating first matrix");
    return std::nullopt;
  }

  auto _matrix_b{matrix::create(p_params.value_same_index, p_params.num_cols_b,
                                p_params.initial_value_b)};
  if (!_matrix_b)
  {
    TNCT_LOG_ERR(p_logger, "error creating first matrix");
    return std::nullopt;
  }

  container::bus::mutiply_matrix<index, data, logger> _mutiply_matrix(p_logger);

  std::chrono::duration<double> _total_time(0);
  for (std::uint16_t _n = 0; _n < p_params.num_runs; ++_n)
  {
    TNCT_LOG_TST(p_logger,
                 format::bus::fmt("=============== Execution # ", _n));
    auto _start_time = std::chrono::high_resolution_clock::now();
    auto _matrix_c{
        _mutiply_matrix(*_matrix_a, *_matrix_b, _should_multipy_sequentially)};

    _total_time += std::chrono::high_resolution_clock::now() - _start_time;
    if (!_matrix_c)
    {
      TNCT_LOG_ERR(p_logger, "Error calculating matrix");
      return std::nullopt;
    }
  }
  _total_time /= p_params.num_runs;

  return _total_time;
}

} // namespace

int main(int p_argc, char **p_argv)
{

  logger     _logger;
  const auto _params{read_options(p_argc, p_argv, _logger)};
  if (_params)
  {
    const auto _parallel_time{multiply_big_matrixes(_logger, *_params, false)};
    const auto _sequential_time{multiply_big_matrixes(_logger, *_params, true)};

    if (!_parallel_time || !_sequential_time)
    {
      return -1;
    }

    const auto _speedup{*_sequential_time / *_parallel_time};

    TNCT_LOG_TST(
        _logger,
        format::bus::fmt("average sequential time = ", *_sequential_time,
                         ", average parallel time: ", *_parallel_time,
                         ", speedup = ", _speedup, ", efficiency = ",
                         100 * _speedup / std::thread::hardware_concurrency()));
  }
}

#ifndef TNCT_CROSSWORDS_BUS_GRID_CREATOR_H
#define TNCT_CROSSWORDS_BUS_GRID_CREATOR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <atomic>
#include <memory>
#include <type_traits>

#include "tnct/async/dispatcher.h"
#include "tnct/container/circular_queue.h"
#include "tnct/crosswords/bus/internal/assembler.h"
#include "tnct/crosswords/dat/error.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/dat/index.h"
#include "tnct/crosswords/evt/internal/grid_attempt_configuration.h"
#include "tnct/crosswords/evt/internal/grid_create_error.h"
#include "tnct/crosswords/evt/internal/grid_create_solved.h"
#include "tnct/crosswords/evt/internal/grid_create_start.h"
#include "tnct/crosswords/evt/internal/grid_create_stop.h"
#include "tnct/crosswords/evt/internal/grid_create_unsolved.h"
#include "tnct/crosswords/evt/internal/grid_permutations_tried.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/time/cpt/chrono_convertible.h"

namespace tnct::crosswords::bus
{

template <log::cpt::logger t_logger>
struct grid_creator
{
  grid_creator(
      t_logger                                   &p_logger,
      std::invocable<std::shared_ptr<dat::grid>, std::chrono::seconds,
                     std::uint64_t> auto          p_callback_on_solved,
      std::invocable<dat::index, dat::index> auto p_callback_on_unsolved)
      : m_logger{p_logger}, m_dispatcher{p_logger},
        m_assembler{p_logger, m_dispatcher}
  {
    on_start();
    on_stop();
    on_solved(p_callback_on_solved);
    on_unsolved(p_callback_on_unsolved);
  }

  ~grid_creator()
  {
    // m_dispatcher.template publish<evt::internal::grid_create_stop>();

    m_stop = true;
  }

  /// \brief
  ///
  /// \param p_entries,
  /// \param p_num_rows,
  /// \param p_num_cols,
  /// \param p_wait_for,
  /// \param p_max_num_rows
  [[nodiscard]] bool start(const dat::entries &p_entries, dat::index p_num_rows,
                           dat::index                         p_num_cols,
                           time::cpt::convertible_to_sec auto p_wait_for,
                           dat::index                         p_max_num_rows)
  {
    if (const auto _result =
            m_dispatcher.template publish<evt::internal::grid_create_start>(
                p_entries, p_num_rows, p_num_cols,
                std::chrono::duration_cast<std::chrono::seconds>(p_wait_for),
                p_max_num_rows);
        _result != async::result::OK)
    {
      TNCT_LOG_ERR(m_logger, format::fmt("Error ", _result,
                                         " publishing 'grid_create_start' for ",
                                         p_entries));
      return false;
    }
    return true;
  }

  void stop()
  {
    if (const auto _result =
            m_dispatcher.template publish<evt::internal::grid_create_stop>();
        _result != async::result::OK)
    {
      TNCT_LOG_ERR(m_logger, format::fmt("Error ", _result,
                                         " publishing 'grid_create_stop"));
    }
  }

  // \brief
  //
  // \tparam dat::index num rows,
  // \tparam dat::index num cols,
  // \tparam std::size_t max memory to be used
  // \tparam std::size_t memory available
  // \tparam std::size_t number of permutations
  void
  on_configuration(std::invocable<dat::index, dat::index, std::size_t,
                                  std::size_t, std::size_t> auto p_callback)
  {
    using evt::internal::grid_attempt_configuration;

    using queue =
        container::circular_queue<t_logger, grid_attempt_configuration, 5>;

    m_dispatcher.template add_handling<grid_attempt_configuration>(
        "grid-attempt-configuration", queue{m_logger},
        [p_callback](grid_attempt_configuration &&p_event)
        {
          p_callback(p_event.num_rows, p_event.num_cols,
                     p_event.max_memory_to_be_used, p_event.memory_available,
                     p_event.number_of_permutations);
        });
  }

  // \brief
  //
  // \tparam std::size_t number of permutations tried so far
  void on_permutations_tried(std::invocable<std::size_t> auto p_callback)
  {
    using evt::internal::grid_permutations_tried;

    using queue =
        container::circular_queue<t_logger, grid_permutations_tried, 500>;

    m_dispatcher.template add_handling<grid_permutations_tried>(
        "grid-permutations-tried", queue{m_logger},
        [p_callback](grid_permutations_tried &&p_event)
        { p_callback(p_event.permutations); });
  }

  void on_error(std::invocable<dat::error, const std::string &> auto p_callback)
  {
    using evt::internal::grid_create_error;

    using queue = container::circular_queue<t_logger, grid_create_error, 5>;

    m_dispatcher.template add_handling<grid_create_error>(
        "grid-create-error", queue{m_logger},
        [p_callback](grid_create_error &&p_event)
        { p_callback(p_event.error, p_event.description); });
  }

private:
  using dispatcher = tnct::async::dispatcher<
      t_logger, evt::internal::grid_permutations_tried,
      evt::internal::grid_create_solved, evt::internal::grid_create_start,
      evt::internal::grid_create_stop, evt::internal::grid_create_unsolved,
      evt::internal::grid_attempt_configuration,
      evt::internal::grid_create_error>;

private:
  void on_start()
  {
    using evt::internal::grid_create_start;
    using queue = container::circular_queue<t_logger, grid_create_start, 1000>;
    m_dispatcher.template add_handling<grid_create_start>(
        "grid-create-start", queue{m_logger},
        [&](grid_create_start &&p_event)
        {
          TNCT_LOG_DEB(m_logger,
                       "subscriber for evt::internal::grid_create_start");

          (*this)(p_event.entries, p_event.num_rows, p_event.num_cols,
                  p_event.wait_for, p_event.max_num_rows);
        });
  }

  void on_stop()
  {
    using evt::internal::grid_create_stop;
    using queue = container::circular_queue<t_logger, grid_create_stop, 10>;
    m_dispatcher.template add_handling<grid_create_stop>(
        "grid-create-stop", queue{m_logger},
        [&](grid_create_stop &&) { m_stop = true; });
  }

  // \brief
  //
  // \tparam std::shared_ptr<dat::grid> grid assembled
  // \tparam std::chrono::seconds how long it took to assemble the grid
  void
  on_solved(std::invocable<std::shared_ptr<dat::grid>, std::chrono::seconds,
                           std::uint64_t> auto p_callback)
  {

    using evt::internal::grid_create_solved;

    using queue = container::circular_queue<t_logger, grid_create_solved, 5>;
    m_dispatcher.template add_handling<grid_create_solved>(
        "grid-create-solved", queue{m_logger},
        [p_callback](grid_create_solved &&p_event)
        { p_callback(p_event.grid, p_event.time, p_event.max_permutations); },
        async::handling_priority::highest);
  }

  // \brief
  //
  // \tparam dat::index num rows,
  // \tparam dat::index num cols,
  void on_unsolved(std::invocable<dat::index, dat::index> auto p_callback)
  {
    using evt::internal::grid_create_unsolved;

    using queue = container::circular_queue<t_logger, grid_create_unsolved, 5>;

    m_dispatcher.template add_handling<grid_create_unsolved>(
        "grid-create-unsolved", queue{m_logger},
        [p_callback](grid_create_unsolved &&p_event)
        { p_callback(p_event.num_rows, p_event.num_cols); });
  }

  std::optional<dat::entries::const_entry_ite>
  is_any_word_too_long(const dat::entries &p_entries, dat::index p_rows,
                       dat::index p_cols) const
  {

    for (dat::entries::const_entry_ite _ite = p_entries.begin();
         _ite != p_entries.end(); ++_ite)
    {
      const dat::index _size{static_cast<dat::index>(_ite->get_word().size())};

      if ((_size > p_rows) && (_size > p_cols))
      {
        return {_ite};
      }
    }
    return std::nullopt;
  }

  /// \brief Tries to create a grid
  ///
  /// Tries to assemble \p p_entries in a grid of \p p_num_rows and \p
  /// p_num_cols.
  ///
  /// If it fails, it will increment \p p_num_rows and \p p_num_cols until \p
  /// p_num_rows is equal to \p p_max_num_rows. Each attempt will take up to \p
  /// p_wait_for time.
  ///
  /// It will publish evt::internal::grid_create_solved or
  /// evt::internal::grid_create_unsolved
  void operator()(const dat::entries &p_entries, dat::index p_num_rows,
                  dat::index p_num_cols, std::chrono::seconds p_wait_for,
                  dat::index p_max_num_rows)
  {

    try
    {

      if (p_wait_for <= std::chrono::seconds::zero())
      {
        if (const auto _result =
                m_dispatcher.template publish<evt::internal::grid_create_error>(
                    dat::error::INVALID_INTERVAL);
            _result != async::result::OK)
        {
          TNCT_LOG_ERR(m_logger, format::fmt("error publishing error ",
                                             dat::error::INVALID_INTERVAL));
        }
        return;
      }

      if (p_max_num_rows <= p_num_rows)
      {
        if (const auto _result =
                m_dispatcher.template publish<evt::internal::grid_create_error>(
                    dat::error::MAX_ROWS_SMALLER_THAN_ROWS);
            _result != async::result::OK)
        {
          TNCT_LOG_ERR(m_logger,
                       format::fmt("error publishing error ",
                                   dat::error::MAX_ROWS_SMALLER_THAN_ROWS));
        }
        return;
      }

      if (std::optional<dat::entries::const_entry_ite> _entry{
              is_any_word_too_long(p_entries, p_num_rows, p_num_cols)};
          _entry.has_value())
      {
        if (const auto _result =
                m_dispatcher.template publish<evt::internal::grid_create_error>(
                    dat::error::WORD_TOO_LONG, _entry.value()->get_word());
            _result != async::result::OK)
        {
          TNCT_LOG_ERR(m_logger, format::fmt("error publishing error ",
                                             dat::error::WORD_TOO_LONG));
        }
        return;
      }

      m_stop = false;

      const auto _max_permutations = number_of_permutations(p_entries);

      crosswords::dat::index _current_num_rows{p_num_rows};
      crosswords::dat::index _current_num_cols{p_num_cols};
      m_dispatcher.clear();

      while ((_current_num_rows <= p_max_num_rows) && (!m_stop))
      {

        TNCT_LOG_DEB(m_logger, format::fmt("Trying grid ", _current_num_rows,
                                           'x', _current_num_cols));

        auto _start = std::chrono::high_resolution_clock::now();

        std::shared_ptr<crosswords::dat::grid> _grid{
            m_assembler.start(p_entries, _current_num_rows, _current_num_cols,
                              p_wait_for, _max_permutations)};

        if (_grid)
        {

          auto _end = std::chrono::high_resolution_clock::now();

          std::chrono::duration<double> _diff = _end - _start;

          TNCT_LOG_DEB(m_logger, format::fmt("grid ", _current_num_rows, 'x',
                                             _current_num_cols, " solved in ",
                                             _diff.count(), " seconds"));

          if (const auto _result{
                  m_dispatcher
                      .template publish<evt::internal::grid_create_solved>(
                          _grid,
                          std::chrono::duration_cast<std::chrono::seconds>(
                              std::chrono::duration<double>(_diff)),
                          _max_permutations)};
              _result != async::result::OK)
          {
            TNCT_LOG_ERR(
                m_logger,
                format::fmt("Error publishing internal::grid_create_solved: ",
                            _result));
          }

          TNCT_LOG_DEB(m_logger, "evt::internal::grid_create_solved published");

          return;
        }

        if (m_stop)
        {
          break;
        }

        TNCT_LOG_DEB(m_logger, format::fmt("Not solved for ", _current_num_rows,
                                           'x', _current_num_cols));

        ++_current_num_rows;
        ++_current_num_cols;

        m_dispatcher.clear();
      }

      TNCT_LOG_DEB(m_logger, "Not solved for all the grid sizes");

      if (const auto _result{
              m_dispatcher
                  .template publish<evt::internal::grid_create_unsolved>(
                      _current_num_rows, _current_num_cols)};
          _result != async::result::OK)
      {
        TNCT_LOG_ERR(
            m_logger,
            format::fmt("Error publishing internal::grid_create_unsolved: ",
                        _result));
      }
      m_dispatcher.clear();
    }
    catch (dat::error &_error)
    {
      if (const auto _result{
              m_dispatcher.template publish<evt::internal::grid_create_error>(
                  _error)};
          _result != async::result::OK)
      {
        TNCT_LOG_ERR(m_logger,
                     format::fmt("could not publish the  error ", _error))
      }
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, format::fmt("error '", _ex.what(), '\''));
    }
    catch (...)
    {
      TNCT_LOG_ERR(m_logger, "unknown error");
    }
  }

  std::uint64_t number_of_permutations(const dat::entries &p_entries) const
  {
    if (p_entries.get_num_entries() > 20)
    {
      return math::factorial<std::uint64_t>(20).value();
    }

    return math::factorial<std::uint64_t>(p_entries.get_num_entries()).value();
  }

private:
  t_logger                                      &m_logger;
  dispatcher                                     m_dispatcher;
  bus::internal::assembler<t_logger, dispatcher> m_assembler;
  std::atomic_bool                               m_stop{false};
};

} // namespace tnct::crosswords::bus

#endif

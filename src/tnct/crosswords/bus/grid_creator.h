#ifndef TNCT_CROSSWORDS_BUS_GRID_CREATOR_H
#define TNCT_CROSSWORDS_BUS_GRID_CREATOR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <atomic>
#include <memory>

#include "tnct/crosswords/bus/internal/assembler.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/evt/grid_create_start.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cpt/logger.h"

namespace tnct::crosswords::bus
{

template <log::cpt::logger t_logger, async::cpt::is_dispatcher t_dispatcher>
struct grid_creator
{
  using events_subscribed =
      std::tuple<evt::grid_create_start, evt::grid_create_stop>;

  // ,
  //              evt::grid_create_unsolved>;

  using events_published =
      std ::tuple<evt::grid_create_solved, evt::grid_create_unsolved>;

  grid_creator(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_assembler(m_dispatcher)
  {
    m_dispatcher.template subscribe<grid_creator, evt::grid_create_start>(
        [&](auto p_event)
        {
          TNCT_LOG_DEB(m_logger, "subscriber for evt::grid_create_start");

          (*this)(p_event.entries, p_event.num_rows, p_event.num_cols,
                  p_event.wait_for, p_event.max_num_rows);
        });

    m_dispatcher.template subscribe<grid_creator, evt::grid_create_stop>(
        [&](auto) { m_stop = true; });

    //    m_dispatcher.template subscribe<evt::grid_create_solved>(
    //        [&](auto) { m_stop = true; });
  }

  ~grid_creator()
  {
    // m_dispatcher.template publish<evt::grid_create_stop>();

    m_stop = true;
  }

private:
  /// \brief Tries to create a grid
  ///
  /// Tries to assemble \p p_entries in a grid of \p p_num_rows and \p
  /// p_num_cols.
  ///
  /// If it fails, it will increment \p p_num_rows and \p p_num_cols until \p
  /// p_num_rows is equal to \p p_max_num_rows. Each attempt will take up to \p
  /// p_wait_for time.
  ///
  /// It will publish evt::grid_create_solved with \p grid attribute \p nullptr
  /// if the grid creation failed, or a valid pointer otherwise

  void operator()(const dat::entries &p_entries, dat::index p_num_rows,
                  dat::index p_num_cols, std::chrono::seconds p_wait_for,
                  dat::index p_max_num_rows)
  {

    try
    {
      m_stop = false;

      TNCT_LOG_DEB(
          m_logger,
          format::fmt("starting grid_creator::operator() with m_stop = ",
                      m_stop));

      crosswords::dat::index _current_num_rows{p_num_rows};
      crosswords::dat::index _current_num_cols{p_num_cols};
      m_dispatcher.clear();

      while ((_current_num_rows <= p_max_num_rows) && (!m_stop))
      {

        TNCT_LOG_DEB(m_logger, format::fmt("Trying grid ", _current_num_rows,
                                           'x', _current_num_cols));

        auto _start = std::chrono::high_resolution_clock::now();

        std::shared_ptr<crosswords::dat::grid> _grid{m_assembler.start(
            p_entries, _current_num_rows, _current_num_cols, p_wait_for)};

        if (_grid)
        {

          auto _end = std::chrono::high_resolution_clock::now();

          std::chrono::duration<double> _diff = _end - _start;

          TNCT_LOG_DEB(m_logger, format::fmt("grid ", _current_num_rows, 'x',
                                             _current_num_cols, " solved in ",
                                             _diff.count(), " seconds"));

          m_dispatcher.template publish<grid_creator, evt::grid_create_solved>(
              _grid, std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::duration<double>(_diff)));

          TNCT_LOG_DEB(m_logger, "evt::grid_create_solved published");

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

      m_dispatcher.template publish<grid_creator, evt::grid_create_unsolved>(
          _current_num_rows, _current_num_cols);
      m_dispatcher.clear();
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

private:
  t_logger                                        &m_logger;
  t_dispatcher                                    &m_dispatcher;
  bus::internal::assembler<t_logger, t_dispatcher> m_assembler;
  std::atomic_bool                                 m_stop{false};
};

} // namespace tnct::crosswords::bus

#endif

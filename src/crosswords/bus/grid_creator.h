#ifndef TENACITAS_SRC_CROSSWORDS_BUS_GRID_CREATOR_H
#define TENACITAS_SRC_CROSSWORDS_BUS_GRID_CREATOR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <atomic>
#include <memory>

#include "tenacitas/src/container/matrix.h"
#include "tenacitas/src/crosswords/asy/dispatcher.h"
#include "tenacitas/src/crosswords/asy/events.h"
#include "tenacitas/src/crosswords/dat/grid.h"
#include "tenacitas/src/crosswords/bus/internal/assembler.h"
#include "tenacitas/src/crosswords/bus/internal/organizer.h"
#include "tenacitas/src/fmt/format_number.h"
#include "tenacitas/src/log/log.h"
#include "tenacitas/src/mat/factorial.h"
#include "tenacitas/src/sta/chrono_convertible.h"

namespace tenacitas::src::crosswords::bus {

struct grid_creator {
  using events_subscribed =
      std::tuple<asy::grid_create_start, asy::grid_create_stop>;

  // ,
  //              asy::grid_create_unsolved>;

  using events_published =
      std ::tuple<asy::grid_create_solved, asy::grid_create_unsolved>;

  grid_creator(asy::dispatcher::ptr p_dispatcher)
      : m_dispatcher(p_dispatcher), m_assembler(m_dispatcher) {
    m_dispatcher->subscribe<grid_creator, asy::grid_create_start>(
        [&](auto p_event) {
#ifdef TENACITAS_LOG
          TNCT_LOG_DEB("subscriber for asy::grid_create_start");
#endif

          (*this)(p_event.entries, p_event.num_rows, p_event.num_cols,
                  p_event.wait_for, p_event.max_num_rows);
        });

    m_dispatcher->subscribe<grid_creator, asy::grid_create_stop>(
        [&](auto) { m_stop = true; });

    //    m_dispatcher->subscribe<asy::grid_create_solved>(
    //        [&](auto) { m_stop = true; });
  }

  ~grid_creator() {
    // m_dispatcher->publish<asy::grid_create_stop>();

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
  /// It will publish asy::grid_create_solved with \p grid attribute \p nullptr
  /// if the grid creation failed, or a valid pointer otherwise

  void operator()(const dat::entries &p_entries, dat::index p_num_rows,
                  dat::index p_num_cols, std::chrono::seconds p_wait_for,
                  dat::index p_max_num_rows) {

    try {
      m_stop = false;

#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("starting grid_creator::operator() with m_stop = ", m_stop);
#endif

      crosswords::dat::index _current_num_rows{p_num_rows};
      crosswords::dat::index _current_num_cols{p_num_cols};
      m_dispatcher->clear();

      while ((_current_num_rows <= p_max_num_rows) && (!m_stop)) {
#ifdef TENACITAS_LOG
        TNCT_LOG_DEB("Trying grid ", _current_num_rows, 'x', _current_num_cols);
#endif

        auto _start = std::chrono::high_resolution_clock::now();

        std::shared_ptr<crosswords::dat::grid> _grid{m_assembler.start(
            p_entries, _current_num_rows, _current_num_cols, p_wait_for)};

        if (_grid) {

          auto _end = std::chrono::high_resolution_clock::now();

          std::chrono::duration<double> _diff = _end - _start;

#ifdef TENACITAS_LOG
          std::stringstream _stream;
          _stream << "grid " << _current_num_rows << 'x' << _current_num_cols
                  << " solved in " << _diff.count() << " seconds";
          std::string _str{_stream.str()};
          TNCT_LOG_DEB(_str);
#endif
          m_dispatcher->publish<grid_creator, asy::grid_create_solved>(
              _grid, std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::duration<double>(_diff)));
#ifdef TENACITAS_LOG
          TNCT_LOG_DEB("asy::grid_create_solved published");
#endif
          return;
        }

        if (m_stop) {
          break;
        }

#ifdef TENACITAS_LOG
        TNCT_LOG_DEB("Not solved for ", _current_num_rows, 'x',
                     _current_num_cols);
#endif
        ++_current_num_rows;
        ++_current_num_cols;

        m_dispatcher->clear();
      }
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("Not solved for all the grid sizes");
#endif
      m_dispatcher->publish<grid_creator, asy::grid_create_unsolved>(
          _current_num_rows, _current_num_cols);
      m_dispatcher->clear();
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("error '", _ex.what(), '\'');
    } catch (...) {
      TNCT_LOG_ERR("unknown error");
    }
  }

private:
  asy::dispatcher::ptr m_dispatcher;
  dat::internal::assembler m_assembler;
  std::atomic_bool m_stop{false};
};

} // namespace tenacitas::src::crosswords::bus

#endif

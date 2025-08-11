#ifndef TENACITAS_SRC_CROSSWORDS_BUS_INTERNAL_ORGANIZER_H
#define TENACITAS_SRC_CROSSWORDS_BUS_INTERNAL_ORGANIZER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tuple>
#include <vector>

#include "tenacitas/src/async/handling_id.h"
#include "tenacitas/src/container/circular_queue.h"
#include "tenacitas/src/crosswords/asy/grid_create_solved.h"
#include "tenacitas/src/crosswords/asy/grid_create_stop.h"
#include "tenacitas/src/crosswords/asy/grid_create_timeout.h"
#include "tenacitas/src/crosswords/asy/grid_create_unsolved.h"
#include "tenacitas/src/crosswords/dat/coordinates.h"
#include "tenacitas/src/crosswords/dat/grid.h"
#include "tenacitas/src/async/traits/is_dispatcher.h"
#include "tenacitas/src/log/traits/logger.h"

namespace tenacitas::src::crosswords::bus::internal {

template <src::log::traits::logger t_logger,
          src::async::traits::dispatcher t_dispatcher>
struct organizer {
  // using events_subscribed =
  //     std::tuple<asy::grid_create_stop, asy::grid_create_solved,
  //                asy::grid_create_unsolved, asy::grid_create_timeout>;

  organizer(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher) {

    m_dispatcher.template add_handling<crosswords::asy::grid_create_stop>(
        m_grid_create_stop, [&](auto) { m_stop = true; },
        container::circular_queue<t_logger, asy::grid_create_stop, 10>{
            m_logger});

    m_dispatcher.template add_handling<crosswords::asy::grid_create_solved>(
        m_grid_create_solved, [&](auto) { m_stop = true; },
        container::circular_queue<t_logger, crosswords::asy::grid_create_solved,
                                  10>{m_logger});

    m_dispatcher.template add_handling<crosswords::asy::grid_create_unsolved>(
        m_grid_create_solved, [&](auto) { m_stop = true; },
        container::circular_queue<t_logger,
                                  crosswords::asy::grid_create_unsolved, 10>{
            m_logger});

    m_dispatcher.template add_handling<crosswords::asy::grid_create_timeout>(
        m_grid_create_solved, [&](auto) { m_stop = true; },
        container::circular_queue<t_logger,
                                  crosswords::asy::grid_create_timeout, 10>{
            m_logger});
  }

  ~organizer() { stop(); }

  bool operator()(std::shared_ptr<dat::grid> p_grid) {
    m_stop = false;
    m_dispatcher.template clear<crosswords::asy::grid_create_stop>(
        m_grid_create_stop);
    m_dispatcher.template clear<crosswords::asy::grid_create_solved>(
        m_grid_create_solved);
    m_dispatcher.template clear<crosswords::asy::grid_create_unsolved>(
        m_grid_create_unsolved);
    m_dispatcher.template clear<crosswords::asy::grid_create_timeout>(
        m_grid_create_timeout);

    if (m_stop) {
      return false;
    }

    if (p_grid->empty()) {
      return false;
    }

    if (!two_first_words_intersect(m_stop, *p_grid)) {
      return false;
    }

    if (m_stop) {
      return false;
    }

    p_grid->reset_positions();

    if (m_stop) {
      return false;
    }

    first_word_positioner _first_word_positioner;

    while (!m_stop && (_first_word_positioner(m_stop, *p_grid))) {
      dat::grid::const_layout_ite _end = p_grid->end();

      dat::grid::const_layout_ite _layout = p_grid->begin();

      dat::grid::layout_ite _to_position = std::next(p_grid->begin());

      while (!m_stop && (_to_position != _end)) {
        while (!m_stop && (_layout->is_positioned()) && (_layout != _end)) {
          if (position(m_stop, *p_grid, _layout, _to_position)) {
            break;
          } else {
            ++_layout;
          }
        }

        if (!m_stop) {
          if (!_to_position->is_positioned()) {
            break;
          }
          _layout = p_grid->begin();
          ++_to_position;
        }
      }

      if (!m_stop) {
        if (p_grid->organized()) {
          m_stop = true;
          return true;
        }
        p_grid->reset_positions();
      }
    }

    return false;
  }

  inline void stop() { m_stop = true; }

private:
  bool all_words_fit(const dat::grid &p_grid) {

    dat::index _row_size{p_grid.get_num_rows()};

    dat::index _col_size{p_grid.get_num_cols()};

    dat::grid::const_layout_ite _end = p_grid.end();

    for (dat::grid::const_layout_ite _layout = p_grid.begin(); _layout != _end;
         ++_layout) {
      dat::index _word_size{_layout->get_word().size()};
      if ((_word_size > _row_size) && (_word_size > _col_size)) {
        // TNCT_LOG_ERR("word '", _layout->get_word(), "' does not fit in grid
        // [",
        //              _row_size, ',', _col_size, ']');
        return false;
      }
    }
    return true;
  }

  dat::index longest_word(const dat::grid &p_grid) {

    dat::index _size{0};

    dat::grid::const_layout_ite _end{p_grid.end()};

    for (dat::grid::const_layout_ite _layout = p_grid.begin(); _layout != _end;
         ++_layout) {
      auto _word_size{_layout->get_word().size()};
      if (_word_size > _size) {
        _size = _word_size;
      }
    }
    return _size;
  }

  struct first_word_positioner {
    bool operator()(bool &p_stop, dat::grid &p_grid) {
      if (m_occupied.get_num_rows() == 0) {
        //      const auto _longest{dat::get_size(p_grid.begin()->get_word())};
        //      auto _num_rows{p_grid.get_num_rows()};
        //      auto _num_cols{p_grid.get_num_cols()};
        //      if (_num_rows > _longest) {
        //        _num_rows -= _longest;
        //      }
        //      if (_num_cols > _longest) {
        //        _num_cols -= _longest;
        //      }
        m_occupied = dat::occupied(p_grid.get_num_rows(), p_grid.get_num_cols(),
                                   dat::word::max_char);
      }
      if (p_stop) {
        return false;
      }

      if (!m_all_horizontal_tried && horizontal(p_stop, p_grid)) {
        return true;
      }

      if (!m_vertical) {
        m_occupied.reset();
        m_vertical = true;
      }

      if (p_stop) {
        return false;
      }

      return vertical(p_stop, p_grid);
    }

  private:
    bool horizontal(bool &p_stop, dat::grid &p_grid) {

      const dat::index _num_rows{m_occupied.get_num_rows()};

      const dat::index _num_cols{m_occupied.get_num_cols()};

      auto _layout = p_grid.begin();

      const auto _word_size{_layout->get_word().size()};

      bool _set{false};

      for (dat::index _row = 0; !p_stop && (_row < _num_rows) && !_set;
           ++_row) {
        for (dat::index _col = 0; !p_stop && (_col < _num_cols) && !_set;
             ++_col) {
          if ((_col + _word_size) > _num_cols) {
            break;
          }

          if (m_occupied(_row, _col) == dat::word::max_char) {
            p_grid.set(_layout, _row, _col, dat::orientation::hori);
            m_occupied(_row, _col) = '#';
            _set = true;
          }
        }
      }

      if (!_set) {
        m_all_horizontal_tried = true;
      }
      return _set;
    }

    bool vertical(bool &p_stop, dat::grid &p_grid) {

      const dat::index _num_rows{m_occupied.get_num_rows()};

      const dat::index _num_cols{m_occupied.get_num_cols()};

      auto _layout = p_grid.begin();

      const auto _word_size{_layout->get_word().size()};

      bool _set{false};

      for (dat::index _row = 0; !p_stop && (_row < _num_rows) && !_set;
           ++_row) {
        for (dat::index _col = 0; !p_stop && (_col < _num_cols) && !_set;
             ++_col) {
          if ((_row + _word_size) > _num_rows) {
            break;
          }

          if (m_occupied(_row, _col) == dat::word::max_char) {
            p_grid.set(_layout, _row, _col, dat::orientation::vert);
            m_occupied(_row, _col) = '#';
            _set = true;
          }
        }
      }

      return _set;
    }

  private:
    bool m_all_horizontal_tried{false};
    bool m_vertical{false};
    dat::occupied m_occupied;
  };

  dat::coordinates find_intersections(bool &p_stop,
                                      const dat::word &p_positioned,
                                      const dat::word &p_to_position) {

    dat::coordinates _coordinates;

    dat::index _size1{p_positioned.size()};

    dat::index _size2{p_to_position.size()};

    for (dat::index _i2 = 0; !p_stop && (_i2 < _size2); ++_i2) {
      for (dat::index _i1 = 0; !p_stop && (_i1 < _size1); ++_i1) {
        if (p_positioned[_i1] == p_to_position[_i2]) {
          _coordinates.push_back({_i2, _i1});
        }
      }
    }

    //    for (decltype(_size2) _i2 = 0; !p_stop && (_i2 < _size2); ++_i2) {
    //      for (decltype(_size1) _i1 = 0; !p_stop && (_i1 < _size1); ++_i1) {
    //        if (static_cast<uint8_t>(p_positioned[_i1]) == 0xC3) {
    //          if (static_cast<uint8_t>(p_to_position[_i2]) == 0xC3) {
    //            if (static_cast<uint8_t>(p_positioned[_i1 + 1]) ==
    //                static_cast<uint8_t>(p_to_position[_i2 + 1])) {
    //              _coordinates.push_back({_i2, _i1});
    //              _i2 += 1;
    //            }
    //          }
    //          _i1 += 1;
    //        } else {
    //          if ((static_cast<uint8_t>(p_to_position[_i2]) != 0xC3) &&
    //              (p_positioned[_i1] == p_to_position[_i2])) {
    //            _coordinates.push_back({_i2, _i1});
    //          }
    //        }
    //      }
    //    }
    return _coordinates;
  }

  bool position_horizontally(dat::grid &p_grid,
                             const dat::coordinate &p_intersect,
                             dat::grid::const_layout_ite p_positioned,
                             dat::grid::layout_ite p_to_position) {

    auto _to_position_idx{p_intersect.first};

    auto _positioned_idx{p_intersect.second};

    dat::index _row_to_position(p_positioned->get_row() + _positioned_idx);

    dat::index _col_to_position(p_positioned->get_col() - _to_position_idx);

    if (_col_to_position < 0) {
      return false;
    }

    const dat::index _word_size{p_to_position->get_word().size()};

    const dat::index _col_end = _col_to_position + _word_size;

    if (_col_end > p_grid.get_num_cols()) {
      return false;
    }

    dat::index _count{0};

    const dat::word &_word{p_to_position->get_word()};

    for (dat::index _i = 0; _i < _word.size(); ++_i) {
      const auto _maybe{
          p_grid.is_occupied(_row_to_position, _col_to_position + _count++)};
      if (_maybe && (_maybe.value() != _word[_i])) {
        return false;
      }
    }

    p_grid.set(p_to_position, _row_to_position, _col_to_position,
               dat::orientation::hori);

    return true;
  }

  bool position_vertically(dat::grid &p_grid,
                           const dat::coordinate &p_intersect,
                           dat::grid::const_layout_ite p_positioned,
                           dat::grid::layout_ite p_to_position) {

    const dat::index _to_position_idx{p_intersect.first};
    const dat::index _positioned_idx{p_intersect.second};

    const dat::index _col_to_position(p_positioned->get_col() +
                                      _positioned_idx);

    const dat::index _row_to_position(p_positioned->get_row() -
                                      _to_position_idx);

    if (_row_to_position < 0) {
      return false;
    }

    const dat::index _word_size{p_to_position->get_word().size()};
    const dat::index _row_end = _row_to_position + _word_size;

    if (_row_end > p_grid.get_num_rows()) {
      return false;
    }

    dat::index _count{0};

    const dat::word &_word{p_to_position->get_word()};

    for (dat::index _i = 0; _i < _word.size(); ++_i) {
      const auto _maybe{
          p_grid.is_occupied(_row_to_position + _count++, _col_to_position)};

      if (_maybe && (_maybe.value() != _word[_i])) {
        return false;
      }
    }

    p_grid.set(p_to_position, _row_to_position, _col_to_position,
               dat::orientation::vert);

    return true;
  }

  bool position(bool &p_stop, dat::grid &p_grid,
                dat::grid::const_layout_ite p_positioned,
                dat::grid::layout_ite p_to_position) {

    auto _intersects{find_intersections(p_stop, p_positioned->get_word(),
                                        p_to_position->get_word())};

    auto _orientation{p_positioned->get_orientation()};

    if (_orientation == dat::orientation::hori) {
      for (auto _coord : _intersects) {
        if (position_vertically(p_grid, _coord, p_positioned, p_to_position)) {
          return true;
        }
      }
    } else {
      for (auto _coord : _intersects) {
        if (position_horizontally(p_grid, _coord, p_positioned,
                                  p_to_position)) {
          return true;
        }
      }
    }
    return false;
  }

  bool position(bool &p_stop, dat::grid &p_grid,
                dat::grid::layout_ite p_to_position) {

    dat::grid::layout_ite _positioned = std::next(p_grid.begin());

    while (_positioned != p_to_position) {
      if (!position(p_stop, p_grid, p_to_position, _positioned)) {
        break;
      }
      ++_positioned;
    }

    return true;
  }

  bool two_first_words_intersect(bool &p_stop, const dat::grid &p_grid) {

    dat::grid::const_layout_ite _layout = p_grid.begin();
    dat::grid::const_layout_ite _to_position = std::next(p_grid.begin());
    if (_to_position == p_grid.end()) {
      return false;
    }

    if ((_to_position == std::next(_layout)) &&
        find_intersections(p_stop, _layout->get_word(),
                           _to_position->get_word())
            .empty()) {
      return false;
    }
    return true;
  }

private:
  t_logger &m_logger;

  t_dispatcher &m_dispatcher;

  bool m_stop{false};

  async::handling_id m_grid_create_stop{"create_stop"};

  async::handling_id m_grid_create_solved{"create_solved"};

  async::handling_id m_grid_create_unsolved{"create_unsolved"};

  async::handling_id m_grid_create_timeout{"create_timeout"};
};

} // namespace tenacitas::src::crosswords::bus::internal

#endif

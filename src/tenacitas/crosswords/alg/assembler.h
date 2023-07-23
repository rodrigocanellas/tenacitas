#ifndef TENACITAS_LIB_CROSSWORDS_ALG_ORGANIZER_H
#define TENACITAS_LIB_CROSSWORDS_ALG_ORGANIZER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include <tenacitas/container/typ/matrix.h>
#include <tenacitas/crosswords/alg/dispatcher.h>
#include <tenacitas/crosswords/evt/events.h>
#include <tenacitas/crosswords/typ/grid.h>
#include <tenacitas/log/alg/logger.h>
#include <tenacitas/math/alg/factorial.h>
#include <tenacitas/number/alg/format.h>

namespace tenacitas::lib::crosswords::bus {

namespace internal {

bool all_words_fit(const typ::grid &p_grid) {
  using namespace typ;
  index _row_size{p_grid.get_num_rows()};
  index _col_size{p_grid.get_num_cols()};

  typ::grid::const_layout_ite _end = p_grid.end();
  for (grid::const_layout_ite _layout = p_grid.begin(); _layout != _end;
       ++_layout) {
    index _word_size{typ::get_size(_layout->get_word())};
    if ((_word_size > _row_size) && (_word_size > _col_size)) {
      TNCT_LOG_ERR("word '", _layout->get_word(), "' does not fit in grid [",
                   _row_size, ',', _col_size, ']');
      return false;
    }
  }
  return true;
}

typ::index longest_word(const typ::grid &p_grid) {
  using namespace typ;
  index _size{0};

  grid::const_layout_ite _end{p_grid.end()};

  for (grid::const_layout_ite _layout = p_grid.begin(); _layout != _end;
       ++_layout) {
    auto _word_size{typ::get_size(_layout->get_word())};
    if (_word_size > _size) {
      _size = _word_size;
    }
  }
  return _size;
}

struct first_word_positioner {
  bool operator()(bool &p_stop, typ::grid &p_grid) {
    if (m_occupied.get_num_rows() == 0) {
      //      const auto _longest{typ::get_size(p_grid.begin()->get_word())};
      //      auto _num_rows{p_grid.get_num_rows()};
      //      auto _num_cols{p_grid.get_num_cols()};
      //      if (_num_rows > _longest) {
      //        _num_rows -= _longest;
      //      }
      //      if (_num_cols > _longest) {
      //        _num_cols -= _longest;
      //      }
      m_occupied = typ::occupied(p_grid.get_num_rows(), p_grid.get_num_cols(),
                                 typ::max_char);
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
  bool horizontal(bool &p_stop, typ::grid &p_grid) {
    using namespace typ;

    const index _num_rows{m_occupied.get_num_rows()};
    const index _num_cols{m_occupied.get_num_rows()};
    auto _layout = p_grid.begin();
    const auto _word_size{typ::get_size(_layout->get_word())};

    bool _set{false};

    for (index _row = 0; !p_stop && (_row < _num_rows) && !_set; ++_row) {
      for (index _col = 0; !p_stop && (_col < _num_cols) && !_set; ++_col) {

        if ((_col + _word_size) > _num_cols) {
          break;
        }
        if (m_occupied(_row, _col) == typ::max_char) {
          p_grid.set(_layout, _row, _col, typ::orientation::hori);
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

  bool vertical(bool &p_stop, typ::grid &p_grid) {
    using namespace typ;

    const index _num_rows{m_occupied.get_num_rows()};
    const index _num_cols{m_occupied.get_num_rows()};
    auto _layout = p_grid.begin();
    const auto _word_size{typ::get_size(_layout->get_word())};

    bool _set{false};

    for (index _row = 0; !p_stop && (_row < _num_rows) && !_set; ++_row) {
      for (index _col = 0; !p_stop && (_col < _num_cols) && !_set; ++_col) {
        if ((_row + _word_size) > _num_rows) {
          break;
        }

        if (m_occupied(_row, _col) == typ::max_char) {
          p_grid.set(_layout, _row, _col, typ::orientation::vert);
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
  typ::occupied m_occupied;
};

typ::coordinates find_intersections(bool &p_stop, const typ::word &p_positioned,
                                    const typ::word &p_to_position) {
  using namespace typ;

  coordinates _coordinates;

  index _w1_size{typ::get_size(p_positioned)};
  index _w2_size{typ::get_size(p_to_position)};

  for (index _i2 = 0; !p_stop && (_i2 < _w2_size); ++_i2) {
    for (index _i1 = 0; !p_stop && (_i1 < _w1_size); ++_i1) {
      if (p_positioned[_i2] == p_to_position[_i1]) {
        _coordinates.push_back({_i1, _i2});
      }
    }
  }

  return _coordinates;
}

bool position_horizontally(typ::grid &p_grid,
                           const typ::coordinate &p_intersect,
                           typ::grid::const_layout_ite p_positioned,
                           typ::grid::layout_ite p_to_position) {
  using namespace typ;

  auto _to_position_idx{p_intersect.first};
  auto _positioned_idx{p_intersect.second};

  index _row_to_position(p_positioned->get_row() + _positioned_idx);

  index _col_to_position(p_positioned->get_col() - _to_position_idx);

  if (_col_to_position < 0) {
    return false;
  }

  const index _word_size{typ::get_size(p_to_position->get_word())};
  const index _col_end = _col_to_position + _word_size;
  if (_col_end > p_grid.get_num_cols()) {
    return false;
  }

  index _count{0};
  const word &_word{p_to_position->get_word()};
  for (auto _c : _word) {
    const auto _maybe{
        p_grid.is_occupied(_row_to_position, _col_to_position + _count++)};
    if (_maybe && (_maybe.value() != _c)) {
      return false;
    }
  }
  p_grid.set(p_to_position, _row_to_position, _col_to_position,
             orientation::hori);
  return true;
}

bool position_vertically(typ::grid &p_grid, const typ::coordinate &p_intersect,
                         typ::grid::const_layout_ite p_positioned,
                         typ::grid::layout_ite p_to_position) {
  using namespace typ;

  const index _to_position_idx{p_intersect.first};
  const index _positioned_idx{p_intersect.second};

  const index _col_to_position(p_positioned->get_col() + _positioned_idx);

  const index _row_to_position(p_positioned->get_row() - _to_position_idx);

  if (_row_to_position < 0) {
    return false;
  }

  const index _word_size{typ::get_size(p_to_position->get_word())};
  const index _row_end = _row_to_position + _word_size;

  if (_row_end > p_grid.get_num_rows()) {
    return false;
  }
  index _count{0};
  const word &_word{p_to_position->get_word()};
  for (auto _c : _word) {
    const auto _maybe{
        p_grid.is_occupied(_row_to_position + _count++, _col_to_position)};
    if (_maybe && (_maybe.value() != _c)) {
      return false;
    }
  }
  p_grid.set(p_to_position, _row_to_position, _col_to_position,
             orientation::vert);
  return true;
}

bool position(bool &p_stop, typ::grid &p_grid,
              typ::grid::const_layout_ite p_positioned,
              typ::grid::layout_ite p_to_position) {
  using namespace typ;

  auto _intersects{internal::find_intersections(
      p_stop, p_positioned->get_word(), p_to_position->get_word())};
  auto _orientation{p_positioned->get_orientation()};

  if (_orientation == orientation::hori) {
    for (auto _coord : _intersects) {
      if (internal::position_vertically(p_grid, _coord, p_positioned,
                                        p_to_position)) {
        return true;
      }
    }
  } else {
    for (auto _coord : _intersects) {
      if (internal::position_horizontally(p_grid, _coord, p_positioned,
                                          p_to_position)) {
        return true;
      }
    }
  }
  return false;
}

bool position(bool &p_stop, typ::grid &p_grid,
              typ::grid::layout_ite p_to_position) {
  using namespace typ;

  grid::layout_ite _positioned = std::next(p_grid.begin());

  while (_positioned != p_to_position) {
    if (!internal::position(p_stop, p_grid, p_to_position, _positioned)) {
      break;
    }
    ++_positioned;
  }

  return true;
}

bool two_first_words_intersect(bool &p_stop, const typ::grid &p_grid) {
  using namespace typ;
  grid::const_layout_ite _layout = p_grid.begin();
  grid::const_layout_ite _to_position = std::next(p_grid.begin());
  if (_to_position == p_grid.end()) {
    return false;
  }

  if ((_to_position == std::next(_layout)) &&
      find_intersections(p_stop, _layout->get_word(), _to_position->get_word())
          .empty()) {
    return false;
  }
  return true;
}

struct organizer {
  ~organizer() = default;

  bool operator()(std::shared_ptr<typ::grid> p_grid) {
    using namespace typ;
    if (m_stop) {
      TNCT_LOG_TRA("organizer ", this, ": stopped");
      return false;
    }

    if (p_grid->empty()) {
      TNCT_LOG_TRA("organizer ", this, ": no words to position");
      return false;
    }

    if (!two_first_words_intersect(m_stop, *p_grid)) {
      TNCT_LOG_TRA("organizer ", this,
                   ": no organization possible because no word intersects '",
                   p_grid->begin()->get_word(), '\'');
      return false;
    }
    if (m_stop) {
      TNCT_LOG_TRA("organizer ", this, ": stopped");
      return false;
    }
    p_grid->reset_positions();
    if (m_stop) {
      TNCT_LOG_TRA("organizer ", this, ": stopped");
      return false;
    }
    internal::first_word_positioner _first_word_positioner;

    while (!m_stop && (_first_word_positioner(m_stop, *p_grid))) {

      grid::const_layout_ite _end = p_grid->end();
      grid::const_layout_ite _layout = p_grid->begin();
      grid::layout_ite _to_position = std::next(p_grid->begin());
      while (!m_stop && (_to_position != _end)) {

        while (!m_stop && (_layout->is_positioned()) && (_layout != _end)) {
          if (internal::position(m_stop, *p_grid, _layout, _to_position)) {
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
          TNCT_LOG_TRA("organizer ", this,
                       ": SUCCESS! setting to stop: ", *p_grid);
          m_stop = true;
          return true;
        }
        p_grid->reset_positions();
      }
    }

    TNCT_LOG_TRA("organizer ", this, ": could not organize");
    return false;
  }

  inline void stop() { m_stop = true; }

private:
  bool m_stop{false};
};

bool compare_entries(const typ::entry &p_e1, const typ::entry &p_e2) {
  if (p_e1.get_word().size() == p_e2.get_word().size()) {
    return p_e1.get_word() < p_e2.get_word();
  }
  return p_e1.get_word().size() < p_e2.get_word().size();
}

void sort_entries(typ::entries &p_entries) {
  std::sort(p_entries.begin(), p_entries.end(), compare_entries);
}

} // namespace internal

/// \brief Tries to assemble a grid
struct assembler {
  assembler(alg::dispatcher::ptr p_dispatcher) : m_dispatcher(p_dispatcher) {}
  assembler() = delete;
  assembler(const assembler &) = delete;
  assembler(assembler &&) = delete;
  assembler &operator=(const assembler &) = delete;
  assembler &operator=(assembler &&) = delete;
  ~assembler() = default;

  /// \brief Tries to assemble a tenacitas::crosswords::typ::grid
  ///
  /// \tparam uint64_t type of integer used to fix the maximum number of
  /// attempts
  ///
  /// \param p_entries entries used to assemble the grid
  ///
  /// \param p_num_rows number of rows of the grid
  ///
  /// \param p_num_cols number of columns of the grid
  ///
  /// \param p_num_threads number of threads used to assemble the grid
  ///
  /// \param p_max_tries maximum number of attempts to assemble the grid
  ///
  /// \details The problem grows exponencially with the number of words. For
  /// instance, with 10 words, there 10! (factorial of 10), i.e. 3628800,
  /// possible combination, and, maybe, with one of them a grid can be
  /// assembled.
  /// Trying to reduce the amount of time, it is possible to define the number
  /// of threads used, where in each one of them a different combination will be
  /// used.
  /// However, as the number of combinations can be huge, it is possible to
  /// define the maximum number of attempts for assembling, before giving it up.
  std::shared_ptr<typ::grid>
  start(const typ::entries &p_entries, typ::index p_num_rows,
        typ::index p_num_cols, uint8_t p_num_threads = 20,
        uint64_t p_max_tries = std::numeric_limits<uint64_t>::max()) {

    m_num_threads = p_num_threads;

    m_entries = p_entries;

    configure_dispatcher();

    typ::entries _entries{m_entries};
    internal::sort_entries(_entries);

    typ::permutation _permutation;
    for (typ::entries::const_entry_ite _entry = _entries.begin();
         _entry != _entries.end(); ++_entry) {
      _permutation.push_back(_entry);
    }

    auto _maybe{
        lib::math::alg::factorial<uint64_t>(_entries.get_num_entries())};

    uint64_t _max_permutations{0};

    if (!_maybe) {
      return nullptr;
    }

    _max_permutations = _maybe.value();

    if (p_max_tries <= _max_permutations) {
      _max_permutations = p_max_tries;
    }

    TNCT_LOG_TRA("_max_permutation_number = ", _max_permutations);
    m_permutation_counter = 0;

    while (true) {
      if (m_stop) {
        TNCT_LOG_TRA("stop requested");
        break;
      }

      if (m_permutation_counter == _max_permutations) {
        TNCT_LOG_TRA(m_permutation_counter, " permutations generated");
        break;
      }

      if (m_solved) {
        TNCT_LOG_TRA("stopping");
        break;
      }

      typ::permutation _aux{_permutation.size()};
      std::reverse_copy(_permutation.begin(), _permutation.end(), _aux.begin());
      TNCT_LOG_TRA(lib::number::alg::format(++m_permutation_counter), ": ",
                   _aux);
      m_dispatcher->publish<evt::new_attempt>(m_permutation_counter);

      auto _grid{std::make_shared<typ::grid>(_aux, p_num_rows, p_num_cols,
                                             m_permutation_counter)};
      if (!m_dispatcher->publish<evt::new_grid_to_organize>(_grid)) {
        TNCT_LOG_ERR("error publishing event evt::new_grid_to_organize");
      }

      std::next_permutation(_permutation.begin(), _permutation.end(),
                            [](typ::entries::const_entry_ite p_e1,
                               typ::entries::const_entry_ite p_e2) -> bool {
                              return internal::compare_entries(*p_e1, *p_e2);
                            });
    }
    TNCT_LOG_TRA("left permutation loop, with ", m_permutation_counter,
                 " permutations were generated, and m_stop = ", m_stop);

    if (m_stop) {
      TNCT_LOG_TRA("stop requested");
      m_dispatcher->stop();
      return {};
    }

    if (!m_solved && (m_num_organizations_finished != m_permutation_counter)) {
      TNCT_LOG_TRA(
          "m_solved = ", m_solved,
          " and m_num_organizations_finished = ", m_num_organizations_finished);
      std::unique_lock<std::mutex> _lock(m_mutex_num_organizations_finished);
      m_cond_num_organizations_finished.wait(_lock, [this]() -> bool {
        TNCT_LOG_TRA("entering lock");
        if (m_num_organizations_finished == m_permutation_counter) {
          TNCT_LOG_TRA("lock released because all organizers have finished");
          return true;
        }
        TNCT_LOG_TRA("lock not release because m_num_organizations_finished = ",
                     m_num_organizations_finished, ", but ",
                     m_permutation_counter, " are expected");
        return false;
      });
    }

    m_dispatcher->stop();

    if (m_solved) {
      TNCT_LOG_TRA(
          "one organizer organized the grid before all permutations were "
          "tried: ",
          *m_solved);
      return m_solved;
    }

    if (m_num_organizations_finished == m_permutation_counter) {
      TNCT_LOG_TRA(
          "m_num_organizations_finished == ", m_num_organizations_finished,
          " and no organizer organized the grid");
    }
    return {};
  }

  /// \brief Stops assembling the grid
  inline void stop() { m_stop = true; }

  /// \brief Retrieves how many attempts were made
  uint64_t get_num_attempts() const { return m_permutation_counter; }

private:
  using organizers = std::vector<bus::internal::organizer>;

private:
  void configure_dispatcher() {

    TNCT_LOG_TRA("configuring publishing for event evt::stop_organizing");
    m_organizers = organizers(m_num_threads);
    for (internal::organizer &_organizer : m_organizers) {
      m_dispatcher->subscribe<evt::stop_organizing>(
          [&_organizer](auto) -> void { _organizer.stop(); });
    }

    TNCT_LOG_TRA("configuring publishing for event evt::new_grid_to_organize");
    auto _new_grid_to_organize_publishing =
        m_dispatcher->add_queue<evt::new_grid_to_organize>();

    for (decltype(m_num_threads) _i = 0; _i < m_num_threads; ++_i) {
      m_dispatcher->subscribe<evt::new_grid_to_organize>(
          _new_grid_to_organize_publishing, [this, _i](auto p_event) -> void {
            if (p_event.grid == nullptr) {
              TNCT_LOG_ERR(
                  "could not call organizer because p_event.grid is null");
              return;
            }

            internal::organizer &_organizer{m_organizers[_i]};
            TNCT_LOG_TRA("calling organizer ", &_organizer);
            if (_organizer(p_event.grid)) {
              TNCT_LOG_TRA("organizer ", &_organizer,
                           " organized grid for permutation ",
                           p_event.grid->get_permutation_number(),
                           "; notifying the grid is organized and notifying "
                           "other organizers to stop");
              if (!m_dispatcher->publish<evt::assembly_finished>(
                      p_event.grid)) {
                TNCT_LOG_ERR("error publishing event evt::assembly_finished");
              }
              if (!m_dispatcher->publish<evt::stop_organizing>()) {
                TNCT_LOG_ERR("error publishing event evt::stop_organizing");
              }
            } else {
              TNCT_LOG_TRA("organizer ", &_organizer,
                           " did not organize permutation ",
                           p_event.grid->get_permutation_number());
              m_dispatcher->publish<evt::assembly_finished>(nullptr);
            }
          });
    }

    TNCT_LOG_TRA("configuring publishing for event evt::assembly_finished");
    m_dispatcher->subscribe<evt::assembly_finished>(
        [this](auto p_event) -> void {
          ++m_num_organizations_finished;
          TNCT_LOG_TRA(m_num_organizations_finished, " organizations finished");
          if (p_event.grid) {
            TNCT_LOG_TRA("and the grid is organized");
            if (!m_solved) {
              TNCT_LOG_TRA("and the final grid is null");
              m_solved = p_event.grid;
            } else {
              TNCT_LOG_TRA("but the final grid was already set");
            }
          }
          std::lock_guard<std::mutex> _lock{m_mutex_num_organizations_finished};
          m_cond_num_organizations_finished.notify_all();
        });
  }

private:
  uint8_t m_num_threads = 20;
  alg::dispatcher::ptr m_dispatcher;
  typ::entries m_entries;
  bool m_stop{false};
  uint64_t m_permutation_counter{0};
  organizers m_organizers;
  std::shared_ptr<typ::grid> m_solved;
  std::mutex m_mutex_organizers;
  std::condition_variable m_cond_stop;
  std::mutex m_mutex_stop;
  size_t m_num_organizations_finished{0};
  std::mutex m_mutex_num_organizations_finished;
  std::condition_variable m_cond_num_organizations_finished;
};

} // namespace tenacitas::lib::crosswords::bus

#endif // ORGANIZER_H

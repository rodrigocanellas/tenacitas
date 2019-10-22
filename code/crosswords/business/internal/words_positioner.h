#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONER_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/positions_occupied.h>
#include <crosswords/business/internal/validate_position.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief words_positioner_t tries to position a set of words
///
/// \tparam t_log provides log funcionality:
/// static void debug(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void info(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void warn(const std::string & p_file, int p_line, const t_params&...
/// p_params)
/// static void error(const std::string & p_file, int p_line, const
/// t_params&... p_params)
/// static void fatal(const std::string & p_file, int p_line, const
/// t_params&... p_params)
///
template<typename t_log>
struct words_positioner_t
{
  typedef t_log log;

  typedef entities::words words;
  typedef entities::word word;
  typedef entities::lexeme lexeme;
  typedef entities::coordinate coordinate;
  typedef entities::coordinates coordinates;
  typedef entities::coordinate::x x;
  typedef entities::coordinate::y y;

  typedef positions_occupied_t<log> positions_occupied;
  typedef validate_position_t<log> validate_position;

  words_positioner_t(x m_x_limit, y m_y_limit)
    : m_x_limit(m_x_limit)
    , m_y_limit(m_y_limit) {}

  //  void reset()
  //  {
  //    m_last_first_position_horizontal.reset();
  //    m_last_first_position_vertical.reset();

  //    if ( (m_position_from_first) && (m_position_first_horizontal)) {
  //      m_position_from_first = true;
  //      m_position_first_horizontal = false;
  //    }
  //    else if ( (m_position_from_first) && (!m_position_first_horizontal)) {
  //      m_position_from_first = false;
  //      m_position_first_horizontal = true;
  //    }
  //    else {
  //      m_position_from_first = false;
  //      m_position_first_horizontal = false;
  //    }
  //  }

  bool operator()(words::iterator p_begin, words::iterator p_end)
  {
    words::iterator _ite = p_begin;

    //    uint32_t _counter = 0;

    uint8_t _shifter = 0;

    crosswords_log_info(log, "####### ", print_words(p_begin, p_end));

    while (true) {
      m_position_from_first = true;
      m_position_first_horizontal = true;
      bool _all_set_positioned = true;
      while (true) {
        if ( (!m_position_from_first) && (!m_position_first_horizontal)) {
          _all_set_positioned = false;
          break;
        }
        bool _hope_to_position = true;
        while (true) {
          if (_ite == p_end) {
            break;
          }

          crosswords_log_debug(log, "trying to position ", _ite->get_lexeme());
          if (_ite->positioned()) {
            crosswords_log_debug(log, _ite->get_lexeme(), " already positioned");
            ++_ite;
          } else {
            positioning _positioning = position(p_begin, _ite);
            if (_positioning == positioning::ok) {
              crosswords_log_debug(log, _ite->get_lexeme(), " positioned!");
              print_positioned(p_begin, p_end);
              ++_ite;
            } else {
              if (_positioning == positioning::some_not_positioned) {
                m_positions_occupied.clear();
                unposition(p_begin, _ite);
//                m_last_first_position_horizontal.reset();
//                m_last_first_position_vertical.reset();
                _ite = p_begin;
              } else { // positioning::first_not_positioned
                crosswords_log_debug(log, _ite->get_lexeme(), " not positioned");
                _hope_to_position = false;
                break;
              }
            }
          }
        }
        if (_hope_to_position) {
          break;
        }

        if ( (m_position_from_first) && (m_position_first_horizontal)) {
          crosswords_log_debug(log, "(true, true) to (true, false)");
          m_position_from_first = true;
          m_position_first_horizontal = false;
        }
        else if ( (m_position_from_first) && (!m_position_first_horizontal)) {
          crosswords_log_debug(log, "(true, false) to (false, true)");
          m_position_from_first = false;
          m_position_first_horizontal = true;
        }
        else {
          crosswords_log_debug(log, "(false, true) to (false, false)");
          m_position_from_first = false;
          m_position_first_horizontal = false;
        }
        m_positions_occupied.clear();
        unposition(p_begin, p_end);
        m_last_first_position_horizontal.reset();
        m_last_first_position_vertical.reset();
        _ite = p_begin;

      }
      if (_all_set_positioned) {
        break;
      }

      crosswords_log_warn(log, "unable to position ", print_words(p_begin, p_end));
      if (_shifter == static_cast<uint8_t>(std::distance(p_begin, p_end))) {
        crosswords_log_warn(log, "all the possible changes (" ,
                            static_cast<uint16_t>(_shifter), ") were made: ");
        return false;
      }
      std::iter_swap(p_begin, std::next(p_begin, ++_shifter));
      crosswords_log_debug(log, "new order: ", print_words(p_begin, p_end));
      m_positions_occupied.clear();
      unposition(p_begin, p_end);
      m_last_first_position_horizontal.reset();
      m_last_first_position_vertical.reset();
      _ite = p_begin;
    }
    return true;
  }

private:
  enum class positioning : char
  {
    ok = 'o',
    first_not_positioned = 'f',
    some_not_positioned = 'n'
  };

private:
  positioning position(words::iterator p_first_positioned,
                       words::iterator p_to_position)
  {

    if (no_word_positioned(p_first_positioned, p_to_position)) {
      if (!position_first(p_to_position)) {
        return positioning::first_not_positioned;
      }
      return positioning::ok;
    }

    positioning _positioning = positioning::ok;
    if (m_position_from_first) {
      _positioning = position_from_first(p_first_positioned, p_to_position);

      if (_positioning == positioning::ok) {
        return positioning::ok;
      }

      m_positions_occupied.clear();
      unposition(p_first_positioned, p_to_position);

      return _positioning;

    }

    _positioning = position_from_last(p_first_positioned, p_to_position);

    if (_positioning == positioning::ok) {
      return positioning::ok;
    }
    return _positioning;
  }

  positioning position_from_last(words::iterator p_first_positioned,
                                 words::iterator p_to_position)
  {

    words::const_iterator _last_positioned = p_to_position;
    --_last_positioned;
    while (true) {
      if (_last_positioned == p_first_positioned) {
        bool _is_positioned = position(p_first_positioned,
                                       _last_positioned,
                                       p_to_position);

        if (_is_positioned) {
          m_positions_occupied.add(*p_to_position);
          return positioning::ok;
        }

        crosswords_log_debug(log,
                             "unable to position ",
                             p_to_position->get_lexeme(),
                             " against ",
                             _last_positioned->get_lexeme());

        crosswords_log_debug(log,
                             "trying to reposition first word");

        return positioning::some_not_positioned;
      }

      bool _is_positioned = position(p_first_positioned,
                                     _last_positioned,
                                     p_to_position);

      if (_is_positioned) {
        m_positions_occupied.add(*p_to_position);
        return positioning::ok;
      }

      crosswords_log_debug(log,
                           "unable to position ",
                           p_to_position->get_lexeme(),
                           " against ",
                           _last_positioned->get_lexeme());

      --_last_positioned;
    }
  }

  positioning position_from_first(words::iterator p_first_positioned,
                                  words::iterator p_to_position)
  {
    if (no_word_positioned(p_first_positioned, p_to_position)) {
      if (!position_first(p_to_position)) {
        return positioning::first_not_positioned;
      }
      return positioning::ok;
    }

    words::const_iterator _last_positioned = p_first_positioned;
    while (true) {
      if (_last_positioned == p_to_position) {
        break;
      }
      bool _is_positioned = position(p_first_positioned,
                                     _last_positioned,
                                     p_to_position);

      if (_is_positioned) {
        m_positions_occupied.add(*p_to_position);
        return positioning::ok;
      }

      crosswords_log_debug(log,
                           "unable to position ",
                           p_to_position->get_lexeme(),
                           " against ",
                           _last_positioned->get_lexeme());

      ++_last_positioned;
    }
    return positioning::some_not_positioned;
  }

  bool no_word_positioned(words::const_iterator p_begin,
                          words::const_iterator p_end) const
  {
    for (words::const_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      if (_ite->positioned()) {
        return false;
      }
    }
    return true;
  }

  bool position(words::const_iterator p_first_positioned,
                words::const_iterator p_last_positioned,
                words::iterator p_to_position)
  {
    //        crosswords_log_debug(log,
    //                             "trying to position ",
    //                             p_to_position->get_lexeme(),
    //                             " against ",
    //                             *p_last_positioned);

    validate_position _validate_position;

    const lexeme& _positioned = p_last_positioned->get_lexeme();
    lexeme::size_type _positioned_size = _positioned.size();

    const lexeme& _to_position = p_to_position->get_lexeme();
    lexeme::size_type _to_position_size = _to_position.size();

    for (lexeme::size_type _i_positioned = 0;
         _i_positioned < _positioned_size;
         ++_i_positioned) {
      for (lexeme::size_type _i_to_position = 0;
           _i_to_position < _to_position_size;
           ++_i_to_position) {
        if (_to_position[_i_to_position] ==
            _positioned[_i_positioned]) {
          //                    crosswords_log_debug(log,
          //                                         "intesect at ",
          //                                         _i_positioned,
          //                                         " in positioned
          //                                         ", _positioned,
          //                                         " and in ",
          //                                         _i_to_position,
          //                                         " in ",
          //                                         _to_position);
          if (p_last_positioned->get_direction() ==
              word::direction::vertical) {

            p_to_position->position(
                  p_last_positioned->get_coordinates()[_i_positioned]
                  .get_x() -
                  x(_i_to_position),
                  p_last_positioned->get_coordinates()[_i_positioned]
                  .get_y(),

                  word::direction::horizontal,
                  word::orientation::forward);
          } else {
            p_to_position->position(
                  p_last_positioned->get_coordinates()[_i_positioned]
                  .get_x(),
                  p_last_positioned->get_coordinates()[_i_positioned]
                  .get_y() -
                  y(_i_to_position),

                  word::direction::vertical,
                  word::orientation::forward);
          }
          //                    crosswords_log_debug(
          //                      log, "it seems ", *p_to_position, "
          //                      was positioned");
          if (_validate_position(p_first_positioned,
                                 p_to_position,
                                 _i_to_position,
                                 m_positions_occupied,
                                 m_x_limit,
                                 m_y_limit)) {
            return true;
          }
          //                    crosswords_log_debug(log, "invalid
          //                    position");
          p_to_position->unposition();
        }
      }
    }
    return false;
  }

  bool position_first_horizontal(words::iterator p_to_position,
                                 lexeme::size_type p_lexeme_size)
  {
    if (x(p_lexeme_size) > m_x_limit) {
      return false;
    }
    if (m_last_first_position_horizontal == coordinate(x(-1), y(-1))) {
      m_last_first_position_horizontal = coordinate(x(0), y(0));
    } else {
      if ((m_last_first_position_horizontal.get_x() + x(1) +
           x(p_lexeme_size)) >= m_x_limit) {
        if (m_last_first_position_horizontal.get_y() ==
            (m_y_limit - y(1))) {
          return false;
        }
        m_last_first_position_horizontal = coordinate(
              x(0), m_last_first_position_horizontal.get_y() + y(1));
      } else {
        m_last_first_position_horizontal =
            coordinate(m_last_first_position_horizontal.get_x() + x(1),
                       m_last_first_position_horizontal.get_y());
      }
    }

    p_to_position->position(m_last_first_position_horizontal.get_x(),
                            m_last_first_position_horizontal.get_y(),
                            word::direction::horizontal,
                            word::orientation::forward);
    return true;
  }

  bool position_first_verical(words::iterator p_to_position,
                              lexeme::size_type p_lexeme_size)
  {
    if (y(p_lexeme_size) > m_y_limit) {
      return false;
    }
    if (m_last_first_position_vertical == coordinate(x(-1), y(-1))) {
      m_last_first_position_vertical = coordinate(x(0), y(0));
    } else {
      if ((m_last_first_position_vertical.get_y() + y(1) +
           y(p_lexeme_size)) >= m_y_limit) {
        if (m_last_first_position_vertical.get_x() ==
            (m_x_limit - x(1))) {
          return false;
        }
        m_last_first_position_vertical = coordinate(
              m_last_first_position_vertical.get_x() + x(1), y(0));
      } else {
        m_last_first_position_vertical =
            coordinate(m_last_first_position_vertical.get_x(),
                       m_last_first_position_vertical.get_y() + y(1));
      }
    }
    p_to_position->position(m_last_first_position_vertical.get_x(),
                            m_last_first_position_vertical.get_y(),
                            word::direction::vertical,
                            word::orientation::forward);

    return true;
  }

  bool position_first(words::iterator p_to_position)
  {

    lexeme::size_type _lexeme_size = p_to_position->get_lexeme().size();

    if (m_position_first_horizontal) {

      if (position_first_horizontal(p_to_position, _lexeme_size)) {
        m_positions_occupied.add(*p_to_position);
        return true;
      }
      return false;
    }

    if (position_first_verical(p_to_position, _lexeme_size)) {
      m_positions_occupied.add(*p_to_position);
      return true;
    }
    return false;
  }

  void unposition(words::iterator p_begin, words::iterator p_end)
  {
    for (words::iterator _ite = p_begin; _ite != p_end; ++_ite) {
      _ite->unposition();
    }
  }

  void
  print_positioned(words::const_iterator p_begin,
                   words::const_iterator p_end)
  {
    using namespace std;

    typedef std::vector<std::string> matrix;

    string::size_type _x_size = m_x_limit.get_value<string::size_type>();
    string::size_type _y_size = m_y_limit.get_value<string::size_type>();

    matrix _m(_x_size, string(_y_size, ' '));

    for (words::const_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      if (_ite->positioned()) {
        const coordinates& _coords = _ite->get_coordinates();
        const lexeme& _lexeme = _ite->get_lexeme();
        for (lexeme::size_type _i = 0; _i < _lexeme.size(); ++_i) {
          matrix::size_type _x =
              _coords[_i].get_x().get_value<string::size_type>();
          string::size_type _y =
              _coords[_i].get_y().get_value<std::string::size_type>();
          char _c = _lexeme[_i];
          _m[_x][_y] = _c;
        }
      }
    }

    cerr << "    ";
    for (matrix::size_type _x = 0; _x < _x_size; ++_x) {
      cerr << setw(2) << setfill('0') << _x << " ";
    }
    std::cerr << std::endl;
    for (std::string::size_type _y = 0; _y < _y_size; ++_y) {
      cerr << setw(2) << setfill('0') << _y << " ";
      for (matrix::size_type _x = 0; _x < _x_size; ++_x) {
        cerr << "  " << _m[_x][_y];
      }
      cerr << std::endl;
    }
  }

private:
  positions_occupied m_positions_occupied;
  coordinate m_last_first_position_horizontal = { x(-1), y(-1) };
  coordinate m_last_first_position_vertical = { x(-1), y(-1) };
  x m_x_limit;
  y m_y_limit;
  bool m_position_from_first = {true};
  bool m_position_first_horizontal = {true};
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORD_POSITIONER_H

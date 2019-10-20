#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORD_POSITIONER_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORD_POSITIONER_H

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

/// \brief positioner002_t position \p words
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
struct word_positioner_t
{
    typedef t_log log;

    typedef entities::words words;
    typedef entities::word word;
    typedef entities::lexeme lexeme;
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;

    typedef positions_occupied_t<log> positions_occupied;
    typedef validate_position_t<log> validate_position;

    void reset() { m_last_position_when_first.clear(); }

    std::pair<bool, bool> operator()(words::iterator p_first_positioned,
                                     words::iterator p_to_position,
                                     x p_x_limit,
                                     y p_y_limit)
    {
        while (true) {
            if (no_word_positioned(p_first_positioned, p_to_position)) {
                if (!position_first(p_to_position, p_x_limit, p_y_limit)) {
                    return { false, false };
                }
                return { true, true };
            }

            words::const_iterator _last_positioned = p_to_position;
            --_last_positioned;
            while (true) {
                if (_last_positioned == p_first_positioned) {
                    bool _is_positioned = position(p_first_positioned,
                                                   _last_positioned,
                                                   p_to_position,
                                                   p_x_limit,
                                                   p_y_limit);

                    if (_is_positioned) {
                        m_positions_occupied.add(*p_to_position);
                        return { true, true };
                    }

                    crosswords_log_debug(log,
                                         "unable to position ",
                                         p_to_position->get_lexeme(),
                                         " against ",
                                         _last_positioned->get_lexeme());

                    m_positions_occupied.clear();
                    unposition(p_first_positioned, p_to_position);

                    crosswords_log_debug(log,
                                         "trying to reposition first word");

                    return { false, true };
                }

                bool _is_positioned = position(p_first_positioned,
                                               _last_positioned,
                                               p_to_position,
                                               p_x_limit,
                                               p_y_limit);

                if (_is_positioned) {
                    m_positions_occupied.add(*p_to_position);
                    return { true, true };
                }

                crosswords_log_debug(log,
                                     "unable to position ",
                                     p_to_position->get_lexeme(),
                                     " against ",
                                     _last_positioned->get_lexeme());

                --_last_positioned;
            }
        }
    }

  private:
    enum class first_position : char
    {
        UP_LEFT = 'L',
        UP_RIGHT = 'R',
        DOWN_RIGHT = 'r',
        DOWN_LEFT = 'l',
        CENTER = 'c'
    };
    typedef std::map<lexeme, first_position> last_position_when_first;

  private:
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
                  words::iterator p_to_position,
                  x p_x_limit,
                  y p_y_limit)
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
                                           p_x_limit,
                                           p_y_limit)) {
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

    bool position_first(words::iterator p_to_position, x p_x_limit, y p_y_limit)
    {
        const lexeme& _lexeme = p_to_position->get_lexeme();
        lexeme::size_type _lexeme_size = _lexeme.size();

        typename last_position_when_first::iterator _ite =
          m_last_position_when_first.find(_lexeme);

        first_position _first_position;

        if (_ite != m_last_position_when_first.end()) {
            switch (_ite->second) {
                case first_position::UP_LEFT:
                    _first_position = first_position::UP_RIGHT;
                    break;
                case first_position::UP_RIGHT:
                    _first_position = first_position::DOWN_RIGHT;
                    break;
                case first_position::DOWN_RIGHT:
                    _first_position = first_position::DOWN_LEFT;
                    break;
                case first_position::DOWN_LEFT:
                    _first_position = first_position::CENTER;
                    break;
                default:
                    // all positions were tried
                    return false;
            }
            m_last_position_when_first[_lexeme] = _first_position;
        } else {
            //            m_last_position_when_first[_lexeme] =
            //            first_position::UP_LEFT;
            m_last_position_when_first.emplace(_lexeme,
                                               first_position::UP_LEFT);
            _first_position = first_position::UP_LEFT;
        }

        x _x;
        y _y;
        if (p_y_limit > y(p_x_limit)) {
            switch (_first_position) {
                case first_position::UP_LEFT:
                    _x = x(0);
                    _y = y(0);
                    break;
                case first_position::UP_RIGHT:
                    _x = p_x_limit - x(1) - x(_lexeme_size);
                    _y = y(0);
                    break;
                case first_position::DOWN_RIGHT:
                    _x = p_x_limit - x(1) - x(_lexeme_size);
                    _y = p_y_limit - y(1);
                    break;
                case first_position::DOWN_LEFT:
                    _x = x(0);
                    _y = p_y_limit - y(1);
                    break;
                default:
                    _x = (p_x_limit - x(_lexeme_size)) / x(2);
                    _y = p_y_limit / y(2);
            }

            p_to_position->position(
              _x, _y, word::direction::horizontal, word::orientation::forward);
        } else {
            switch (_first_position) {
                case first_position::UP_LEFT:
                    _x = x(0);
                    _y = y(0);
                    break;
                case first_position::UP_RIGHT:
                    _x = p_x_limit - x(1);
                    _y = y(0);
                    break;
                case first_position::DOWN_RIGHT:
                    _x = p_x_limit - x(1);
                    _y = p_y_limit - y(_lexeme_size);
                    break;
                case first_position::DOWN_LEFT:
                    _x = x(0);
                    _y = p_y_limit - y(_lexeme_size);
                    break;
                default:
                    _y = (p_y_limit - y(_lexeme_size)) / y(2);
                    _x = p_x_limit / x(2);
            }

            p_to_position->position(
              _x, _y, word::direction::vertical, word::orientation::forward);
        }
        m_positions_occupied.add(*p_to_position);
        return true;
    }

    void unposition(words::iterator p_begin, words::iterator p_end)
    {
        for (words::iterator _ite = p_begin; _ite != p_end; ++_ite) {
            _ite->unposition();
        }
    }

  private:
    positions_occupied m_positions_occupied;
    last_position_when_first m_last_position_when_first;
    uint8_t m_position_first_word = { 0 };
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORD_POSITIONER_H

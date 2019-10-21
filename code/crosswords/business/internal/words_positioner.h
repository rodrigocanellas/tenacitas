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
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;

    typedef positions_occupied_t<log> positions_occupied;
    typedef validate_position_t<log> validate_position;

    void reset()
    {
        m_last_first_position_horizontal.reset();
        m_last_first_position_vertical.reset();
    }

    bool operator()(words::iterator p_first,
                    words::iterator p_end,
                    x p_x_limit,
                    y p_y_limit)
    {
        words::iterator _ite = p_first;
        uint32_t _counter = 0;

        crosswords_log_info(
          log, "############ ", _counter++, " - ", print_words(p_first, p_end));

        while (true) {

            if (_ite == p_end) {
                break;
            }

            crosswords_log_debug(
              log, "trying to position ", _ite->get_lexeme());
            if (_ite->positioned()) {
                crosswords_log_debug(
                  log, _ite->get_lexeme(), " was previously positioned");
                ++_ite;
            } else {
                positioning _positioning =
                  position(p_first, _ite, p_x_limit, p_y_limit);
                if (_positioning == positioning::ok) {
                    crosswords_log_debug(
                      log, _ite->get_lexeme(), " was positioned");
                    print_positioned(p_first, p_end, p_x_limit, p_y_limit);
                    ++_ite;
                } else {
                    if (_positioning == positioning::other_not_positioned) {
                        _ite = p_first;
                    } else {
                        crosswords_log_debug(
                          log, _ite->get_lexeme(), " was not positioned");
                        return false;
                    }
                }
            }
        }
        return true;
    }

  private:
    enum class positioning : char
    {
        ok = 'o',
        first_not_positioned = 'f',
        other_not_positioned = 'n'
    };

  private:
    positioning position(words::iterator p_first_positioned,
                         words::iterator p_to_position,
                         x p_x_limit,
                         y p_y_limit)
    {
        while (true) {
            if (no_word_positioned(p_first_positioned, p_to_position)) {
                if (!position_first(p_to_position, p_x_limit, p_y_limit)) {
                    return positioning::first_not_positioned;
                }
                return positioning::ok;
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
                        return positioning::ok;
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

                    return positioning::other_not_positioned;
                }

                bool _is_positioned = position(p_first_positioned,
                                               _last_positioned,
                                               p_to_position,
                                               p_x_limit,
                                               p_y_limit);

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

    bool position_first_horizontal(words::iterator p_to_position,
                                   x p_x_limit,
                                   y p_y_limit,
                                   lexeme::size_type p_lexeme_size)
    {
        if (x(p_lexeme_size) > p_x_limit) {
            return false;
        }
        if (m_last_first_position_horizontal == coordinate(x(-1), y(-1))) {
            m_last_first_position_horizontal = coordinate(x(0), y(0));
        } else {
            if ((m_last_first_position_horizontal.get_x() + x(1) +
                 x(p_lexeme_size)) >= p_x_limit) {
                if (m_last_first_position_horizontal.get_y() ==
                    (p_y_limit - y(1))) {
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
                                x p_x_limit,
                                y p_y_limit,
                                lexeme::size_type p_lexeme_size)
    {
        if (y(p_lexeme_size) > p_y_limit) {
            return false;
        }
        if (m_last_first_position_vertical == coordinate(x(-1), y(-1))) {
            m_last_first_position_vertical = coordinate(x(0), y(0));
        } else {
            if ((m_last_first_position_vertical.get_y() + y(1) +
                 y(p_lexeme_size)) >= p_y_limit) {
                if (m_last_first_position_vertical.get_x() ==
                    (p_x_limit - x(1))) {
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

    bool position_first(words::iterator p_to_position, x p_x_limit, y p_y_limit)
    {

        lexeme::size_type _lexeme_size = p_to_position->get_lexeme().size();

        if (position_first_horizontal(
              p_to_position, p_x_limit, p_y_limit, _lexeme_size)) {
            m_positions_occupied.add(*p_to_position);
            return true;
        }

        if (position_first_verical(
              p_to_position, p_x_limit, p_y_limit, _lexeme_size)) {
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

  private:
    positions_occupied m_positions_occupied;
    coordinate m_last_first_position_horizontal = { x(-1), y(-1) };
    coordinate m_last_first_position_vertical = { x(-1), y(-1) };
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORD_POSITIONER_H

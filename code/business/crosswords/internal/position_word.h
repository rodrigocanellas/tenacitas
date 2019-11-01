#ifndef TENACITAS_BUSINESS_CROSSWORDS_POSITION_WORD_H
#define TENACITAS_BUSINESS_CROSSWORDS_POSITION_WORD_H

#include <business/crosswords/internal/log.h>
#include <business/crosswords/internal/positions_occupied.h>
#include <business/crosswords/internal/validate_position.h>
#include <entities/crosswords/coordinate.h>
#include <entities/crosswords/lexeme.h>
#include <entities/crosswords/word.h>
#include <entities/crosswords/words.h>

namespace tenacitas {
namespace business {
namespace crosswords {

/// \brief validate_position validates a position
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
struct position_word_t
{

    typedef t_log log;

    typedef entities::crosswords::words words;
    typedef entities::crosswords::word word;
    typedef entities::crosswords::lexeme lexeme;
    typedef entities::crosswords::coordinate::x x;
    typedef entities::crosswords::coordinate::y y;

    typedef positions_occupied_t<log> positions_occupied;
    typedef validate_position_t<log> validate_position;

    bool operator()(const words& p_words,
                    words::iterator p_to_position,
                    x p_x_limit,
                    y p_y_limit,
                    positions_occupied& p_positions_occupied)
    {
        if (no_word_positioned(p_words)) {
            position_first(p_to_position,
                           p_x_limit,
                           p_y_limit,
                           p_positions_occupied);
            return true;
        }
        words::iterator _positioned = p_to_position;
        --_positioned;
        words::const_iterator _positioned_begin =
                p_words.begin();
        while (true) {
            if (_positioned == _positioned_begin) {
                bool _is_positioned =
                        position(p_words,
                                 p_to_position,
                                 _positioned,
                                 p_positions_occupied,
                                 p_x_limit,
                                 p_y_limit);


                if (_is_positioned) {

                    p_positions_occupied.add(*p_to_position);
                    break;
                }

                return false;
            }

            bool _is_positioned =
                    position(p_words, p_to_position,
                             _positioned,
                             p_positions_occupied,
                             p_x_limit,
                             p_y_limit);

            if (_is_positioned) {
                p_positions_occupied.add(*p_to_position);

                break;
            }

            --_positioned;
        }
        return true;
    }

private:

    bool no_word_positioned(const words & p_words)const {
        for (const word& _word : p_words) {
            if (_word.positioned()) {
                return false;
            }
        }
        return true;
    }

    bool
    position(const words & p_words,
             words::iterator p_to_position,
             words::const_iterator p_positioned,
             positions_occupied& p_positions_occupied,
             x p_x_limit,
             y p_y_limit)
    {
        crosswords_log_debug(log,
                             "trying to position ",
                             p_to_position->get_lexeme(),
                             " against ",
                             *p_positioned);

        validate_position _validate_position;

        const lexeme& _positioned = p_positioned->get_lexeme();
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
                    crosswords_log_debug(log,
                                         "intesect at ",
                                         _i_positioned,
                                         " in positioned ",
                                         _positioned,
                                         " and in ",
                                         _i_to_position,
                                         " in ",
                                         _to_position);
                    if (p_positioned->get_direction() ==
                            word::direction::vertical) {

                        p_to_position->position(
                                    p_positioned->get_coordinates()[_i_positioned]
                                    .get_x() -
                                    x(_i_to_position),
                                    p_positioned->get_coordinates()[_i_positioned]
                                    .get_y(),

                                    word::direction::horizontal,
                                    word::orientation::forward);
                    } else {
                        p_to_position->position(
                                    p_positioned->get_coordinates()[_i_positioned]
                                    .get_x(),
                                    p_positioned->get_coordinates()[_i_positioned]
                                    .get_y() -
                                    y(_i_to_position),

                                    word::direction::vertical,
                                    word::orientation::forward);
                    }
                    crosswords_log_debug(
                                log, "it seems ", *p_to_position, " was positioned");
                    if (_validate_position(p_words,
                                          p_to_position,
                                          _i_to_position,
                                          p_positions_occupied,
                                          p_x_limit,
                                          p_y_limit)) {
                        return true;
                    }
                    crosswords_log_debug(log, "invalid position");
                    p_to_position->unposition();
                }
            }
        }
        return false;

    }

    void position_first(words::iterator p_to_position,
                        x p_x_limit,
                        y p_y_limit,
                        positions_occupied& p_positions_occupied)
    {

        if (p_y_limit > y(p_x_limit)) {
            p_to_position->position(x(0),
                                    y(0),
                                    word::direction::horizontal,
                                    word::orientation::forward);
        } else {
            p_to_position->position(x(0),
                                    y(0),
                                    word::direction::vertical,
                                    word::orientation::forward);
        }

        //        m_positioned.push_back(p_ptr_word_to_position);
        p_positions_occupied.add(*p_to_position);
    }
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITION_WORD_H

#ifndef TENACITAS_CROSSWORDS_BUSINESS_VALIDATE_POSITION_H
#define TENACITAS_CROSSWORDS_BUSINESS_VALIDATE_POSITION_H

#include <set>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/positions_occupied.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/words.h>
#include <crosswords/entities/coordinate.h>

namespace tenacitas {
namespace crosswords {
namespace business {

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
struct validate_position {
    typedef entities::words::iterator word_pointer;
    typedef entities::word word;
    typedef entities::lexeme lexeme;
    typedef entities::words::const_iterator word_const_pointer;
    typedef entities::coordinate coordinate;
    typedef entities::coordinates coordinates;
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;
    typedef t_log log;
    typedef std::set<lexeme::size_type> intersections;
    typedef positions_occupied_t<t_log> positions_occupied;

private:
    bool check_boundaries_over_under_flow(
            word_const_pointer p_to_position
            , x p_x_limit
            , y p_y_limit) const
    {
        // check boundaries overflow and underflow
        if (p_to_position->get_x0() < x(0)) {
            crosswords_log_debug(
                        log, "underflow in x position for ", *p_to_position);
            return false;
        }

        if (p_to_position->get_y0() < y(0)) {
            crosswords_log_debug(
                        log, "underflow in y position for ", *p_to_position);
            return false;
        }

        if ((p_to_position->get_direction() == word::direction::horizontal) &&
                (p_to_position->get_xn() >= p_x_limit)) {
            crosswords_log_debug(
                        log, "overflow in x position for ", *p_to_position);
            return false;
        }

        if ((p_to_position->get_direction() == word::direction::vertical) &&
                (p_to_position->get_yn() >= p_y_limit)) {
            crosswords_log_debug(
                        log, "overflow in y position for ", *p_to_position);
            return false;
        }
        return true;
    }

    bool valid_extremes_horizontal(
            word_const_pointer p_word,
            positions_occupied & p_positions_occupied,
            x p_x_limit,
            const intersections& /*p_intersections*/) const
    {

        std::pair<bool, char> _occupied({ false, ' ' });
        //        intersections::const_iterator _end = p_intersections.end();

        if (p_word->get_x0() != x(0)) {
            _occupied = p_positions_occupied.find(p_word->get_x0() - x(1), p_word->get_y0());

            if (_occupied.first) {
                crosswords_log_debug(log,
                                     "(",
                                     p_word->get_x0() - x(1),
                                     ",",
                                     p_word->get_y0(),
                                     ") has '",
                                     _occupied.second,
                                     "'");
                return false;
            }
        }

        if (p_word->get_xn() < (p_x_limit - x(1))) {
            _occupied = p_positions_occupied.find(p_word->get_xn() + x(1), p_word->get_y0());
            if (_occupied.first) {
                crosswords_log_debug(log,
                                     "(",
                                     p_word->get_xn() + x(1),
                                     ",",
                                     p_word->get_y0(),
                                     ") has '",
                                     _occupied.second,
                                     "'");
                return false;
            }
        }

        //        if (p_intersections.find(0) == _end) {

        //            if (p_word->get_x0() != x(0)) {
        //                if (p_word->get_y0() != y(0)) {
        //                    _occupied = p_positions_occupied.find(p_word->get_x0() - x(1),
        //                                            p_word->get_y0() - y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_x0() - x(1),
        //                                             ",",
        //                                             p_word->get_y0() - y(1),
        //                                             ") has '",
        //                                             _occupied.second,
        //                                             "'");
        //                        return false;
        //                    }
        //                }

        //                if (p_word->get_y0() != (m_y_limit - y(1))) {
        //                    _occupied = p_positions_occupied.find(p_word->get_x0() - x(1),
        //                                            p_word->get_y0() + y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_x0() - x(1),
        //                                             ",",
        //                                             p_word->get_y0() + y(1),
        //                                             ") has '",
        //                                             _occupied.second,
        //                                             "'");
        //                        return false;
        //                    }
        //                }
        //            }
        //        }

        //        if (p_intersections.find(p_word->get_size() - 1) == _end) {
        //            if (p_word->get_xn() < (m_x_limit - x(1))) {
        //                if (p_word->get_y0() != y(0)) {
        //                    _occupied = p_positions_occupied.find(p_word->get_xn() + x(1),
        //                                            p_word->get_y0() - y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_xn() + x(1),
        //                                             ",",
        //                                             p_word->get_y0() - y(1),
        //                                             ") has '",
        //                                             _occupied.second);
        //                        return false;
        //                    }
        //                }

        //                if (p_word->get_y0() != (m_y_limit - y(1))) {
        //                    _occupied = p_positions_occupied.find(p_word->get_xn() + x(1),
        //                                            p_word->get_y0() + y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_xn() + x(1),
        //                                             ",",
        //                                             p_word->get_y0() + y(1),
        //                                             ") has '",
        //                                             _occupied.second,
        //                                             "'");
        //                        return false;
        //                    }
        //                }
        //            }
        //        }

        return true;
    }

    bool valid_extremes_vertical(word_const_pointer p_word,
                                 positions_occupied & p_positions_occupied,
                                 y p_y_limit,
                                 const intersections& /*p_intersections*/) const
    {
        std::pair<bool, char> _occupied({ false, ' ' });
        if (p_word->get_y0() != y(0)) {
            _occupied = p_positions_occupied.find(p_word->get_x0(), p_word->get_y0() - y(1));

            if (_occupied.first) {
                crosswords_log_debug(log,
                                     "(",
                                     p_word->get_x0(),
                                     ",",
                                     p_word->get_y0() - y(1),
                                     ") has '",
                                     _occupied.second,
                                     "'");
                return false;
            }
        }

        if (p_word->get_yn() != (p_y_limit - y(1))) {
            _occupied = p_positions_occupied.find(p_word->get_x0(), p_word->get_yn() + y(1));
            if (_occupied.first) {
                crosswords_log_debug(log,
                                     "(",
                                     p_word->get_x0(),
                                     ",",
                                     p_word->get_yn() + y(1),
                                     ") has '",
                                     _occupied.second,
                                     "'");
                return false;
            }
        }
        //        intersections::const_iterator _end = p_intersections.end();
        //        if (p_intersections.find(0) == _end) {
        //            if (p_word->get_y0() != y(0)) {

        //                if (p_word->get_x0() != x(0)) {
        //                    _occupied = p_positions_occupied.find(p_word->get_x0() - x(1),
        //                                            p_word->get_y0() - y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_x0() - x(1),
        //                                             ",",
        //                                             p_word->get_y0() - y(1),
        //                                             ") has '",
        //                                             _occupied.second,
        //                                             "'");
        //                        return false;
        //                    }
        //                }

        //                if (p_word->get_x0() != (m_x_limit - x(1))) {
        //                    _occupied = p_positions_occupied.find(p_word->get_x0() + x(1),
        //                                            p_word->get_y0() - y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_x0() + x(1),
        //                                             ",",
        //                                             p_word->get_y0() - y(1),
        //                                             ") has '",
        //                                             _occupied.second,
        //                                             "'");
        //                        return false;
        //                    }
        //                }
        //            }
        //        }

        //        if (p_intersections.find(p_word->get_size() - 1) == _end) {
        //            if (p_word->get_yn() != (m_y_limit - y(1))) {
        //                _occupied =
        //                  p_positions_occupied.find(p_word->get_x0(), p_word->get_yn() +
        //                  y(1));
        //                if (_occupied.first) {
        //                    crosswords_log_debug(log,
        //                                         "(",
        //                                         p_word->get_x0(),
        //                                         ",",
        //                                         p_word->get_yn() + y(1),
        //                                         ") has '",
        //                                         _occupied.second,
        //                                         "'");
        //                    return false;
        //                }

        //                if (p_word->get_x0() != x(0)) {
        //                    _occupied = p_positions_occupied.find(p_word->get_x0() - x(1),
        //                                            p_word->get_yn() + y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_x0() - x(1),
        //                                             ",",
        //                                             p_word->get_yn() + y(1),
        //                                             ") has '",
        //                                             _occupied.second);
        //                        return false;
        //                    }
        //                }

        //                if (p_word->get_x0() != (m_x_limit - x(1))) {
        //                    _occupied = p_positions_occupied.find(p_word->get_x0() + x(1),
        //                                            p_word->get_yn() + y(1));
        //                    if (_occupied.first) {
        //                        crosswords_log_debug(log,
        //                                             "(",
        //                                             p_word->get_x0() + x(1),
        //                                             ",",
        //                                             p_word->get_yn() + y(1),
        //                                             ") has '",
        //                                             _occupied.second,
        //                                             ")");
        //                        return false;
        //                    }
        //                }
        //            }
        //        }

        return true;
    }

    bool valid_extremes(word_const_pointer p_word,
                        positions_occupied & p_positions_occupied,
                        x p_x_limit,
                        y p_y_limit,
                        const intersections& p_intersections) const
    {
        // horizontal
        if ((p_word->get_direction() == word::direction::horizontal) &&
                (!valid_extremes_horizontal(p_word, p_positions_occupied, p_x_limit, p_intersections))) {
            return false;
        }

        if ((p_word->get_direction() == word::direction::vertical) &&
                (!valid_extremes_vertical(p_word, p_positions_occupied, p_x_limit, p_intersections))) {
            return false;
        }

        return true;
    }

    bool left_and_right_are_free(word_const_pointer p_word,
                                 positions_occupied & p_positions_occupied,
                                 x p_x_limit,
                                 const intersections& p_intersections) const
    {

        if (p_word->get_direction() != word::direction::vertical) {
            return true;
        }

        const coordinates& _coords = p_word->get_coordinates();
        lexeme::size_type _size = p_word->get_size();
        std::pair<bool, char> _occupied({ false, ' ' });

        intersections::const_iterator _end = p_intersections.end();
        for (lexeme::size_type _i = 0; _i < _size; ++_i) {
            if (p_intersections.find(_i) != _end) {
                continue;
            }

            if (p_word->get_x0() != x(0)) {
                _occupied =
                        p_positions_occupied.find(_coords[_i].get_x() - x(1), _coords[_i].get_y());
                if (_occupied.first) {
                    crosswords_log_debug(log,
                                         "(",
                                         _coords[_i].get_x() - x(1),
                                         ",",
                                         _coords[_i].get_y(),
                                         ") has ",
                                         _occupied.second);
                    return false;
                }
            }

            if (p_word->get_x0() != (p_x_limit - x(1))) {
                _occupied =
                        p_positions_occupied.find(_coords[_i].get_x() + x(1), _coords[_i].get_y());
                if (_occupied.first) {
                    crosswords_log_debug(log,
                                         "(",
                                         _coords[_i].get_x() + x(1),
                                         ",",
                                         _coords[_i].get_y(),
                                         ") has ",
                                         _occupied.second);
                    return false;
                }
            }
        }
        return true;
    }

    bool above_and_below_are_free(word_const_pointer p_word,
                                  positions_occupied & p_positions_occupied,
                                  y p_y_limit,
                                  const intersections& p_intersections) const
    {
        if (p_word->get_direction() != word::direction::horizontal) {
            return true;
        }

        const coordinates& _coords = p_word->get_coordinates();
        lexeme::size_type _size = p_word->get_size();
        std::pair<bool, char> _occupied({ false, ' ' });

        intersections::const_iterator _end = p_intersections.end();

        for (lexeme::size_type _i = 0; _i < _size; ++_i) {
            if (p_intersections.find(_i) != _end) {
                continue;
            }

            if (p_word->get_y0() != y(0)) {
                _occupied =
                        p_positions_occupied.find(_coords[_i].get_x(), _coords[_i].get_y() - y(1));
                if (_occupied.first) {
                    crosswords_log_debug(log,
                                         "(",
                                         _coords[_i].get_x(),
                                         ",",
                                         _coords[_i].get_y() - y(1),
                                         ") has ",
                                         _occupied.second);
                    return false;
                }
            }

            if (p_word->get_y0() != (p_y_limit - y(1))) {
                _occupied =
                        p_positions_occupied.find(_coords[_i].get_x(), _coords[_i].get_y() + y(1));
                if (_occupied.first) {
                    crosswords_log_debug(log,
                                         "(",
                                         _coords[_i].get_x(),
                                         ",",
                                         _coords[_i].get_y() + y(1),
                                         ") has ",
                                         _occupied.second);
                    return false;
                }
            }
        }
        return true;
    }

    bool valid_position(word_const_pointer p_to_position,
                        positions_occupied & p_positions_occupied,
                        lexeme::size_type p_intersection_idx) const
    {

        if (!check_boundaries_over_under_flow(p_to_position)) {
            return false;
        }

//        print();

        intersections _intersections;
        _intersections.emplace(p_intersection_idx);

        std::pair<bool, char> _occupied({ false, ' ' });
        lexeme::size_type _size = p_to_position->get_size();
        const coordinates& _coordinates = p_to_position->get_coordinates();
        const lexeme& _lexeme = p_to_position->get_lexeme();
        for (lexeme::size_type _i = 0; _i < _size; ++_i) {
            if (_i == p_intersection_idx) {
                continue;
            }
            _occupied =
              p_positions_occupied.find(_coordinates[_i].get_x(), _coordinates[_i].get_y());
            if (_occupied.first) {
                if (_occupied.second != _lexeme[_i]) {
                    crosswords_log_debug(log,
                                         _coordinates[_i],
                                         " has '",
                                         _occupied.second,
                                         "', BUT '",
                                         _lexeme[_i],
                                         "' was expected");
                    return false;
                }
                word_const_pointer _intersected =
                  find(_coordinates[_i]);

                if (_intersected == m_positioned.end()) {
                    throw std::runtime_error("ERROR at line " +
                                             std::to_string(__LINE__));
                }

                if ((*_intersected)->get_direction() ==
                    p_to_position->get_direction()) {
                    crosswords_log_debug(
                      log,
                      *p_to_position,
                      " and ",
                      *(*_intersected),
                      " intersect, but have the same direction");
                    return false;
                }

                _intersections.emplace(_i);
            }
        }

        if (!valid_extremes(p_to_position, _intersections)) {
            return false;
        }

        if (!above_and_below_are_free(p_to_position, _intersections)) {
            return false;
        }

        return (left_and_right_are_free(p_to_position, _intersections));

//        return true;
    }

    word_const_pointer find(const coordinate& p_coord) const
    {
        word_const_pointer _end = m_positioned.end();
        for (word_const_pointer _ite = m_positioned.begin();
             _ite != _end;
             ++_ite) {
            const coordinates& _coords = (*_ite)->get_coordinates();
            lexeme::size_type _size = (*_ite)->get_size();
            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
                if (_coords[_i] == p_coord) {
                    return _ite;
                }
            }
        }
        return _end;
    }


};


} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // VALIDATE_POSITION_H

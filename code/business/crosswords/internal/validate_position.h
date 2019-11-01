#ifndef TENACITAS_BUSINESS_CROSSWORDS_VALIDATE_POSITION_H
#define TENACITAS_BUSINESS_CROSSWORDS_VALIDATE_POSITION_H

#include <set>

#include <business/crosswords/internal/log.h>
#include <business/crosswords/internal/positions_occupied.h>
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
struct validate_position_t
{

    typedef t_log log;

    typedef entities::crosswords::lexeme lexeme;
    typedef entities::crosswords::words words;
    typedef entities::crosswords::word word;
    typedef entities::crosswords::coordinates coordinates;
    typedef entities::crosswords::coordinate coordinate;
    typedef entities::crosswords::coordinate::x x;
    typedef entities::crosswords::coordinate::y y;

    typedef std::set<lexeme::size_type> intersections;
    typedef positions_occupied_t<t_log> positions_occupied;

    bool operator()(words::const_iterator p_begin,
                    words::const_iterator p_to_position,
                    lexeme::size_type p_intersection_idx,
                    const positions_occupied& p_positions_occupied,
                    x p_x_limit,
                    y p_y_limit) const
    {

        if (!check_boundaries(p_to_position, p_x_limit, p_y_limit)) {
            return false;
        }

        //        print();

        intersections _intersections;
        _intersections.emplace(p_intersection_idx);

        if (!colision_free(p_begin,
                           p_to_position,
                           p_intersection_idx,
                           p_positions_occupied,
                           _intersections)) {
            return false;
        }

        if (!valid_extremes(p_to_position,
                            p_positions_occupied,
                            p_x_limit,
                            p_y_limit,
                            _intersections)) {
            return false;
        }

        if (!above_and_below_are_free(
              p_to_position, p_positions_occupied, p_y_limit, _intersections)) {
            return false;
        }

        return (left_and_right_are_free(
          p_to_position, p_positions_occupied, p_x_limit, _intersections));
    }

  private:
    bool check_boundaries(words::const_iterator p_to_position,
                          x p_x_limit,
                          y p_y_limit) const
    {
        // check boundaries overflow and underflow
        if (p_to_position->get_x0() < x(0)) {
            //            crosswords_log_debug(
            //              log, "underflow in x position for ",
            //              *p_to_position);
            return false;
        }

        if (p_to_position->get_y0() < y(0)) {
            //            crosswords_log_debug(
            //              log, "underflow in y position for ",
            //              *p_to_position);
            return false;
        }

        if ((p_to_position->get_direction() == word::direction::horizontal) &&
            (p_to_position->get_xn() >= p_x_limit)) {
            //            crosswords_log_debug(
            //              log, "overflow in x position for ", *p_to_position);
            return false;
        }

        if ((p_to_position->get_direction() == word::direction::vertical) &&
            (p_to_position->get_yn() >= p_y_limit)) {
            //            crosswords_log_debug(
            //              log, "overflow in y position for ", *p_to_position);
            return false;
        }
        return true;
    }

    bool valid_extremes_horizontal(
      words::const_iterator p_word,
      const positions_occupied& p_positions_occupied,
      x p_x_limit,
      const intersections& /*p_intersections*/) const
    {

        std::pair<bool, char> _occupied({ false, ' ' });
        //        intersections::const_iterator _end = p_intersections.end();

        if (p_word->get_x0() != x(0)) {
            _occupied = p_positions_occupied.find(p_word->get_x0() - x(1),
                                                  p_word->get_y0());

            if (_occupied.first) {
                //                crosswords_log_debug(log,
                //                                     "(",
                //                                     p_word->get_x0() - x(1),
                //                                     ",",
                //                                     p_word->get_y0(),
                //                                     ") has '",
                //                                     _occupied.second,
                //                                     "'");
                return false;
            }
        }

        if (p_word->get_xn() < (p_x_limit - x(1))) {
            _occupied = p_positions_occupied.find(p_word->get_xn() + x(1),
                                                  p_word->get_y0());
            if (_occupied.first) {
                //                crosswords_log_debug(log,
                //                                     "(",
                //                                     p_word->get_xn() + x(1),
                //                                     ",",
                //                                     p_word->get_y0(),
                //                                     ") has '",
                //                                     _occupied.second,
                //                                     "'");
                return false;
            }
        }

        //        if (p_intersections.find(0) == _end) {

        //            if (p_word->get_x0() != x(0)) {
        //                if (p_word->get_y0() != y(0)) {
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_x0() - x(1),
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
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_x0() - x(1),
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
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_xn() + x(1),
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
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_xn() + x(1),
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

    bool valid_extremes_vertical(words::const_iterator p_word,
                                 const positions_occupied& p_positions_occupied,
                                 y p_y_limit,
                                 const intersections& /*p_intersections*/) const
    {
        std::pair<bool, char> _occupied({ false, ' ' });
        if (p_word->get_y0() != y(0)) {
            _occupied = p_positions_occupied.find(p_word->get_x0(),
                                                  p_word->get_y0() - y(1));

            if (_occupied.first) {
                //                crosswords_log_debug(log,
                //                                     "(",
                //                                     p_word->get_x0(),
                //                                     ",",
                //                                     p_word->get_y0() - y(1),
                //                                     ") has '",
                //                                     _occupied.second,
                //                                     "'");
                return false;
            }
        }

        if (p_word->get_yn() != (p_y_limit - y(1))) {
            _occupied = p_positions_occupied.find(p_word->get_x0(),
                                                  p_word->get_yn() + y(1));
            if (_occupied.first) {
                //                crosswords_log_debug(log,
                //                                     "(",
                //                                     p_word->get_x0(),
                //                                     ",",
                //                                     p_word->get_yn() + y(1),
                //                                     ") has '",
                //                                     _occupied.second,
                //                                     "'");
                return false;
            }
        }
        //        intersections::const_iterator _end = p_intersections.end();
        //        if (p_intersections.find(0) == _end) {
        //            if (p_word->get_y0() != y(0)) {

        //                if (p_word->get_x0() != x(0)) {
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_x0() - x(1),
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
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_x0() + x(1),
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
        //                  p_positions_occupied.find(p_word->get_x0(),
        //                  p_word->get_yn() + y(1));
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
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_x0() - x(1),
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
        //                    _occupied =
        //                    p_positions_occupied.find(p_word->get_x0() + x(1),
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

    bool valid_extremes(words::const_iterator p_word,
                        const positions_occupied& p_positions_occupied,
                        x p_x_limit,
                        y p_y_limit,
                        const intersections& p_intersections) const
    {
        // horizontal
        if ((p_word->get_direction() == word::direction::horizontal) &&
            (!valid_extremes_horizontal(
              p_word, p_positions_occupied, p_x_limit, p_intersections))) {
            return false;
        }

        if ((p_word->get_direction() == word::direction::vertical) &&
            (!valid_extremes_vertical(
              p_word, p_positions_occupied, p_y_limit, p_intersections))) {
            return false;
        }

        return true;
    }

    bool left_and_right_are_free(words::const_iterator p_word,
                                 const positions_occupied& p_positions_occupied,
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
                _occupied = p_positions_occupied.find(
                  _coords[_i].get_x() - x(1), _coords[_i].get_y());
                if (_occupied.first) {
                    //                    crosswords_log_debug(log,
                    //                                         "(",
                    //                                         _coords[_i].get_x()
                    //                                         - x(1),
                    //                                         ",",
                    //                                         _coords[_i].get_y(),
                    //                                         ") has ",
                    //                                         _occupied.second);
                    return false;
                }
            }

            if (p_word->get_x0() != (p_x_limit - x(1))) {
                _occupied = p_positions_occupied.find(
                  _coords[_i].get_x() + x(1), _coords[_i].get_y());
                if (_occupied.first) {
                    //                    crosswords_log_debug(log,
                    //                                         "(",
                    //                                         _coords[_i].get_x()
                    //                                         + x(1),
                    //                                         ",",
                    //                                         _coords[_i].get_y(),
                    //                                         ") has ",
                    //                                         _occupied.second);
                    return false;
                }
            }
        }
        return true;
    }

    bool above_and_below_are_free(
      words::const_iterator p_word,
      const positions_occupied& p_positions_occupied,
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
                _occupied = p_positions_occupied.find(
                  _coords[_i].get_x(), _coords[_i].get_y() - y(1));
                if (_occupied.first) {
                    //                    crosswords_log_debug(log,
                    //                                         "(",
                    //                                         _coords[_i].get_x(),
                    //                                         ",",
                    //                                         _coords[_i].get_y()
                    //                                         - y(1),
                    //                                         ") has ",
                    //                                         _occupied.second);
                    return false;
                }
            }

            if (p_word->get_y0() != (p_y_limit - y(1))) {
                _occupied = p_positions_occupied.find(
                  _coords[_i].get_x(), _coords[_i].get_y() + y(1));
                if (_occupied.first) {
                    //                    crosswords_log_debug(log,
                    //                                         "(",
                    //                                         _coords[_i].get_x(),
                    //                                         ",",
                    //                                         _coords[_i].get_y()
                    //                                         + y(1),
                    //                                         ") has ",
                    //                                         _occupied.second);
                    return false;
                }
            }
        }
        return true;
    }

    bool colision_free(words::const_iterator p_begin,
                       words::const_iterator p_to_position,
                       lexeme::size_type p_intersection_idx,
                       const positions_occupied& p_positions_occupied,
                       intersections& p_intersections) const
    {

        std::pair<bool, char> _occupied({ false, ' ' });
        lexeme::size_type _size = p_to_position->get_size();
        const coordinates& _coordinates = p_to_position->get_coordinates();
        const lexeme& _lexeme = p_to_position->get_lexeme();
        for (lexeme::size_type _i = 0; _i < _size; ++_i) {
            if (_i == p_intersection_idx) {
                continue;
            }
            _occupied = p_positions_occupied.find(_coordinates[_i].get_x(),
                                                  _coordinates[_i].get_y());
            if (_occupied.first) {
                if (_occupied.second != _lexeme[_i]) {
                    //                    crosswords_log_debug(log,
                    //                                         _coordinates[_i],
                    //                                         " has '",
                    //                                         _occupied.second,
                    //                                         "', BUT '",
                    //                                         _lexeme[_i],
                    //                                         "' was
                    //                                         expected");
                    return false;
                }
                words::const_iterator _intersected =
                  find(p_begin, p_to_position, _coordinates[_i]);

                //                if (_intersected == p_to_position) {
                //                    throw std::runtime_error("ERROR at line "
                //                    +
                //                                             std::to_string(__LINE__));
                //                }

                if (_intersected->get_direction() ==
                    p_to_position->get_direction()) {
                    //                    crosswords_log_debug(
                    //                      log,
                    //                      *p_to_position,
                    //                      " and ",
                    //                      *_intersected,
                    //                      " intersect, but have the same
                    //                      direction");
                    return false;
                }

                p_intersections.emplace(_i);
            }
        }
        return true;
    }

    words::const_iterator find(words::const_iterator p_first_positioned,
                               words::const_iterator p_end_positioned,
                               const coordinate& p_coord) const
    {
        words::const_iterator _end = p_end_positioned;
        for (words::const_iterator _ite = p_first_positioned; _ite != _end;
             ++_ite) {
            const coordinates& _coords = _ite->get_coordinates();
            lexeme::size_type _size = _ite->get_size();
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

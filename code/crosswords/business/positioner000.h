#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include <crosswords/business/internal/log.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief positioner000_t position \p words
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
struct positioner000_t
{

    typedef entities::words words;
    typedef entities::word word;
    typedef word::direction direction;
    typedef word::orientation orientation;
    typedef entities::lexeme lexeme;
    typedef entities::description description;
    typedef entities::coordinate coordinate;
    typedef entities::coordinates coordinates;
    typedef coordinate::x x;
    typedef coordinate::y y;
    typedef t_log log;
    typedef std::set<lexeme::size_type> intersections;

    enum class result : int16_t
    {
        ok = 0,
        lexeme_overflows_horizontal = 1,
        lexeme_overflows_vertical = 2
    };

    positioner000_t(x p_x_limit, y p_y_limit)
        : m_x_limit(p_x_limit)
        , m_y_limit(p_y_limit)
    {
        crosswords_log_debug(log, "y = ", m_y_limit, ", x = ", m_x_limit);
    }

    result add(const lexeme& p_lexeme, const description& p_description)
    {
        lexeme::size_type _word_size = p_lexeme.size();
        if (m_y_limit < y(_word_size)) {
            crosswords_log_error(
                        log,
                        p_lexeme,
                        "'s size is ",
                        _word_size,
                        ", which is bigger than the vertical limit of ",
                        m_y_limit);
            return result::lexeme_overflows_vertical;
        }
        if (m_x_limit < x(_word_size)) {
            crosswords_log_error(
                        log,
                        p_lexeme,
                        "'s size is ",
                        _word_size,
                        ", which is bigger than the horizontal limit of ",
                        m_x_limit);
            return result::lexeme_overflows_horizontal;
        }
        crosswords_log_debug(log,
                             "'",
                             p_lexeme,
                             "', with description '",
                             p_description,
                             "' was added");
        m_words.push_back(word(
                              static_cast<word::id>(m_words.size() + 1), p_lexeme, p_description));
        return result::ok;
    }

    words operator()()
    {
        log_words();
        //sort_words_by_size();
        //        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        create_first_words_configuration();
        log_current_words_configuration();
        log_positioned();
        //        std::this_thread::sleep_for(std::chrono::milliseconds(8000));
        if (position_words()) {
            optimize_words();
        }
        return m_words;
    }

private:
    typedef std::list<words::iterator> words_pointers;

    typedef std::list<words_pointers> words_configurations;

    typedef std::map<coordinate, char> occupied;

private:
    inline void sort_words_by_size()
    {
        crosswords_log_debug(log, "ordering words by lexeme size, descending");
        std::sort(m_words.begin(),
                  m_words.end(),
                  [](const word& p_pos1, const word& p_pos2) -> bool {
            return (p_pos1.get_lexeme().size() >
                    p_pos2.get_lexeme().size());
        });
    }

    inline void create_first_words_configuration()
    {
        crosswords_log_debug(log, "creating first words configuration");
        for (words::iterator _ite = m_words.begin(); _ite != m_words.end();
             ++_ite) {
            m_first_configuration.push_back(_ite);
        }
        m_words_configurations.push_back(m_first_configuration);
    }

    void log_master_configuration()
    {
        crosswords_log_debug(log, "log_master_configuration");
        words_pointers::const_iterator _end = m_first_configuration.end();
        words_pointers::const_iterator _ite = m_first_configuration.begin();

        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            words::const_iterator _pos = *_ite;
            crosswords_log_debug(
                        log, "\t", _i, ":", &(*_pos), ":", _pos->get_lexeme());
            ++_i;
        }
    }

    void log_current_words_configuration()
    {
        define_current_words_configuration();
        crosswords_log_info(log, "log_current_words_configuration");
        log_configuration(m_current_configuration);
    }

    void log_configuration(words_configurations::const_iterator p_ite_cfg)
    {
        words_pointers::const_iterator _end = p_ite_cfg->end();
        words_pointers::const_iterator _ite = p_ite_cfg->begin();

        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            words::const_iterator _pos = *_ite;
            crosswords_log_info(
                        log, "\t", _i, ":", &(*_pos), ":", _pos->get_lexeme());
            ++_i;
        }
    }

    std::string words_ite2str(words_pointers::const_iterator p_ite) const
    {
        std::stringstream _stream;
        words::const_iterator _pos = *p_ite;
        _stream << "(" << &(*_pos) << "," << (*p_ite)->get_id() << ","
                << (*p_ite)->get_lexeme() << ")";
        return _stream.str();
    }
    void log_words() const
    {
        crosswords_log_debug(log, "log_words");
        words::const_iterator _end = m_words.end();
        words::const_iterator _ite = m_words.begin();
        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            crosswords_log_debug(log,
                                 "\t",
                                 _i,
                                 "=(",
                                 &(*_ite),
                                 ",",
                                 _ite->get_id(),
                                 ",",
                                 _ite->get_lexeme(),
                                 ")");

            ++_i;
        }
    }

    void log_positioned() const
    {
        crosswords_log_info(log, "log_positioned");
        if (m_positioned.empty()) {
            crosswords_log_debug(log, "m_positioned is empty");
        }
        words_pointers::const_iterator _end = m_positioned.end();
        words_pointers::const_iterator _ite = m_positioned.begin();

        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            words::const_iterator _pos = *_ite;
            crosswords_log_info(
                        log,
                        "\t",
                        _pos->get_id(),
                        ":",
                        &(*_pos),
                        ":",
                        _pos->get_lexeme(),
                        ":",
                        _pos->get_x0(),
                        ":",
                        _pos->get_y0(),
                        ":",
                        (_pos->get_direction() == word::direction::vertical ? "V" : "H"),
                        ":",
                        (_pos->get_orientation() == word::orientation::forward ? "F"
                                                                               : "B"));
            ++_i;
        }
    }

    bool position_words()
    {
        crosswords_log_debug(log, "positon_words");

        bool _is_positioned = true;

        define_current_words_configuration();
        words_pointers::iterator _ptr_word_to_position =
                m_current_configuration->begin();

        while (true) {
            crosswords_log_debug(log,
                                 "===> starting loop, trying to position word ",
                                 words_ite2str(_ptr_word_to_position));
            if (position(*_ptr_word_to_position)) {
                crosswords_log_debug(log,
                                     words_ite2str(_ptr_word_to_position),
                                     " was positioned: ",
                                     *(*_ptr_word_to_position));
                print();

                if (m_positioned.size() == m_current_configuration->size()) {
                    crosswords_log_debug(log, "all positioned");
                    _is_positioned = true;
                    break;
                }
                ++_ptr_word_to_position;
            } else {
                crosswords_log_debug(log,
                                     words_ite2str(_ptr_word_to_position),
                                     " was not positioned");
                log_positioned();

                update_words_configuration(_ptr_word_to_position);
                define_current_words_configuration();
                crosswords_log_info(
                            log, "################# new current configuration defined");
                log_current_words_configuration();

                if (equals(m_first_configuration, *m_current_configuration)) {
                    crosswords_log_warn(log, "MASTER CYCLE!!!!");
                    _is_positioned = false;
                    break;
                }

                if (cycle()) {
                    crosswords_log_warn(log, "=============== CYCLE!!");
                    //                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    {
                        for (word& _word : m_words) {
                            _word.unposition();
                        }
                    }
                    m_positioned.clear();
                    m_occupied.clear();
                    words_pointers _first = *(m_words_configurations.begin());
                    words_pointers::iterator _ite = _first.begin();
                    _first.erase(_ite);
                    _first.push_back(*_ite);
                    m_words_configurations.clear();
                    m_words_configurations.push_back(std::move(_first));
                    define_current_words_configuration();
                }

                _ptr_word_to_position = m_current_configuration->begin();

                words_pointers::size_type _size = m_positioned.size();

                crosswords_log_debug(
                            log, "moving to the right word in the configuration");
                for (words_pointers::size_type _counter = 0; _counter < _size;
                     ++_counter) {
                    crosswords_log_debug(
                                log, "\t", words_ite2str(_ptr_word_to_position));
                    ++_ptr_word_to_position;
                }
            }
        }
        return _is_positioned;
    }

    void update_words_configuration(words_pointers::const_iterator p_ite)
    {
        words_pointers _words_pointers(*m_current_configuration);

        _words_pointers.remove(*p_ite);

        _words_pointers.push_back(*p_ite);

        m_words_configurations.push_back(std::move(_words_pointers));
    }

    bool position(words::iterator p_ptr_word_to_position)
    {
        if (m_positioned.empty()) {
            position_first(p_ptr_word_to_position);
            return true;
        }
        words_pointers::const_iterator _positioned_end = m_positioned.end();
        words_pointers::const_iterator _positioned_ite = m_positioned.begin();
        while (true) {
            if (_positioned_ite == _positioned_end) {
                return false;
            }

            bool _positioned =
                    position(*_positioned_ite, p_ptr_word_to_position);

            if (_positioned) {

                m_positioned.push_back(p_ptr_word_to_position);
                add_to_occupied(p_ptr_word_to_position);

                break;
            }

            ++_positioned_ite;
        }
        return true;
    }

    bool position(words::const_iterator p_positioned,
                  words::iterator p_to_position)
    {
        crosswords_log_debug(log,
                             "trying to position ",
                             p_to_position->get_lexeme(),
                             " against ",
                             *p_positioned);

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
                    if (valid_position(p_to_position, _i_to_position)) {
                        return true;
                    }
                    crosswords_log_debug(log, "invalid position");
                    p_to_position->unposition();
                }
            }
        }
        return false;
    }

    bool check_boundaries_over_under_flow(
            words::const_iterator p_to_position) const
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
                (p_to_position->get_xn() >= m_x_limit)) {
            crosswords_log_debug(
                        log, "overflow in x position for ", *p_to_position);
            return false;
        }

        if ((p_to_position->get_direction() == word::direction::vertical) &&
                (p_to_position->get_yn() >= m_y_limit)) {
            crosswords_log_debug(
                        log, "overflow in y position for ", *p_to_position);
            return false;
        }
        return true;
    }

    std::pair<bool, char> is_occupied(x p_x, y p_y) const
    {
        occupied::const_iterator _ite =
                m_occupied.find({ std::move(p_x), std::move(p_y) });
        if (_ite == m_occupied.end()) {
            return { false, ' ' };
        }
        return { true, _ite->second };
    }

    std::pair<bool, char> is_occupied(const coordinate& p_coord) const
    {
        occupied::const_iterator _ite =
                m_occupied.find({ p_coord.get_x(), p_coord.get_y() });
        if (_ite == m_occupied.end()) {
            return { false, ' ' };
        }
        return { true, _ite->second };
    }

    bool valid_extremes_horizontal(words::const_iterator p_word,
                                   const intersections& p_intersections) const
    {

        std::pair<bool, char> _occupied({ false, ' ' });
        intersections::const_iterator _end = p_intersections.end();

        if (p_word->get_x0() != x(0)) {
            _occupied = is_occupied(p_word->get_x0() - x(1), p_word->get_y0());

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

        if (p_word->get_xn() < (m_x_limit - x(1))) {
            _occupied = is_occupied(p_word->get_xn() + x(1), p_word->get_y0());
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

        if (p_intersections.find(0) == _end) {

            if (p_word->get_x0() != x(0)) {
                if (p_word->get_y0() != y(0)) {
                    _occupied = is_occupied(p_word->get_x0() - x(1),
                                            p_word->get_y0() - y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_x0() - x(1),
                                             ",",
                                             p_word->get_y0() - y(1),
                                             ") has '",
                                             _occupied.second,
                                             "'");
                        return false;
                    }
                }

                if (p_word->get_y0() != (m_y_limit - y(1))) {
                    _occupied = is_occupied(p_word->get_x0() - x(1),
                                            p_word->get_y0() + y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_x0() - x(1),
                                             ",",
                                             p_word->get_y0() + y(1),
                                             ") has '",
                                             _occupied.second,
                                             "'");
                        return false;
                    }
                }
            }
        }

        if (p_intersections.find(p_word->get_size() - 1) == _end) {
            if (p_word->get_xn() < (m_x_limit - x(1))) {
                if (p_word->get_y0() != y(0)) {
                    _occupied = is_occupied(p_word->get_xn() + x(1),
                                            p_word->get_y0() - y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_xn() + x(1),
                                             ",",
                                             p_word->get_y0() - y(1),
                                             ") has '",
                                             _occupied.second);
                        return false;
                    }
                }

                if (p_word->get_y0() != (m_y_limit - y(1))) {
                    _occupied = is_occupied(p_word->get_xn() + x(1),
                                            p_word->get_y0() + y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_xn() + x(1),
                                             ",",
                                             p_word->get_y0() + y(1),
                                             ") has '",
                                             _occupied.second,
                                             "'");
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool valid_extremes_vertical(words::const_iterator p_word,
                                 const intersections& p_intersections) const
    {
        std::pair<bool, char> _occupied({ false, ' ' });
        if (p_word->get_y0() != y(0)) {
            _occupied = is_occupied(p_word->get_x0(), p_word->get_y0() - y(1));

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

        if (p_word->get_yn() != (m_y_limit - y(1))) {
            _occupied = is_occupied(p_word->get_x0(), p_word->get_yn() + y(1));
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
        intersections::const_iterator _end = p_intersections.end();
        if (p_intersections.find(0) == _end) {
            if (p_word->get_y0() != y(0)) {

                if (p_word->get_x0() != x(0)) {
                    _occupied = is_occupied(p_word->get_x0() - x(1),
                                            p_word->get_y0() - y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_x0() - x(1),
                                             ",",
                                             p_word->get_y0() - y(1),
                                             ") has '",
                                             _occupied.second,
                                             "'");
                        return false;
                    }
                }

                if (p_word->get_x0() != (m_x_limit - x(1))) {
                    _occupied = is_occupied(p_word->get_x0() + x(1),
                                            p_word->get_y0() - y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_x0() + x(1),
                                             ",",
                                             p_word->get_y0() - y(1),
                                             ") has '",
                                             _occupied.second,
                                             "'");
                        return false;
                    }
                }
            }
        }

        if (p_intersections.find(p_word->get_size() - 1) == _end) {
            if (p_word->get_yn() != (m_y_limit - y(1))) {
                _occupied =
                        is_occupied(p_word->get_x0(), p_word->get_yn() + y(1));
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

                if (p_word->get_x0() != x(0)) {
                    _occupied = is_occupied(p_word->get_x0() - x(1),
                                            p_word->get_yn() + y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_x0() - x(1),
                                             ",",
                                             p_word->get_yn() + y(1),
                                             ") has '",
                                             _occupied.second);
                        return false;
                    }
                }

                if (p_word->get_x0() != (m_x_limit - x(1))) {
                    _occupied = is_occupied(p_word->get_x0() + x(1),
                                            p_word->get_yn() + y(1));
                    if (_occupied.first) {
                        crosswords_log_debug(log,
                                             "(",
                                             p_word->get_x0() + x(1),
                                             ",",
                                             p_word->get_yn() + y(1),
                                             ") has '",
                                             _occupied.second,
                                             ")");
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool valid_extremes(words::const_iterator p_word,
                        const intersections& p_intersections) const
    {
        // horizontal
        if ((p_word->get_direction() == word::direction::horizontal) &&
                (!valid_extremes_horizontal(p_word, p_intersections))) {
            return false;
        }

        if ((p_word->get_direction() == word::direction::vertical) &&
                (!valid_extremes_vertical(p_word, p_intersections))) {
            return false;
        }

        return true;
    }

    bool left_and_right_are_free(words::const_iterator p_word,
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
                        is_occupied(_coords[_i].get_x() - x(1), _coords[_i].get_y());
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

            if (p_word->get_x0() != (m_x_limit - x(1))) {
                _occupied =
                        is_occupied(_coords[_i].get_x() + x(1), _coords[_i].get_y());
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

    bool above_and_below_are_free(words::const_iterator p_word,
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
                        is_occupied(_coords[_i].get_x(), _coords[_i].get_y() - y(1));
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

            if (p_word->get_y0() != (m_y_limit - y(1))) {
                _occupied =
                        is_occupied(_coords[_i].get_x(), _coords[_i].get_y() + y(1));
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

    bool valid_position(words::const_iterator p_to_position,
                        lexeme::size_type p_intersection_idx) const
    {

        if (!check_boundaries_over_under_flow(p_to_position)) {
            return false;
        }

        print();

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
                    is_occupied(_coordinates[_i].get_x(), _coordinates[_i].get_y());
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
                words_pointers::const_iterator _intersected =
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

        //        if (!valid_extremes(p_to_position, _intersections)) {
        //            return false;
        //        }

        //        if (!above_and_below_are_free(p_to_position, _intersections)) {
        //            return false;
        //        }

        //        return (left_and_right_are_free(p_to_position, _intersections));

        return true;
    }

    words_pointers::const_iterator find(const coordinate& p_coord) const
    {
        words_pointers::const_iterator _end = m_positioned.end();
        for (words_pointers::const_iterator _ite = m_positioned.begin();
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

    inline void define_current_words_configuration()
    {
        m_current_configuration = m_words_configurations.end();
        --m_current_configuration;
    }

    bool cycle()
    {
        if (m_words_configurations.size() <= 1) {
            return false;
        }

        words_configurations::const_iterator _last =
                m_words_configurations.end();
        --_last;
        words_configurations::const_iterator _ite =
                m_words_configurations.begin();
        while (true) {
            if (_ite == _last) {
                break;
            }
            if (equals(*_ite, *_last)) {
                return true;
            }
            ++_ite;
        }
        return false;
    }

    bool equals(const words_pointers& p_ite1, const words_pointers& p_ite2)
    {
        words_pointers::const_iterator _end1 = p_ite1.end();
        words_pointers::const_iterator _ite1 = p_ite1.begin();

        words_pointers::const_iterator _end2 = p_ite2.end();
        words_pointers::const_iterator _ite2 = p_ite2.begin();

        while (true) {
            if ((_ite1 == _end1) || (_ite2 == _end2)) {
                break;
            }

            if ((*_ite1)->get_id() != (*_ite2)->get_id()) {
                return false;
            }
            ++_ite1;
            ++_ite2;
        }
        return true;
    }

    void position_first(words::iterator p_ptr_word_to_position)
    {
        if (m_y_limit > y(m_x_limit)) {
            x _x(m_x_limit / x(2));
            y _y((m_y_limit / y(2)) -
                 y(p_ptr_word_to_position->get_lexeme().size() / 2));
            p_ptr_word_to_position->position(
                        _x, _y, direction::vertical, orientation::forward);
        } else {
            x _x((m_x_limit / x(2)) -
                 x(p_ptr_word_to_position->get_lexeme().size() / 2));
            y _y(m_y_limit / y(2));
            p_ptr_word_to_position->position(
                        _x, _y, direction::horizontal, orientation::forward);
        }

        //        if (m_y_limit > y(m_x_limit)) {
        //            p_ptr_word_to_position->position(
        //              x(0), y(0), direction::horizontal, orientation::forward);
        //        } else {
        //            p_ptr_word_to_position->position(
        //              x(0), y(0), direction::vertical, orientation::forward);
        //        }

        m_positioned.push_back(p_ptr_word_to_position);
        add_to_occupied(p_ptr_word_to_position);
        print();
    }

    void optimize_words()
    {
        x _min_x = std::numeric_limits<x>::max();
        y _min_y = std::numeric_limits<y>::max();

        {
            words::const_iterator _end = m_words.end();
            for (words::const_iterator _ite = m_words.begin(); _ite != _end;
                 ++_ite) {
                if (_ite->get_x0() < _min_x) {
                    _min_x = _ite->get_x0();
                }
                if (_ite->get_y0() < _min_y) {
                    _min_y = _ite->get_y0();
                }
            }
        }
        words::iterator _end = m_words.end();
        if ((_min_x > x(0)) || (_min_y > y(0))) {
            for (words::iterator _ite = m_words.begin(); _ite != _end; ++_ite) {
                _ite->position(_ite->get_x0() - _min_x,
                               _ite->get_y0() - _min_y,
                               _ite->get_direction(),
                               _ite->get_orientation());
            }
        }
    }

    void add_to_occupied(words::const_iterator p_word)
    {
        const lexeme& _lexeme = p_word->get_lexeme();
        x _x = p_word->get_x0();
        y _y = p_word->get_y0();
        lexeme::size_type _size = _lexeme.size();
        if (p_word->get_direction() == word::direction::vertical) {
            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
                m_occupied[{ _x, _y + y(_i) }] = _lexeme[_i];
            }
        } else {
            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
                m_occupied[{ _x + x(_i), _y }] = _lexeme[_i];
            }
        }
    }

    void print() const
    {
        using namespace std;

        typedef std::vector<std::string> matrix;

        string::size_type _x_size =
                static_cast<string::size_type>(m_x_limit.get_value());
        string::size_type _y_size =
                static_cast<string::size_type>(m_y_limit.get_value());

        matrix _m(_x_size, string(_y_size, ' '));

        words::const_iterator _end = m_words.end();
        for (words::const_iterator _ite = m_words.begin(); _ite != _end;
             ++_ite) {
            if (_ite->positioned()) {
                const coordinates& _coords = _ite->get_coordinates();
                const lexeme& _lexeme = _ite->get_lexeme();
                for (lexeme::size_type _i = 0; _i < _lexeme.size(); ++_i) {
                    matrix::size_type _x = static_cast<string::size_type>(
                                _coords[_i].get_x().get_value());
                    string::size_type _y = static_cast<std::string::size_type>(
                                _coords[_i].get_y().get_value());
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
    x m_x_limit;
    y m_y_limit;

    words m_words;
    words_configurations m_words_configurations;
    words_pointers m_positioned;
    words_configurations::iterator m_current_configuration;
    occupied m_occupied;
    words_pointers m_first_configuration;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER_H

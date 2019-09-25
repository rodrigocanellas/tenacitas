#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <limits>
#include <list>
#include <sstream>
#include <string>
#include <utility>
#include <map>
#include <tuple>


#include <crosswords/business/internal/log.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/limit.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>
#include <crosswords/entities/lexeme.h>

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
    typedef entities::limit limit;
    typedef t_log log;

    enum class result : int16_t
    {
        ok = 0,
        lexeme_overflows_horizontal = 1,
        lexeme_overflows_vertical = 2
    };

    positioner000_t(limit p_vertical_limit, limit p_horizontal_limit)
        : m_vertical(p_vertical_limit)
        , m_horizontal(p_horizontal_limit)
    {
        crosswords_log_debug(
                    log, "vertical = ", m_vertical, ", horizontal = ", m_horizontal);
    }

    result add(const lexeme& p_lexeme, const description& p_description)
    {
        limit _word_size = static_cast<limit>(p_lexeme.size());
        if (_word_size > m_vertical) {
            crosswords_log_error(
                        log,
                        p_lexeme,
                        "'s size is ",
                        _word_size,
                        ", which is bigger than the vertical limit of ",
                        m_vertical);
            return result::lexeme_overflows_vertical;
        }
        if (_word_size > m_horizontal) {
            crosswords_log_error(
                        log,
                        p_lexeme,
                        "'s size is ",
                        _word_size,
                        ", which is bigger than the horizontal limit of ",
                        m_horizontal);
            return result::lexeme_overflows_horizontal;
        }
        crosswords_log_debug(log,
                             "'",
                             p_lexeme,
                             "', with description '",
                             p_description,
                             "' was added");
        m_words.push_back(
                    word(static_cast<word::id>(m_words.size() + 1),
                         p_lexeme,
                         p_description));
        return result::ok;
    }

    words operator()()
    {
        log_words();
        sort_words_by_size();
        create_first_words_configuration();
        log_current_words_configuration();
        position_words();
//        optimize_words();
        return m_words;
    }

private:
    typedef std::list<words::iterator> words_pointers;

    typedef std::list<words_pointers> words_configurations;

    typedef std::pair<word::x, word::y> coordinate;

//    struct cmp_item {
//        bool operator()(const item & p_left, const item & p_right) {
//            return ( (std::get<0>(p_left) < std::get<0>(p_right)) &&
//                     (std::get<1>(p_left) < std::get<1>(p_right)) ) ;
//        }
//    };


    typedef std::map<coordinate, char> occupied;

private:

    inline void sort_words_by_size()
    {
        crosswords_log_debug(log,
                             "ordering words by lexeme size, descending");
        m_words.sort(
                    [](const word& p_pos1, const word& p_pos2) -> bool {
            return (p_pos1.get_lexeme().size() > p_pos2.get_lexeme().size());
        });
    }

    inline void create_first_words_configuration()
    {
        words_pointers _first;
        crosswords_log_debug(log, "creating first words history");
        for (words::iterator _ite = m_words.begin();
             _ite != m_words.end();
             ++_ite) {
            _first.push_back(_ite);
        }
        m_words_configurations.push_back(std::move(_first));
    }

    void log_current_words_configuration()
    {
        define_current_words_configuration();
        crosswords_log_debug(log, "log_current_words_configuration");
        words_pointers::const_iterator _end = m_current_configuration->end();
        words_pointers::const_iterator _ite = m_current_configuration->begin();

        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            words::const_iterator _pos = *_ite;
            crosswords_log_debug(log, "\t", _i, ":", &(*_pos));
            ++_i;
        }
    }

    std::string words_ite2str(
            words_pointers::const_iterator p_ite) const
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

    bool position_words()
    {
        crosswords_log_debug(log, "positon_words");

        bool _is_positioned = true;

        define_current_words_configuration();
        words_pointers::iterator _ptr_word_to_position =
                m_current_configuration->begin();

        while (true) {
            crosswords_log_debug(log,
                                 "starting loop, trying to position word ",
                                 words_ite2str(_ptr_word_to_position));
            if (position(*_ptr_word_to_position)) {
                crosswords_log_debug(log,
                                     words_ite2str(_ptr_word_to_position),
                                     " was positioned ");
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
                if ((*_ptr_word_to_position)->get_id() ==
                        m_current_configuration->back()->get_id()) {
                    crosswords_log_debug(
                                log, "and it was the last, so we clear positioned list");
                    m_positioned.clear();
                }
                crosswords_log_debug(log, "updating history");
                update_words_configuration(_ptr_word_to_position);
                crosswords_log_debug(log, "checking for cycle");
                if (cycle()) {
                    crosswords_log_debug(log, "CYCLE!!");
                    _is_positioned = false;
                    break;
                }
                define_current_words_configuration();
                _ptr_word_to_position = m_current_configuration->begin();

                crosswords_log_debug(log,
                                     "new current history defined, ",
                                     words_ite2str(_ptr_word_to_position),
                                     " and is the new current");
                words_pointers::size_type _size = m_positioned.size();

                crosswords_log_debug(
                            log, "moving to the right word in the history");
                for (words_pointers::size_type _counter = 0;
                     _counter < _size;
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
        while(true) {
            if (_positioned_ite==_positioned_end) {
                return false;
            }

            bool _positioned=
                    position(*_positioned_ite, p_ptr_word_to_position);

            if (_positioned) {
                if (p_ptr_word_to_position->get_x() < 0) {
                    crosswords_log_debug(log, "underflow in x position for "
                                         , *p_ptr_word_to_position);
                    p_ptr_word_to_position->unposition();
                    return false;
                }
                if (p_ptr_word_to_position->get_x() > m_horizontal) {
                    crosswords_log_debug(log, "overflow in x position for "
                                         , *p_ptr_word_to_position);
                    p_ptr_word_to_position->unposition();
                    return false;
                }
                if (p_ptr_word_to_position->get_y() < 0) {
                    crosswords_log_debug(log, "underflow in y position for "
                                         , *p_ptr_word_to_position);
                    p_ptr_word_to_position->unposition();
                    return false;
                }
                if (p_ptr_word_to_position->get_y() > m_vertical) {
                    crosswords_log_debug(log, "overflow in y position for "
                                         , *p_ptr_word_to_position);
                    p_ptr_word_to_position->unposition();
                    return false;
                }


                m_positioned.push_back(p_ptr_word_to_position);
                add_to_occupied(p_ptr_word_to_position);

                return true;
            }
            else {
                ++_positioned_ite;
            }
        }
        return false;
    }

    bool position(words::const_iterator p_positioned, words::iterator p_to_position) {
        const lexeme & _positioned = p_positioned->get_lexeme();
        lexeme::size_type _positioned_size = _positioned.size();

        const lexeme & _to_position = p_to_position->get_lexeme();
        lexeme::size_type _to_position_size = _to_position.size();

        for (lexeme::size_type _i_positioned = 0;
             _i_positioned < _positioned_size;
             ++_i_positioned) {
            for (lexeme::size_type _i_to_position = 0;
                 _i_to_position < _to_position_size;
                 ++_i_to_position) {
                if (_to_position[_i_to_position] == _positioned[_i_positioned]) {
                    if (p_positioned->get_direction()==word::direction::vertical) {

                        p_to_position->position(p_positioned->get_x()-
                                                static_cast<word::x>(_i_to_position)
                                                , p_positioned->get_y()+
                                                static_cast<word::y>(_i_positioned)
                                                , word::direction::horizontal
                                                , word::orientation::forward);
                    }
                    else {
                        p_to_position->position(p_positioned->get_x()-
                                                static_cast<word::x>(_i_positioned)
                                                , p_positioned->get_y()-
                                                static_cast<word::y>(_i_to_position)
                                                , word::direction::vertical
                                                , word::orientation::forward);
                    }
                    if (!valid_position(p_to_position, _i_to_position)) {
                        p_to_position->unposition();
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool valid_position(words::const_iterator p_to_position
                        , lexeme::size_type p_intersection_idx) const {

        const lexeme & _lexeme = p_to_position->get_lexeme();
        lexeme::size_type _size = _lexeme.size();


        // intersection is not at the begining
        if (p_intersection_idx != 0) {
            word::x _x0 = p_to_position->get_x();
            word::y _y0 = p_to_position->get_y();

            if (!is_valid_extreme(_x0, _y0, p_to_position->get_direction())) {
                return false;
            }

        }
        else if (p_intersection_idx != (_size - 1)) {
            // intersection is not at the end

            if (p_to_position->get_direction() == word::direction::horizontal) {
                word::x _x0 = p_to_position->get_x() + _size;
                word::y _y0 = p_to_position->get_y();

                if (!is_valid_extreme(_x0, _y0, word::direction::horizontal)) {
                    return false;
                }
            }
            else {
                word::x _x0 = p_to_position->get_x();
                word::y _y0 = p_to_position->get_y()+_size;

                if (!is_valid_extreme(_x0, _y0, word::direction::vertical)) {
                    return false;
                }
            }
        }




//        if (p_to_position->get_direction() == word::direction::horizontal){
//            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
//                if (_i != p_intersection_idx) {
//                    // AQUI

//                }
//            }
//        }



//        for (lexeme::size_type _i = 0; _i < _size; ++_i) {
//            if (_i != p_intersection_idx) {
//                if (p_to_position->get_direction() == word::direction::vertical) {
//                    word::x _x = p_to_position->get_x();
//                    word::y _y = p_to_position->get_y() + _i;
//                    std::pair<bool, char> _occupied = is_occupied(_x, _y);
//                    if (_occupied.first) {
//                        if (_occupied.second != _lexeme[_i]) {
//                            return false;
//                        }
//                        else {

//                        }
//                    }
//                }
//            }
//        }

        return true;
    }

    bool is_valid_extreme(word::x p_x, word::y p_y, word::direction p_direction) const {
        if (p_direction == word::direction::horizontal) {
            if  (p_x > 0) {
                if (is_occupied(p_x-1, p_y).first) {
                    return false;
                }

                if ( (p_y > 0) && (is_occupied(p_x-1, p_y-1).first) ) {
                    return false;

                }
                if ( (p_y < m_vertical) && (is_occupied(p_x-1, p_y+1).first)) {
                    return false;
                }
            }
        }
        else {
            if  (p_y > 0) {
                if (is_occupied(p_x, p_y-1).first) {
                    return false;
                }

                if ( (p_x > 0) && (is_occupied(p_x-1, p_y-1).first) ) {
                    return false;

                }
                if ( (p_x < m_horizontal) && (is_occupied(p_x+1, p_y-1).first)) {
                    return false;
                }
            }

        }
        return true;
    }


    std::pair<bool, char> is_occupied(word::x p_x, word::y p_y) const {
        coordinate _coordinate = {p_x, p_y};
        occupied::const_iterator _ite =  m_occupied.find(_coordinate);
        if (_ite == m_occupied.end()) {
            return {false, ' '};
        }
        return {true, _ite->second};
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

        words_configurations::const_iterator _last = m_words_configurations.end();
        --_last;
        words_configurations::const_iterator _ite = m_words_configurations.begin();
        while (true) {
            if (_ite == _last) {
                break;
            }
            if (!equals(*_ite, *_last)) {
                return false;
            }
            ++_ite;
        }
        return true;
    }

    bool equals(const words_pointers& p_ite1,
                const words_pointers& p_ite2)
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
        if (m_vertical < m_horizontal) {
            limit x = static_cast<limit>(m_horizontal / 2);
            limit y = static_cast<limit>(m_vertical / 2) -
                    static_cast<limit>(p_ptr_word_to_position->get_lexeme().size() / 2);
            p_ptr_word_to_position->position(x, y, direction::vertical, orientation::forward);
        } else {
            limit x = static_cast<limit>(m_horizontal / 2) -
                    static_cast<limit>(p_ptr_word_to_position->get_lexeme().size() / 2);
            limit y = static_cast<limit>(m_vertical / 2);
            p_ptr_word_to_position->position(x, y, direction::horizontal, orientation::forward);
        }
        m_positioned.push_back(p_ptr_word_to_position);
        add_to_occupied(p_ptr_word_to_position);
    }

    void optimize_words()
    {
        limit _min_x = std::numeric_limits<limit>::max();
        limit _min_y = _min_x;

        {
            words::const_iterator _end = m_words.end();
            for (words::const_iterator _ite = m_words.begin();
                 _ite != _end;
                 ++_ite) {
                if (_ite->get_x() < _min_x) {
                    _min_x = _ite->get_x();
                }
                if (_ite->get_y() < _min_y) {
                    _min_y = _ite->get_y();
                }
            }
        }
        words::iterator _end = m_words.end();
        if ((_min_x > 0) || (_min_y > 0)) {
            for (words::iterator _ite = m_words.begin(); _ite != _end;
                 ++_ite) {
                _ite->position(_ite->get_x() - _min_x,
                               _ite->get_y() - _min_y,
                               _ite->get_direction(),
                               _ite->get_orientation());
            }
        }
    }

    void add_to_occupied(words::const_iterator p_word) {
        const lexeme & _lexeme = p_word->get_lexeme();
        word::x _x = p_word->get_x();
        word::y _y = p_word->get_y();
        lexeme::size_type _size = _lexeme.size();
        if (p_word->get_direction() == word::direction::vertical) {
            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
                m_occupied[{_x, _y + _i}] = _lexeme[_i];
            }
        }
        else {
            for (lexeme::size_type _i = 0; _i < _size; ++_i) {
                m_occupied[{_x + _i, _y}] = _lexeme[_i];
            }
        }
    }

private:
    limit m_vertical;
    limit m_horizontal;

    words m_words;
    words_configurations m_words_configurations;
    words_pointers m_positioned;
    words_configurations::iterator m_current_configuration;
    occupied m_occupied;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER_H

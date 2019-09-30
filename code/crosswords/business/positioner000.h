#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER_000_H

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>

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
    typedef coordinate::x x;
    typedef coordinate::y y;
    typedef t_log log;

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
        crosswords_log_debug(
          log, "vertical = ", m_y_limit, ", horizontal = ", m_x_limit);
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
        sort_words_by_size();
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
        m_words.sort([](const word& p_pos1, const word& p_pos2) -> bool {
            return (p_pos1.get_lexeme().size() > p_pos2.get_lexeme().size());
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
              _pos->get_x(),
              ":",
              _pos->get_y(),
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
                                 "starting loop, trying to position word ",
                                 words_ite2str(_ptr_word_to_position));
            if (position(*_ptr_word_to_position)) {
                crosswords_log_debug(log,
                                     words_ite2str(_ptr_word_to_position),
                                     " was positioned: ",
                                     *(*_ptr_word_to_position));

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
                log_master_configuration();
                //                std::this_thread::sleep_for(std::chrono::milliseconds(3000));

                if (equals(m_first_configuration, *m_current_configuration)) {
                    crosswords_log_warn(log, "MASTER CYCLE!!!!");
                    _is_positioned = false;
                    break;
                }

                if (cycle()) {
                    crosswords_log_warn(log, "CYCLE!!");
                    std::this_thread::sleep_for(std::chrono::seconds(5));
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
        crosswords_log_debug(
          log, "trying to position against ", p_positioned->get_lexeme());
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
                          //                          p_positioned->get_x() -
                          //                          x(_i_to_position),
                          //                          p_positioned->get_y() +
                          //                          y(_i_positioned),

                          p_positioned->get_x() - x(_i_to_position),
                          p_positioned->get_y() + y(_i_positioned),

                          word::direction::horizontal,
                          word::orientation::forward);
                    } else {
                        p_to_position->position(
                          //                          p_positioned->get_x() -
                          //                          x(_i_positioned),
                          //                          p_positioned->get_y() -
                          //                          y(_i_to_position),
                          p_positioned->get_x() + x(_i_positioned),
                          p_positioned->get_y() - y(_i_to_position),

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

    bool valid_position(words::iterator p_to_position,
                        lexeme::size_type p_intersection_idx) const
    {

        // check boundaries overflow and underflow
        if (p_to_position->get_x() < x(0)) {
            crosswords_log_debug(
              log, "underflow in x position for ", *p_to_position);
            p_to_position->unposition();
            return false;
        }
        if ((p_to_position->get_direction() == word::direction::horizontal) &&
            (p_to_position->get_x() + x(p_to_position->get_lexeme().size()) >
             m_x_limit)) {
            crosswords_log_debug(
              log, "overflow in x position for ", *p_to_position);
            p_to_position->unposition();
            return false;
        }
        if (p_to_position->get_y() < y(0)) {
            crosswords_log_debug(
              log, "underflow in y position for ", *p_to_position);
            p_to_position->unposition();
            return false;
        }
        if ((p_to_position->get_direction() == word::direction::vertical) &&
            (p_to_position->get_y() + y(p_to_position->get_lexeme().size()) >
             m_y_limit)) {
            crosswords_log_debug(
              log, "overflow in y position for ", *p_to_position);
            p_to_position->unposition();
            return false;
        }

        const lexeme& _lexeme = p_to_position->get_lexeme();
        lexeme::size_type _size = _lexeme.size();

        // check first and last letter

        // intersection is not at the begining
        if (p_intersection_idx != 0) {
            x _x0 = p_to_position->get_x();
            y _y0 = p_to_position->get_y();

            if (!is_valid_extreme(_x0, _y0, p_to_position->get_direction())) {
                crosswords_log_debug(
                  log,
                  "(",
                  _x0,
                  ",",
                  _y0,
                  "), with direction ",
                  word::direction2str(p_to_position->get_direction()),
                  " is not a valid extreme");
                return false;
            }

        } else if (p_intersection_idx != (_size - 1)) {
            // intersection is not at the end

            if (p_to_position->get_direction() == word::direction::horizontal) {
                x _x0 = p_to_position->get_x() + x(_size);
                y _y0 = p_to_position->get_y();

                if (!is_valid_extreme(_x0, _y0, word::direction::horizontal)) {
                    crosswords_log_debug(
                      log,
                      "(",
                      _x0,
                      ",",
                      _y0,
                      "), with direction ",
                      word::direction2str(word::direction::horizontal),
                      " is not a valid extreme");

                    return false;
                }
            } else {
                x _x0 = p_to_position->get_x();
                y _y0 = p_to_position->get_y() + y(_size);

                if (!is_valid_extreme(_x0, _y0, word::direction::vertical)) {
                    crosswords_log_debug(
                      log,
                      "(",
                      _x0,
                      ",",
                      _y0,
                      "), with direction ",
                      word::direction2str(word::direction::vertical),
                      " is not a valid extreme");
                    return false;
                }
            }
        }

        //        {
        //            x _x0 = p_to_position->get_x();
        //            y _y0 = p_to_position->get_y();

        //            // check all the letters, except the one that intersects
        //            if (p_to_position->get_direction() ==
        //            word::direction::horizontal) {
        //                for (lexeme::size_type _i = 0; _i < _size; ++_i) {
        //                    if (_i != p_intersection_idx) {
        //                        x _x = _x0 + x(_i);
        //                        std::pair<bool, char> _occupied =
        //                        is_occupied(_x, _y0); if ((_occupied.first) &&
        //                            (_occupied.second != _lexeme[_i])) {
        //                            crosswords_log_debug(log,
        //                                                 "(",
        //                                                 _x,
        //                                                 ",",
        //                                                 _y0,
        //                                                 ") is occupied with
        //                                                 ", _occupied.second);

        //                            return false;
        //                        }

        //                        _occupied = is_occupied(_x, _y0 - y(1));
        //                        if ((_y0 > y(0)) && (_occupied.first)) {
        //                            crosswords_log_debug(log,
        //                                                 "(",
        //                                                 _x,
        //                                                 ",",
        //                                                 _y0 - y(1),
        //                                                 ") is occupied with
        //                                                 ", _occupied.second);
        //                            return false;
        //                        }

        //                        _occupied = is_occupied(_x, _y0 + y(1));
        //                        if ((_y0 < (m_y_limit - y(1))) &&
        //                        (_occupied.first)) {
        //                            crosswords_log_debug(log,
        //                                                 "(",
        //                                                 _x,
        //                                                 ",",
        //                                                 m_y_limit - y(1),
        //                                                 ") is occupied with
        //                                                 ", _occupied.second);
        //                            return false;
        //                        }
        //                    }
        //                }
        //            } else {
        //                for (lexeme::size_type _i = 0; _i < _size; ++_i) {
        //                    if (_i != p_intersection_idx) {
        //                        y _y = _y0 + y(_i);
        //                        std::pair<bool, char> _occupied =
        //                        is_occupied(_x0, _y); if ((_occupied.first) &&
        //                            (_occupied.second != _lexeme[_i])) {
        //                            crosswords_log_debug(log,
        //                                                 "(",
        //                                                 _x0,
        //                                                 ",",
        //                                                 _y,
        //                                                 ") is occupied with
        //                                                 ", _occupied.second);
        //                            return false;
        //                        }

        //                        _occupied = is_occupied(_x0 - x(1), _y);
        //                        if ((_x0 > x(0)) && (_occupied.first)) {
        //                            crosswords_log_debug(log,
        //                                                 "(",
        //                                                 _x0 - x(1),
        //                                                 ",",
        //                                                 _y,
        //                                                 ") is occupied with
        //                                                 ", _occupied.second);
        //                            return false;
        //                        }

        //                        _occupied = is_occupied(_x0 + x(1), _y0);
        //                        if ((_x0 < (m_x_limit - x(1))) &&
        //                        (_occupied.first)) {
        //                            crosswords_log_debug(log,
        //                                                 "(",
        //                                                 _x0 + x(1),
        //                                                 ",",
        //                                                 _y0,
        //                                                 ") is occupied with
        //                                                 ", _occupied.second);

        //                            return false;
        //                        }
        //                    }
        //                }
        //            }
        //        }
        {}

        return true;
    }

    bool is_valid_extreme(x p_x, y p_y, word::direction p_direction) const
    {
        std::pair<bool, char> _occupied;
        if (p_direction == word::direction::horizontal) {
            if (p_x > x(0)) {
                _occupied = is_occupied(p_x - x(1), p_y);
                if (_occupied.first) {
                    crosswords_log_debug(log,
                                         "(",
                                         p_x - x(1),
                                         ",",
                                         p_y,
                                         ") is occupied with ",
                                         _occupied.second);
                    return false;
                }

                _occupied = is_occupied(p_x - x(1), p_y - y(1));
                if ((p_y > y(0)) && (_occupied.first)) {
                    crosswords_log_debug(log,
                                         "(",
                                         p_x - x(1),
                                         ",",
                                         p_y - y(1),
                                         ") is occupied with ",
                                         _occupied.second);

                    return false;
                }
                _occupied = is_occupied(p_x - x(1), p_y + y(1));
                if ((p_y < m_y_limit) && (_occupied.first)) {
                    crosswords_log_debug(log,
                                         "(",
                                         p_x - x(1),
                                         ",",
                                         p_y + y(1),
                                         ") is occupied with ",
                                         _occupied.second);
                    return false;
                }
            }
        } else {

            if (p_y > y(0)) {
                _occupied = is_occupied(p_x, p_y - y(1));
                if (_occupied.first) {
                    crosswords_log_debug(log,
                                         "(",
                                         p_x,
                                         ",",
                                         p_y - y(1),
                                         ") is occupied with ",
                                         _occupied.second);
                    return false;
                }

                _occupied = is_occupied(p_x - x(1), p_y - y(1));
                if ((p_x > x(0)) && (_occupied.first)) {
                    crosswords_log_debug(log,
                                         "(",
                                         p_x - x(1),
                                         ",",
                                         p_y - y(1),
                                         ") is occupied with ",
                                         _occupied.second);
                    return false;
                }
                _occupied = is_occupied(p_x + x(1), p_y - y(1));
                if ((p_x < m_x_limit) && (_occupied.first)) {
                    crosswords_log_debug(log,
                                         "(",
                                         p_x + x(1),
                                         ",",
                                         p_y - y(1),
                                         ") is occupied with ",
                                         _occupied.second);
                    return false;
                }
            }
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
        //        if (m_y_limit < y(m_x_limit)) {
        //            x _x(m_x_limit / x(2));
        //            y _y((m_y_limit / y(2)) -
        //                 y(p_ptr_word_to_position->get_lexeme().size() / 2));
        //            p_ptr_word_to_position->position(
        //              _x, _y, direction::vertical, orientation::forward);
        //        } else {
        //            x _x((m_x_limit / x(2)) -
        //                 x(p_ptr_word_to_position->get_lexeme().size() / 2));
        //            y _y(m_y_limit / y(2));
        //            p_ptr_word_to_position->position(
        //              _x, _y, direction::horizontal, orientation::forward);
        //        }

        if (m_y_limit < y(m_x_limit)) {
            //            x _x(m_x_limit / x(2));
            //            y _y((m_y_limit / y(2)) -
            //                 y(p_ptr_word_to_position->get_lexeme().size() /
            //                 2));
            p_ptr_word_to_position->position(
              x(0), y(0), direction::vertical, orientation::forward);
        } else {
            //            x _x((m_x_limit / x(2)) -
            //                 x(p_ptr_word_to_position->get_lexeme().size() /
            //                 2));
            //            y _y(m_y_limit / y(2));
            p_ptr_word_to_position->position(
              x(0), y(0), direction::horizontal, orientation::forward);
        }

        m_positioned.push_back(p_ptr_word_to_position);
        add_to_occupied(p_ptr_word_to_position);
    }

    void optimize_words()
    {
        x _min_x = std::numeric_limits<x>::max();
        y _min_y = std::numeric_limits<y>::max();

        {
            words::const_iterator _end = m_words.end();
            for (words::const_iterator _ite = m_words.begin(); _ite != _end;
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
        if ((_min_x > x(0)) || (_min_y > y(0))) {
            for (words::iterator _ite = m_words.begin(); _ite != _end; ++_ite) {
                _ite->position(_ite->get_x() - _min_x,
                               _ite->get_y() - _min_y,
                               _ite->get_direction(),
                               _ite->get_orientation());
            }
        }
    }

    void add_to_occupied(words::const_iterator p_word)
    {
        const lexeme& _lexeme = p_word->get_lexeme();
        x _x = p_word->get_x();
        y _y = p_word->get_y();
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

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
        optimize_words();
        return m_words;
    }

private:
    typedef std::list<typename words::const_iterator> words_iterators;

    typedef std::list<words_iterators> words_configurations;

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
        words_iterators _first;
        crosswords_log_debug(log, "creating first words history");
        for (words::const_iterator _ite = m_words.begin();
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
        words_iterators::const_iterator _end = m_current_configuration->end();
        words_iterators::const_iterator _ite = m_current_configuration->begin();

        int16_t _i = 0;
        for (; _ite != _end; ++_ite) {
            words::const_iterator _pos = *_ite;
            crosswords_log_debug(log, "\t", _i, ":", &(*_pos));
            ++_i;
        }
    }

    std::string words_ite2str(
            words_iterators::const_iterator p_ite) const
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
        words_iterators::const_iterator _current_words_cfg_ite =
                m_current_configuration->begin();

        while (true) {
            crosswords_log_debug(log,
                                 "starting loop, trying to word ",
                                 words_ite2str(_current_words_cfg_ite));
            if (positioned(_current_words_cfg_ite)) {
                crosswords_log_debug(log,
                                     words_ite2str(_current_words_cfg_ite),
                                     " was positioned ");
                if (m_positioned.size() == m_current_configuration->size()) {
                    crosswords_log_debug(log, "all positioned");
                    _is_positioned = true;
                    break;
                }
                ++_current_words_cfg_ite;
                crosswords_log_debug(log,
                                     "new current_history_ite = ",
                                     words_ite2str(_current_words_cfg_ite));
            } else {
                crosswords_log_debug(log,
                                     words_ite2str(_current_words_cfg_ite),
                                     " was not positioned");
                if ((*_current_words_cfg_ite)->get_id() ==
                        m_current_configuration->back()->get_id()) {
                    crosswords_log_debug(
                                log, "and it was the last, so we clear positioned list");
                    m_positioned.clear();
                }
                crosswords_log_debug(log, "updating history");
                update_words_configuration(_current_words_cfg_ite);
                crosswords_log_debug(log, "checking for cycle");
                if (cycle()) {
                    crosswords_log_debug(log, "CYCLE!!");
                    _is_positioned = false;
                    break;
                }
                define_current_words_configuration();
                _current_words_cfg_ite = m_current_configuration->begin();

                crosswords_log_debug(log,
                                     "new current history defined, ",
                                     words_ite2str(_current_words_cfg_ite),
                                     " and is the new current");
                words_iterators::size_type _size = m_positioned.size();

                crosswords_log_debug(
                            log, "moving to the right word in the history");
                for (words_iterators::size_type _counter = 0;
                     _counter < _size;
                     ++_counter) {
                    crosswords_log_debug(
                                log, "\t", words_ite2str(_current_words_cfg_ite));
                    ++_current_words_cfg_ite;
                }
            }
        }
        return _is_positioned;
    }

    void update_words_configuration(words_iterators::const_iterator p_ite)
    {
        words_iterators _words_iterators(*m_current_configuration);

        _words_iterators.remove(*p_ite);

        _words_iterators.push_back(*p_ite);

        m_words_configurations.push_back(std::move(_words_iterators));
    }

    bool positioned(words_iterators::const_iterator /*p_positon_iterator*/)
    {
        if (m_positioned.empty()) {
            position_first();
            return true;
        }
        return false;
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

    bool equals(const words_iterators& p_ite1,
                const words_iterators& p_ite2)
    {
        words_iterators::const_iterator _end1 = p_ite1.end();
        words_iterators::const_iterator _ite1 = p_ite1.begin();

        words_iterators::const_iterator _end2 = p_ite2.end();
        words_iterators::const_iterator _ite2 = p_ite2.begin();

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

    void position_first()
    {
        typename words::iterator _begin = m_words.begin();
        if (m_vertical < m_horizontal) {
            limit x = static_cast<limit>(m_horizontal / 2);
            limit y = static_cast<limit>(m_vertical / 2) -
                    static_cast<limit>(_begin->get_lexeme().size() / 2);
            _begin->position(x, y, direction::vertical, orientation::forward);
        } else {
            limit x = static_cast<limit>(m_horizontal / 2) -
                    static_cast<limit>(_begin->get_lexeme().size() / 2);
            limit y = static_cast<limit>(m_vertical / 2);
            _begin->position(x, y, direction::horizontal, orientation::forward);
        }
        m_positioned.push_back(_begin);
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

private:
    limit m_vertical;
    limit m_horizontal;

    words m_words;
    words_configurations m_words_configurations;
    words_iterators m_positioned;
    words_configurations::const_iterator m_current_configuration;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER_H

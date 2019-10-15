#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONER004_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONER004_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/word_positioner.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief positioner004_t position \p words
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
struct positioner004_t
{
    typedef t_log log;

    typedef entities::words words;
    typedef entities::word word;
    typedef entities::lexeme lexeme;
    typedef entities::description description;
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;

    typedef word_positioner_t<log> word_positioner;

    explicit positioner004_t(x p_x_limit = x(13), y p_y_limit = y(13))
        : m_x_limit(p_x_limit)
        , m_y_limit(p_y_limit)
    {}

    void add(lexeme&& p_lexeme, description&& p_description)
    {
        lexeme::size_type _word_size = p_lexeme.size();
        if ((m_y_limit < y(_word_size)) && (m_x_limit < x(_word_size))) {

            if (m_y_limit < y(_word_size)) {
                std::stringstream _stream;
                _stream << p_lexeme << "'s size is " << _word_size
                        << ", which is bigger than the vertical limit of "
                        << m_y_limit;
                std::string _msg = _stream.str();

                crosswords_log_error(log, _stream.str());
                throw std::runtime_error(_msg);
            }
            if (m_x_limit < x(_word_size)) {
                std::string _msg(
                            p_lexeme + "'s size is " + std::to_string(_word_size) +
                            ", which is bigger than the horizontal "
                            "limit of " +
                            std::to_string(m_x_limit.get_value<int16_t>()));
                crosswords_log_error(log, _msg);
                throw std::runtime_error(_msg);
            }
        }
        crosswords_log_debug(log,
                             "'",
                             p_lexeme,
                             "', with description '",
                             p_description,
                             "' was added");
        m_words.add(word(static_cast<word::id>(m_words.get_size() + 1),
                         p_lexeme,
                         p_description));
    }


    words operator()()
    {
        m_words.sort();
        word_positioner _word_positioner;
        uint32_t _counter = 0;
        crosswords_log_info(log, "STARTING ", std::time(nullptr));

        words::iterator _begin = m_words.begin();
        words::iterator _end = m_words.end();
        words::iterator _ite = _begin;

        positioning _positioning = positioning::positioned;

        while (true) {
            if (_ite == _end) {
                break;
            }

            _positioning = position_set(_begin, _ite);
            if (_positioning == positioning::positioned) {
                ++_ite;
            }
            else {

            }

        }
        crosswords_log_info(log, "FINISHING ", std::time(nullptr));
        return m_words;
    }



private:
    typedef std::list<word::id> words_ids;
    typedef std::list<words_ids> failures;

    enum class positioning : char {
        positioned = 'p',
        no_more_partions = 'm'
    };

    struct update_set{
        void operator()(words::iterator p_begin, words::iterator &p_end) {

        }

    };

private:

    positioning position_set(words::iterator p_begin,
                      words::iterator p_ite) {
        word_positioner _word_positioner;

        bool _no_more_permutations = false;

        while (true) {
            crosswords_log_info(log,
                                "############ ",
                                print_words(p_begin, p_ite));

            if (_word_positioner(p_begin, p_ite, m_x_limit, m_y_limit)) {
                m_words.print_positioned(m_x_limit, m_y_limit);
                crosswords_log_info(log, *p_ite, " was positioned");
                break;
            }
            crosswords_log_debug(log, p_ite->get_lexeme(),
                                 " was NOT positioned");

            if (!std::next_permutation(
                        p_begin, std::next(p_ite), words::cmp_words())) {
                crosswords_log_warn(log, "no more permutations");
                _no_more_permutations=true;
                break;
            }
            p_ite = p_begin;
        }
        return (_no_more_permutations ?
                    positioning::no_more_partitions :
                    positioning::positioned);
    }

    bool already_tried(words::const_iterator p_begin,
                       words::const_iterator p_end)
    {
        for (words::const_iterator _word_ite = p_begin; _word_ite != p_end; ++_word_ite) {
            words_ids::size_type _size =
                    static_cast<words_ids::size_type>(std::distance(p_begin, _word_ite));
            bool _match = true;
            for (const words_ids & _words_ids : m_failures) {
                if (_words_ids.size() == _size) {
                    words_ids::const_iterator _ids_ends = _words_ids.end();
                    words::const_iterator _word_aux = p_begin;
                    for (words_ids::const_iterator _ids_ite = _words_ids.begin();
                         _ids_ite != _ids_ends; ++_ids_ite) {
                        if ((*_ids_ite) != _word_aux->get_id()) {
                            _match=false;
                            break;
                        }
                    }
                    if (_match) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    //    bool already_tried(words::const_iterator p_begin,
    //                       words::const_iterator p_end)
    //    {
    //        for (const words_ids& _words_ids : m_failures) {
    //            if (_words_ids.size() == static_cast<words_ids::size_type>(
    //                                       std::distance(p_begin, p_end))) {
    //                words_ids::const_iterator _ids_end = _words_ids.end();
    //                bool _match = true;
    //                words::const_iterator _ite = p_begin;
    //                for (words_ids::const_iterator _ids_ite = _words_ids.begin();
    //                     _ids_ite != _ids_end;
    //                     ++_ids_ite) {
    //                    if (_ite->get_id() != *_ids_ite) {
    //                        _match = false;
    //                        break;
    //                    }
    //                }
    //                if (_match) {
    //                    return true;
    //                }
    //            }
    //        }
    //        return false;
    //    }
    void add_failure(words::const_iterator p_begin, words::const_iterator p_end)
    {
        words_ids _words_ids;
        for (words::const_iterator _ite = p_begin; _ite != p_end; ++_ite) {
            _words_ids.push_back(_ite->get_id());
        }
        m_failures.push_back(std::move(_words_ids));
    }
    void unposition(words::iterator p_begin, words::iterator p_end)
    {
        for (words::iterator _ite = p_begin; _ite != p_end; ++_ite) {
            _ite->unposition();
        }
    }
    std::string print_words(words::const_iterator p_begin,
                            words::const_iterator p_last)
    {
        words::const_iterator _end = std::next(p_last);
        std::stringstream _stream;
        for (words::const_iterator _ite = p_begin; _ite != _end; ++_ite) {
            _stream << _ite->get_lexeme() << " ";
        }
        return _stream.str();
    }

private:
    x m_x_limit;
    y m_y_limit;
    words m_words;
    failures m_failures;
};
} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONER003_H

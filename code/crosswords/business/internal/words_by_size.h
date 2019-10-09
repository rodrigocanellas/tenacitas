#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_BY_SIZE_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_BY_SIZE_H

#include <functional>
#include <list>
#include <string>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/positions_occupied.h>
#include <crosswords/business/internal/position_word.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>


namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief words_by_size groups \p word by size
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
struct words_by_size_t
{
    typedef t_log log;

    typedef entities::words words;
    typedef entities::word word;
    typedef entities::lexeme::size_type size;
    typedef entities::coordinate::x x;
    typedef entities::coordinate::y y;

    typedef positions_occupied_t<log> positions_occupied;
    typedef position_word_t<log> position_word;

    words_by_size_t() = delete;

    explicit words_by_size_t(size p_size)
        : m_size(p_size)
        , m_limits_defined(false)
    {}

    words_by_size_t(const words_by_size_t&) = default;
    words_by_size_t(words_by_size_t&&) noexcept = default;
    words_by_size_t& operator=(const words_by_size_t&) = default;
    words_by_size_t& operator=(words_by_size_t&&) noexcept = default;

    ~words_by_size_t() = default;

    inline size get_size() const { return m_size; }

    bool position(words & p_words,
                  x p_x_limit,
                  y p_y_limit,
                  positions_occupied& p_positions_occupied)
    {
        if (!m_limits_defined) {
            define_limits(p_words);
        }
        position_word _position_word;
        for (words::iterator _ite = m_begin; _ite != m_end; ++_ite) {
            if (!_position_word(p_words, _ite, p_x_limit, p_y_limit,
                                p_positions_occupied)) {
                // for now, we give up
                return false;
            }
        }
        return true;
    }

    bool operator==(const words_by_size_t& p_words_by_size) const
    {
        return m_size == p_words_by_size.m_size;
    }

    bool operator!=(const words_by_size_t& p_words_by_size) const
    {
        return m_size != p_words_by_size.m_size;
    }

    bool operator<(const words_by_size_t& p_words_by_size) const
    {
        return m_size < p_words_by_size.m_size;
    }

    bool operator>(const words_by_size_t& p_words_by_size) const
    {
        return m_size > p_words_by_size.m_size;
    }
private:
    void define_limits(words & p_words) {
        words::iterator _end = p_words.end();
        m_begin = _end;
        m_end = _end;
        m_limits_defined= true;
        for (words::iterator _ite1 = p_words.begin();
             _ite1 != _end;
             ++_ite1) {
            if (_ite1->get_size() == m_size) {
                m_begin = _ite1;
                for (m_end = m_begin; m_end != _end; ++m_end) {
                    if (m_end->get_size() < m_size) {
                        break;
                    }
                }
                break;
            }
        }

    }
private:
    size m_size;
    bool m_limits_defined;
    words::iterator m_begin;
    words::iterator m_end;


};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORDS_BY_SIZE_H

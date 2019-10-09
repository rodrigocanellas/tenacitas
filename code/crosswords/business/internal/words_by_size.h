#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_BY_SIZE_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_BY_SIZE_H

#include <functional>
#include <list>
#include <string>

#include <crosswords/business/internal/log.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/description.h>
#include <crosswords/entities/lexeme.h>
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
    typedef entities::words::iterator words_iterator;
    typedef entities::words::const_iterator word_const_iterator;
    typedef entities::lexeme::size_type size;

    words_by_size_t() = delete;

    explicit words_by_size_t(size p_size)
      : m_size(p_size)
    {}

    words_by_size_t(const words_by_size_t&) = default;
    words_by_size_t(words_by_size_t&&) noexcept = default;
    words_by_size_t& operator=(const words_by_size_t&) = default;
    words_by_size_t& operator=(words_by_size_t&&) noexcept = default;

    ~words_by_size_t() = default;

    inline size get_size() const { return m_size; }

    bool position(words_iterator /*p_all_begin*/, words_iterator /*p_all_end*/)
    {}

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
    words_iterator m_begin;
    words_iterator m_end;
    size m_size;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORDS_BY_SIZE_H

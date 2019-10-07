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
    typedef entities::words::iterator word_pointer;
    typedef entities::words::const_iterator word_const_pointer;
    typedef entities::lexeme::size_type size;

    words_by_size_t() = delete;

    explicit words_by_size_t(size p_size)
      : m_size(p_size)
      , m_words_references()
    {}

    words_by_size_t(const words_by_size_t&) = default;
    words_by_size_t(words_by_size_t&&) noexcept = default;
    words_by_size_t& operator=(const words_by_size_t&) = default;
    words_by_size_t& operator=(words_by_size_t&&) noexcept = default;

    ~words_by_size_t() = default;

    inline void traverse(
      std::function<void(word_pointer p_word_pointer)> p_function)
    {
        for (word_pointer _word_pointer : m_words_references) {
            p_function(_word_pointer);
        }
    }

    inline void traverse(
      std::function<void(word_const_pointer p_word_pointer)> p_function) const
    {
        for (word_const_pointer _word_pointer : m_words_references) {
            p_function(_word_pointer);
        }
    }

    void add(word_pointer p_word_pointer)
    {
        if (p_word_pointer->get_size() != m_size) {
            throw std::runtime_error(
              std::to_string(p_word_pointer->get_size()) + " is not " +
              std::to_string(m_size));
        }
        m_words_references.push_back(p_word_pointer);
    }

    inline size get_size() const { return m_size; }

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
    typedef std::list<word_pointer> words_references;

  private:
    size m_size;
    words_references m_words_references;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // WORDS_BY_SIZE_H

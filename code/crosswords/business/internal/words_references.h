#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_REFERENCES_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_REFERENCES_H

#include <functional>
#include <iostream>
#include <list>

#include <crosswords/business/internal/log.h>
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
struct words_references_t
{
    typedef entities::words words;
    typedef entities::word word;
    typedef std::list<words::iterator> list;
    typedef list::size_type size;
    typedef list::const_iterator const_iterator;
    typedef list::iterator iterator;
    typedef t_log log;

    friend std::ostream& operator<<(std::ostream& p_out,
                                    const words_references_t& p_wr)
    {
        if (p_wr.empty()) {
            crosswords_log_debug(log, "words_references is empty");
        }
        const_iterator _end = p_wr.end();
        const_iterator _ite = p_wr.begin();

        for (; _ite != _end; ++_ite) {
            words::const_iterator _pos = *_ite;
            crosswords_log_debug(log, &(*_pos), ":", *_pos);
        }

        return p_out;
    }

    explicit words_references_t(words& p_words)
    {
        for (words::iterator _ite = p_words.begin(); _ite != p_words.end();
             ++_ite) {
            m_list.push_back(_ite);
        }
    }

    words_references_t() = default;

    words_references_t(const words_references_t&) = default;
    words_references_t(words_references_t&&) noexcept = default;

    words_references_t& operator=(const words_references_t&) = default;
    words_references_t& operator=(words_references_t&&) noexcept = default;

    ~words_references_t() = default;

    size get_size() const { return m_list.size(); }
    bool empty() const { return m_list.empty(); }

    void add(words::iterator p_ite) { m_list.push_back(p_ite); }
    void clear() { m_list.clear(); }
    void remove(const_iterator p_ite) { m_list.remove(p_ite); }

    void move_to_end(const_iterator p_ite)
    {
        remove(*p_ite);
        add(*p_ite);
    }

    bool operator==(const words_references_t& p_wr) const
    {
        const_iterator _end1 = this->end();
        const_iterator _ite1 = this->begin();

        const_iterator _end2 = p_wr.end();
        const_iterator _ite2 = p_wr.begin();

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

    bool operator!=(const words_references_t& p_wr) const
    {
        return !(*this == p_wr);
    }

    const_iterator advance(size p_steps) const
    {
        size _size = get_size();
        if (p_steps > _size) {
            return m_list.end();
        }
        const_iterator _ite = m_list.begin();
        for (size _counter = 0; _counter < get_size(); ++_counter) {
            ++_ite;
        }
        return _ite;
    }

    iterator advance(size p_steps)
    {
        size _size = get_size();
        if (p_steps > _size) {
            return m_list.end();
        }
        iterator _ite = m_list.begin();
        for (size _counter = 0; _counter < get_size(); ++_counter) {
            ++_ite;
        }
        return _ite;
    }

    void traverse(std::function<void(const word& p_word)> p_function)
    {
        const_iterator _end = m_list.end();
        for (const_iterator _ite = m_list.begin(); _ite != _end; ++_ite) {
            p_function(*(*_ite));
        }
    }

    void traverse(std::function<void(words::iterator p_iterator)> p_function)
    {
        const_iterator _end = m_list.end();
        for (const_iterator _ite = m_list.begin(); _ite != _end; ++_ite) {
            p_function(*_ite);
        }
    }

  private:
    iterator begin() { return m_list.begin(); }
    iterator end() { return m_list.end(); }
    const_iterator begin() const { return m_list.begin(); }
    const_iterator end() const { return m_list.end(); }

  private:
    list m_list;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // words_references_t_H

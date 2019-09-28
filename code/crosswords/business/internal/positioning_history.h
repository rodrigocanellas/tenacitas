#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONING_HISTORY_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONING_HISTORY_H

#include <functional>
#include <iostream>
#include <list>

#include <crosswords/business/internal/log.h>
#include <crosswords/business/internal/words_references.h>
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
struct positioning_history_t
{
    typedef t_log log;
    typedef words_references_t<log> words_references;

    friend std::iostream& operator<<(std::iostream& p_out,
                                     const positioning_history_t& p_ph)
    {
        const_iterator _end = p_ph.end();
        for (const_iterator _ite = p_ph.begin(); _ite != _end; ++_ite) {
            crosswords_log_debug(log, *_ite);
        }
        return p_out;
    }

    void log_current() const { crosswords_log_debug(log, *m_current); }

    void update(typename words_references::const_iterator p_wr_ite)
    {
        words_references _wr(*m_current);
        _wr.move_to_end(p_wr_ite);
        add(std::move(_wr));
    }

    void add(words_references&& p_wr)
    {
        m_list.push_back(std::move(p_wr));
        m_current = m_list.back();
    }

    bool cycle() const
    {
        if (m_list.size() <= 1) {
            return false;
        }

        const_iterator _last = m_list.end();
        --_last;
        const_iterator _ite = m_list.begin();
        while (true) {
            if (_ite == _last) {
                break;
            }
            if (*_ite != *_last) {
                return false;
            }
            ++_ite;
        }
        return true;
    }

  private:
    typedef std::list<words_references> list;
    typedef typename list::const_iterator const_iterator;
    typedef typename list::iterator iterator;

  private:
    const_iterator end() const { return m_list.end(); }
    const_iterator begin() const { return m_list.begin(); }
    iterator end() { return m_list.end(); }
    iterator begin() { return m_list.begin(); }

  private:
    list m_list;
    typename words_references::const_iterator m_current;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONING_HISTORY_H

#ifndef TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONS_OCCUPIED_H
#define TENACITAS_CROSSWORDS_BUSINESS_WORDS_POSITIONS_OCCUPIED_H

#include <map>
#include <utility>

#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/word.h>

namespace tenacitas {
namespace crosswords {
namespace business {

/// \brief keeps track of positioned that are occupied
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
struct positions_occupied_t
{
    typedef t_log log;
    typedef entities::coordinate coordinate;
    typedef entities::word word;

    void add(const word& p_word)
    {
        p_word.traverse([this](const coordinate& p_coord, char p_char) -> void {
            this->m_map[p_coord] = p_char;
        });
    }

    std::pair<bool, char> find(const coordinate& p_coord) const
    {
        map::const_iterator _ite = m_map.find(p_coord);
        if (_ite == m_map.end()) {
            return { false, ' ' };
        }
        return { true, _ite->second };
    }

    std::pair<bool, char> find(const coordinate::x& p_x, const coordinate::y &p_y) const
    {
        map::const_iterator _ite = m_map.find(coordinate(p_x, p_y);
        if (_ite == m_map.end()) {
            return { false, ' ' };
        }
        return { true, _ite->second };
    }

  private:
    typedef std::map<coordinate, char> map;

  private:
    map m_map;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONS_OCCUPIED_H

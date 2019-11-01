#ifndef TENACITAS_BUSINESS_CROSSWORDS_WORDS_POSITIONS_OCCUPIED_H
#define TENACITAS_BUSINESS_CROSSWORDS_WORDS_POSITIONS_OCCUPIED_H

#include <map>
#include <utility>

#include <entities/crosswords/coordinate.h>
#include <entities/crosswords/word.h>

namespace tenacitas {
namespace business {
namespace crosswords {

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
    typedef entities::crosswords::coordinate coordinate;
    typedef entities::crosswords::word word;

    void add(const word& p_word)
    {
        p_word.traverse([this](const coordinate& p_coord, char p_char) -> void {
            pair _pair = { ' ', 0 };
            _pair = this->m_map[p_coord];

            if (_pair.first == ' ') {
                this->m_map[p_coord] = { p_char, 1 };
            } else {
                this->m_map[p_coord] = { p_char, ++_pair.second };
            }
        });
    }

    void remove(const word& p_word)
    {
        p_word.traverse(
          [this](const coordinate& p_coord, char /*p_char*/) -> void {
              pair _pair = { ' ', 0 };
              _pair = this->m_map[p_coord];

              if (_pair.second == 1) {
                  this->m_map[p_coord] = { ' ', 0 };
              } else {
                  int8_t _value = _pair.second;
                  --_value;
                  this->m_map[p_coord] = { _pair.first, _value };
              }
          });
    }

    std::pair<bool, char> find(const coordinate& p_coord) const
    {
        map::const_iterator _ite = m_map.find(p_coord);
        if (_ite == m_map.end()) {
            return { false, ' ' };
        }
        return { true, _ite->second.first };
    }

    std::pair<bool, char> find(const coordinate::x& p_x,
                               const coordinate::y& p_y) const
    {
        map::const_iterator _ite = m_map.find(coordinate(p_x, p_y));
        if ((_ite == m_map.end()) || (_ite->second.first == ' ')) {
            return { false, ' ' };
        }
        return { true, _ite->second.first };
    }

    void clear() { m_map.clear(); }

    void print() const
    {
        map::const_iterator _end = m_map.end();
        for (map::const_iterator _ite = m_map.begin(); _ite != _end; ++_ite) {
            std::cerr << "[(" << _ite->first.get_x() << ","
                      << _ite->first.get_y() << ");" << _ite->second.first
                      << ";" << static_cast<uint16_t>(_ite->second.second)
                      << "]  ";
        }
    }

  private:
    typedef std::pair<char, int8_t> pair;
    typedef std::map<coordinate, pair> map;

  private:
    map m_map;
};

} // namespace business
} // namespace crosswords
} // namespace tenacitas

#endif // POSITIONS_OCCUPIED_H

#ifndef TENACITAS_ENTITIES_CROSSWORDS_BOARD_H
#define TENACITAS_ENTITIES_CROSSWORDS_BOARD_H

#include <algorithm>
#include <cstdint>
#include <list>
#include <tuple>

#include <entities/crosswords/description.h>
#include <entities/crosswords/lexeme.h>
#include <entities/crosswords/limit.h>
#include <entities/crosswords/word.h>
#include <entities/crosswords/words.h>

namespace tenacitas {
namespace entities {
namespace crosswords {

template<typename t_positioner>
struct board_t
{

    board_t() = delete;
    explicit board_t(t_positioner&& p_positioner)
      : m_positions(std::move(p_positioner()))
    {}

    board_t(const board_t&) = delete;
    board_t(board_t&&) noexcept = default;
    board_t& operator=(const board_t&) = delete;
    board_t& operator=(board_t&&) noexcept = default;
    ~board_t() = default;

    inline words::const_iterator begin() const { return m_positions.begin(); }
    inline words::const_iterator end() const { return m_positions.end(); }

    inline bool test_answer(word::id p_id, const std::string& p_value)
    {
        words::iterator _end = m_positions.end();
        words::iterator _begin = m_positions.begin();
        words::iterator _ite =
          std::find_if(_begin, _end, [&p_id](const word& p_pos) -> bool {
              return (p_pos.get_id() == p_id);
          });

        if ((_ite != _end) && (_ite->get_lexeme() == p_value)) {
            _ite->set_answer(p_value);
            return true;
        }
        return false;
    }

  private:
    words m_positions;
};

} // namespace crosswords
} // namespace entities
} // namespace tenacitas

#endif // BOARD_H

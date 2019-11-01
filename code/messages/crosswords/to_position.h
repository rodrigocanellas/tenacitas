#ifndef TENACITAS_CROSSWORDS_BUSINESS_TO_POSITION_H
#define TENACITAS_CROSSWORDS_BUSINESS_TO_POSITION_H

#include <business/crosswords/internal/positions_occupied.h>
#include <entities/crosswords/words.h>


namespace tenacitas {
namespace messages {
namespace crosswords {

template <typename t_log>
struct to_position
{
  typedef t_log log;
  typedef business::crosswords::positions_occupied_t<log> positions_occupied;

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const to_position& p_to_position)
  {
    p_out << "to_positon msg: "
          << entities::crosswords::print_words(p_to_position.m_words.begin(),
                                   p_to_position.m_words.end());
    return p_out;
  }

  to_position() = default;
  inline explicit to_position(const entities::crosswords::words& p_words,
  const positions_occupied & p_positions_occupied)
    : m_words(p_words),
      m_positions_occupied(p_positions_occupied)
  {}

  inline to_position(entities::crosswords::words::const_iterator p_begin,
                     entities::crosswords::words::const_iterator& p_end,
                     const positions_occupied & p_positions_occupied)
    : m_words(p_begin, p_end),
      m_positions_occupied(p_positions_occupied)
  {}

  to_position(to_position&&) noexcept = default;
  to_position(const to_position&) = default;

  to_position& operator=(to_position&&) noexcept = default;
  to_position& operator=(const to_position&) = default;

  ~to_position() = default;

  inline entities::crosswords::words get_words() const { return m_words; }
  inline positions_occupied get_positions_occupied(){return m_positions_occupied;}

private:
  entities::crosswords::words m_words;
  positions_occupied m_positions_occupied;
};

} // namespace crosswords
} // namespace messages
} // namespace tenacitas

#endif // TO_POSITION_H

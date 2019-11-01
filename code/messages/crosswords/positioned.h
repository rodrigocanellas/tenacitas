#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONED_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONED_H

#include <entities/crosswords/words.h>
#include <business/crosswords/internal/positions_occupied.h>

namespace tenacitas {
namespace messages {
namespace crosswords {

template <typename t_log>
struct positioned   
{

  typedef t_log log;
  typedef business::crosswords::positions_occupied_t<log> positions_occupied;

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const positioned& p_positioned)
  {
    p_out << "positioned msg: "
          << entities::crosswords::print_words(p_positioned.m_words.begin(),
                                   p_positioned.m_words.end());
    return p_out;
  }

  positioned() = default;
  inline explicit positioned(const entities::crosswords::words& p_words,
                             const positions_occupied & p_positions_occupied)
    : m_words(p_words),
      m_positions_occupied(p_positions_occupied)

  {}
  inline positioned(entities::crosswords::words::const_iterator p_begin,
                    entities::crosswords::words::const_iterator& p_end,
                    const positions_occupied & p_positions_occupied)
    : m_words(p_begin, p_end),
      m_positions_occupied(p_positions_occupied)
  {}

  positioned(positioned&&) noexcept = default;
  positioned(const positioned&) = default;

  positioned& operator=(positioned&&) noexcept = default;
  positioned& operator=(const positioned&) = default;

  ~positioned() = default;

  inline entities::crosswords::words get_words() const { return m_words; }
  inline positions_occupied get_positions_occupied(){return m_positions_occupied;}
private:
  entities::crosswords::words m_words;
  positions_occupied m_positions_occupied;
};

} // namespace crosswords
} // namespace messages
} // namespace tenacitas

#endif // positioned_H

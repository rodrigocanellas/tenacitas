#ifndef TENACITAS_CROSSWORDS_BUSINESS_TO_POSITION_H
#define TENACITAS_CROSSWORDS_BUSINESS_TO_POSITION_H

#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace messages {

struct to_position
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const to_position& p_to_position)
  {
    p_out << "to_positon msg: "
          << entities::print_words(p_to_position.m_words.begin(),
                                   p_to_position.m_words.end());
    return p_out;
  }

  to_position() = default;
  inline explicit to_position(const entities::words& p_words)
    : m_words(p_words)
  {}

  inline to_position(entities::words::const_iterator p_begin,
                     entities::words::const_iterator& p_end)
    : m_words(p_begin, p_end)
  {}

  to_position(to_position&&) noexcept = default;
  to_position(const to_position&) = default;

  to_position& operator=(to_position&&) noexcept = default;
  to_position& operator=(const to_position&) = default;

  ~to_position() = default;

  inline entities::words get_words() const { return m_words; }

private:
  entities::words m_words;
};

} // namespace messages
} // namespace crosswords
} // namespace tenacitas

#endif // TO_POSITION_H

#ifndef TENACITAS_CROSSWORDS_BUSINESS_NOT_POSITIONED_H
#define TENACITAS_CROSSWORDS_BUSINESS_NOT_POSITIONED_H

#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace messages {

struct not_positioned
{
  friend std::ostream& operator<<(std::ostream& p_out,
                                  const not_positioned& p_not_positioned)
  {
    p_out << "not_positioned msg: "
          << entities::print_words(p_not_positioned.m_words.begin(),
                                   p_not_positioned.m_words.end());
    return p_out;
  }

  not_positioned() = default;
  inline explicit not_positioned(const entities::words& p_words)
    : m_words(p_words)
  {}
  inline not_positioned(entities::words::const_iterator p_begin,
                        entities::words::const_iterator& p_end)
    : m_words(p_begin, p_end)
  {}

  not_positioned(not_positioned&&) noexcept = default;
  not_positioned(const not_positioned&) = default;

  not_positioned& operator=(not_positioned&&) noexcept = default;
  not_positioned& operator=(const not_positioned&) = default;

  ~not_positioned() = default;

  inline entities::words get_words() const { return m_words; }

private:
  entities::words m_words;
};

} // namespace messages
} // namespace crosswords
} // namespace tenacitas

#endif // not_position_H

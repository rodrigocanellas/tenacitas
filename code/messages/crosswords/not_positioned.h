#ifndef TENACITAS_CROSSWORDS_BUSINESS_NOT_POSITIONED_H
#define TENACITAS_CROSSWORDS_BUSINESS_NOT_POSITIONED_H

#include <entities/crosswords/words.h>

namespace tenacitas {
namespace messages {
namespace crosswords {

struct not_positioned
{
  friend std::ostream& operator<<(std::ostream& p_out,
                                  const not_positioned& p_not_positioned)
  {
    p_out << "not_positioned msg: "
          << entities::crosswords::print_words(p_not_positioned.m_words.begin(),
                                   p_not_positioned.m_words.end());
    return p_out;
  }

  not_positioned() = default;
  inline explicit not_positioned(const entities::crosswords::words& p_words)
    : m_words(p_words)
  {}
  inline not_positioned(entities::crosswords::words::const_iterator p_begin,
                        entities::crosswords::words::const_iterator& p_end)
    : m_words(p_begin, p_end)
  {}

  not_positioned(not_positioned&&) noexcept = default;
  not_positioned(const not_positioned&) = default;

  not_positioned& operator=(not_positioned&&) noexcept = default;
  not_positioned& operator=(const not_positioned&) = default;

  ~not_positioned() = default;

  inline entities::crosswords::words get_words() const { return m_words; }

private:
  entities::crosswords::words m_words;
};

} // namespace crosswords
} // namespace messages
} // namespace tenacitas

#endif // not_position_H

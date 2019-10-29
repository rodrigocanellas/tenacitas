#ifndef TENACITAS_CROSSWORDS_BUSINESS_STOP_POSITIONING_H
#define TENACITAS_CROSSWORDS_BUSINESS_STOP_POSITIONING_H

#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace messages {

struct stop_positioning
{
  friend std::ostream& operator<<(std::ostream& p_out,
                                  const stop_positioning& p_stop_positioning)
  {
    p_out << "stop_positioning msg: "
          << entities::print_words(p_stop_positioning.m_words.begin(),
                                   p_stop_positioning.m_words.end());
    return p_out;
  }

  stop_positioning() = default;
  inline explicit stop_positioning(const entities::words& p_words)
    : m_words(p_words)
  {}
  inline stop_positioning(entities::words::const_iterator p_begin,
                        entities::words::const_iterator& p_end)
    : m_words(p_begin, p_end)
  {}

  stop_positioning(stop_positioning&&) noexcept = default;
  stop_positioning(const stop_positioning&) = default;

  stop_positioning& operator=(stop_positioning&&) noexcept = default;
  stop_positioning& operator=(const stop_positioning&) = default;

  ~stop_positioning() = default;

  inline entities::words get_words() const { return m_words; }

private:
  entities::words m_words;
};

} // namespace messages
} // namespace crosswords
} // namespace tenacitas

#endif // not_position_H

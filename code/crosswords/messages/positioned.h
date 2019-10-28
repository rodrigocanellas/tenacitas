#ifndef TENACITAS_CROSSWORDS_BUSINESS_POSITIONED_H
#define TENACITAS_CROSSWORDS_BUSINESS_POSITIONED_H

#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace messages {

struct positioned {
  friend std::ostream& operator<<(std::ostream& p_out, const positioned& p_positioned)
  {
    entities::print_words(p_positioned.m_words.begin(),
                          p_positioned.m_words.end());
    return p_out;
  }

  positioned ()=default;
  inline explicit positioned(const entities::words & p_words)
    : m_words(p_words) {}
  inline positioned(entities::words::const_iterator p_begin,
              entities::words::const_iterator & p_end)
    : m_words(p_begin, p_end) {}

  positioned(positioned&&)noexcept=default;
  positioned(const positioned&)=default;

  positioned&operator=(positioned&&)noexcept=default;
  positioned&operator=(const positioned&)=default;

  ~positioned()=default;

  inline entities::words get_words()const{
    return m_words;
  }

private:
  entities::words m_words;
};

} // namespace messages
} // namespace crosswords
} // namespace tenacitas

#endif // positioned_H

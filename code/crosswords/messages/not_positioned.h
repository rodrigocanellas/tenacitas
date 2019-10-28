#ifndef TENACITAS_CROSSWORDS_BUSINESS_NOT_POSITIONED_H
#define TENACITAS_CROSSWORDS_BUSINESS_NOT_POSITIONED_H

#include <crosswords/entities/words.h>

namespace tenacitas {
namespace crosswords {
namespace messages {

struct not_positioned {
  friend std::ostream& operator<<(std::ostream& p_out, const not_positioned& /*p_not_positioned*/)
  {
    p_out << "not positioned";
    return p_out;
  }

};

} // namespace messages
} // namespace crosswords
} // namespace tenacitas

#endif // not_position_H

#ifndef SPLIT_STR_H
#define SPLIT_STR_H

#include <functional>
#include <string>

namespace tenacitas {
namespace string {
namespace business {

void
slipt_str(const std::string& p_values,
          std::function<void(std::string&&)> p_handler,
          char p_separator = ' ');

} // namespace business
} // namespace string
} // namespace tenacitas

#endif // SPLIT_STR_H

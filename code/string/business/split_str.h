#ifndef SPLIT_STR_H
#define SPLIT_STR_H

#include <functional>
#include <string>

namespace tenacitas {
namespace string {
namespace business {

///
/// \brief slipt_str splits strings from another string, provided a separator
/// char \param p_values p_values is the string that contains other strings
/// \param p_handler p_handler is the function (object) that will handle each
/// string found in @p p_values \param p_separator p_separator is the char that
/// separates the strings inside @p p_values
///
void
slipt_str(const std::string& p_values,
          std::function<void(std::string&&)> p_handler,
          char p_separator = ' ');

} // namespace business
} // namespace string
} // namespace tenacitas

#endif // SPLIT_STR_H

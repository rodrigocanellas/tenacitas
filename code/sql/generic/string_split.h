#ifndef CAPEMISA_SQL_GENERIC_STRING_SPLIT_H
#define CAPEMISA_SQL_GENERIC_STRING_SPLIT_H

#include <functional>
#include <string>

namespace capemisa {
namespace sql {
namespace generic {

struct string_split
{
  void operator()(const std::string& p_str,
                  std::function<void(std::string&&)> p_split)
  {
    std::string::size_type _last = 0;
    std::string::size_type _next = 0;

    while ((_next = p_str.find(';', _last)) != std::string::npos) {
      p_split(p_str.substr(_last, _next - _last));
      _last = _next + 1;
    }
    p_split(p_str.substr(_last));
  }
};

} // namespace generic
} // namespace sql
} // namespace capemisa

#endif // STRING_SPLIT_H

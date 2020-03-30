#ifndef TENACITAS_STRING_BUSINESS_FILE_NAME_FROM_PATH_H
#define TENACITAS_STRING_BUSINESS_FILE_NAME_FROM_PATH_H

#include <cstring>

namespace tenacitas {
namespace formater {
namespace bus {

const char*
file_name_from_path(const char* p_path)
{
  const char* _p = p_path + strlen(p_path);
  while (true) {
    if (_p == p_path) {
      return _p;
    }
    if ((*_p == '/') || (*_p == '\\')) {
      return ++_p;
    }
    --_p;
  }
}

} // namespace business
} // namespace string
} // namespace tenacitas

#endif // FILE_NAME_FROM_PATH_H

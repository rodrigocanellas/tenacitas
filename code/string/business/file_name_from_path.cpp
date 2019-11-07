#include <string/business/file_name_from_path.h>

namespace tenacitas {
namespace string {
namespace business {

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



#include <communication/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

std::string filename(const char *p_path) {
  const char *_end = p_path + strlen(p_path);
  const char *_ite = _end;
  const char *_begin = &p_path[0];

  for (; _ite >= _begin; --_ite) {
    if (*_ite == '/') {
      break;
    }
  }

  return {++_ite, _end};
}

} // namespace communication
} // namespace tenacitas

#include <sql/entities/columns.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const columns& p_columns)
{
  p_out << "[";
  columns::const_iterator _end = p_columns.end();
  for (columns::const_iterator _ite = p_columns.begin(); _ite != _end; ++_ite) {
    p_out << *_ite;
    if (std::next(_ite) != _end) {
      p_out << ",";
    }
  }
  p_out << "]";

  return p_out;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

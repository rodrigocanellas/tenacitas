#include <sql/entities/tables.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const tables& p_tables)
{
  p_out << "[";
  tables::const_iterator _end = p_tables.end();
  for (tables::const_iterator _ite = p_tables.begin(); _ite != _end; ++_ite) {
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

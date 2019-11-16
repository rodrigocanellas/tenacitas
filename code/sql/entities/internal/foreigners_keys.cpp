#include <sql/entities/foreigners_keys.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const foreigners_keys& p_foreigners_keys)
{
  p_out << "[";
  foreigners_keys::const_iterator _end = p_foreigners_keys.end();
  for (foreigners_keys::const_iterator _ite = p_foreigners_keys.begin();
       _ite != _end;
       ++_ite) {
    p_out << *_ite;
  }
  p_out << "]";
  return p_out;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

#include <sql/entities/database.h>

namespace capemisa {
namespace sql {
namespace entities {
std::ostream&
operator<<(std::ostream& p_out, const database& /*p_database*/)
{
  return p_out;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

#include <sql/entities/database.h>

namespace capemisa {
namespace sql {
namespace entities {
std::ostream&
operator<<(std::ostream& p_out, const database& p_database)
{
  p_out << "{"
        << "\"name\" : \"" << p_database.get_name() << "\", "
        << "\"tables\" : " << p_database.m_tables << "}";
  return p_out;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

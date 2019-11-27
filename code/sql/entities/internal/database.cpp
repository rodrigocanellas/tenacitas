#include <sql/entities/database.h>

#include <sql/entities/server.h>

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

const server&
database::get_server() const
{
  return *m_server;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

#include <sql/entities/host.h>
#include <sql/entities/server.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const server& p_server)
{
  p_out << "{\"name\" : \"" << p_server.get_name() << "\", "
        << "\"databases\" : " << p_server.m_databases << "}";

  return p_out;
}

generic::ptr<database>
server::find(const generic::name& p_database_name)
{
  return m_databases.find(p_database_name);
}

generic::ptr<database>
server::add_database(const generic::name& p_database_name)
{
  return m_databases.add(p_database_name, this);
}

const host&
server::get_host() const
{
  return *m_host;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

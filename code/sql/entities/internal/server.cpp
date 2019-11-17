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

ptr<database>
server::find(const name& p_database_name)
{
  return m_databases.find(
    [&p_database_name](const ptr<database>& p_db) -> bool {
      return p_db->get_name() == p_database_name;
    });
}

ptr<database>
server::add_database(const name& p_database_name)
{
  ptr<database> _db =
    m_databases.find([&p_database_name](const ptr<database>& p_db) -> bool {
      return p_db->get_name() == p_database_name;
    });

  if (_db == nullptr) {
    _db = make_ptr<database>(this, p_database_name);
    m_databases.add(_db);
  }
  return _db;
}

host*
server::get_host() const
{
  return m_host;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

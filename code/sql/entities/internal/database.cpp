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

ptr<table>
database::add_table(const name& p_table_name)
{
  ptr<table> _table =
    m_tables.find([&p_table_name](const ptr<table>& p_table) -> bool {
      return p_table->get_name() == p_table_name;
    });
  if (_table == nullptr) {
    _table = make_ptr<table>(this, p_table_name);
    m_tables.add(_table);
  }
  return _table;
}

server*
database::get_server() const
{
  return m_server;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

#include <sql/entities/primary_key.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const primary_key& p_pk)
{
  p_out << "\"pk\": {"
        << "\"columns\": [";

  primary_key::pk_columns::const_iterator _end = p_pk.end_columns();
  for (primary_key::pk_columns::const_iterator _ite = p_pk.begin_columns();
       _ite != _end;
       ++_ite) {
    p_out << *(*_ite);
    if (std::next(_ite) != _end) {
      p_out << ", ";
    }
  }
  p_out << "]}";
  return p_out;
}

const std::string&
primary_key::get_table_name() const
{
  return m_table->get_name();
}

} // namespace entities
} // namespace sql
} // namespace capemisa

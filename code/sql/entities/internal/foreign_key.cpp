#include <sql/entities/foreign_key.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const foreign_key& p_fk)
{
  p_out << "{ \"name\" : \"" << p_fk.get_name() << "\", "
        << "\"columns\" : " << p_fk.m_columns << "}";

  return p_out;
}

const table&
foreign_key::get_table() const
{
  return *m_table;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

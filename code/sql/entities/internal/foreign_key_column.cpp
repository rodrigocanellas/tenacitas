#include <sql/entities/foreign_key_column.h>
#include <sql/entities/primary_key.h>
#include <sql/entities/primary_key_column.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const foreign_key_column& p_fk)
{

  p_out << "{"
        << "\"name\" : \"" << p_fk.get_name() << "\", "
        << "\"pk\" : { \"table\" : \""
        << p_fk.m_primary_key_column->get_primary_key().get_table().get_name()
        << "\""
        << ", \"column\" : \"" << p_fk.m_primary_key_column->get_name() << "\"}"
        << "}";
  return p_out;
}

const foreign_key&
foreign_key_column::get_foreign_key() const
{
  return *m_foreign_key;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

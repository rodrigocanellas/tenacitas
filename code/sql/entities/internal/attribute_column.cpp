#include <sql/entities/attribute_column.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const attribute_column& p_attr)
{

  p_out << "{" << (const column)p_attr << ", \"null\" : \""
        << (p_attr.is_null() ? "true" : "false") << "\", "
        << "\"unique\" : \"" << (p_attr.is_unique() ? "true" : "false") << "\""
        << "}";

  return p_out;
}

const table&
attribute_column::get_table() const
{
  return *m_table;
}

} // namespace entities
} // namespace sql
} // namespace capemisa

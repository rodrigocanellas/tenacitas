#include <sql/entities/database.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const table& p_table)
{
  p_out << "{\"name\": \"" << p_table.get_name()
        << "\", \"columns\" : " << p_table.m_columns;
  if (p_table.get_primary_key() != nullptr) {
    p_out << ", \"primary_key\" : " << *(p_table.get_primary_key());
  }
  p_out << ", \"foreigners_keys\" : " << p_table.m_foreigners_keys;

  p_out << "}";
  return p_out;
}

database*
table::get_database() const
{
  return m_database;
}

ptr<column>
table::add_column(const name& p_column_name)
{

  ptr<column> _col =
    m_columns.find([&p_column_name](const ptr<column>& p_col) -> bool {
      return p_col->get_name() == p_column_name;
    });

  if (_col == nullptr) {
    _col = make_ptr<column>(this, p_column_name);
    m_columns.add(_col);
  }
  return _col;
}

void
table::add_to_primary_key(ptr<column> p_col)
{
  if (p_col == nullptr) {
    throw std::runtime_error("column is null in table::add_to_primary_key");
  }
  if (p_col->get_table()->get_name() != m_name) {
    throw std::runtime_error("column is not of table " + m_name);
  }

  if (m_primary_key == nullptr) {
    m_primary_key = make_ptr<primary_key>(this);
  }
  m_primary_key->add_column(p_col);
}

} // namespace entities
} // namespace sql
} // namespace capemisa

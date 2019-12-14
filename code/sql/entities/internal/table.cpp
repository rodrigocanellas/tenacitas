#include <sql/entities/database.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const table& p_table)
{
  p_out << "{\"name\": \"" << p_table.get_name() << "\""
        << ", \"pk\" : " << *p_table.m_primary_key
        << ", \"attributes\" : " << p_table.m_attributes
        << ", \"fks\" : " << p_table.m_foreigners_keys << "}";

  return p_out;
}

const database&
table::get_database() const
{
  return *m_database;
}

// generic::ptr<column>
// table::add_column(const generic::name& p_column_name, column_type p_type, size
// p_size)
//{

//  generic::ptr<column> _col =
//    m_columns.find([&p_column_name](const generic::ptr<column>& p_col) -> bool {
//      return p_col->get_name() == p_column_name;
//    });

//  if (_col == nullptr) {
//    _col = generic::make_ptr<column>(this, p_column_name, p_type, p_size);
//    m_columns.add(_col);
//  }
//  return _col;
//}

// generic::ptr<column>
// table::add_column(const generic::name& p_column_name, column_type p_type)
//{

//  generic::ptr<column> _col =
//    m_columns.find([&p_column_name](const generic::ptr<column>& p_col) -> bool {
//      return p_col->get_name() == p_column_name;
//    });

//  if (_col == nullptr) {
//    _col = generic::make_ptr<column>(this, p_column_name, p_type);
//    m_columns.add(_col);
//  }
//  return _col;
//}

// void
// table::add_to_primary_key(generic::ptr<column> p_col)
//{
//  if (p_col == nullptr) {
//    throw std::runtime_error("column is null in table::add_to_primary_key");
//  }
//  if (p_col->get_table()->get_name() != m_name) {
//    throw std::runtime_error("column is not of table " + m_name);
//  }

//  if (m_primary_key == nullptr) {
//    m_primary_key = generic::make_ptr<primary_key>(this);
//  }
//  m_primary_key->add_column(p_col);
//}

} // namespace entities
} // namespace sql
} // namespace capemisa

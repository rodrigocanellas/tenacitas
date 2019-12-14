#ifndef CAPEMISA_SQL_ENTITIES_TABLE_VALUES_H
#define CAPEMISA_SQL_ENTITIES_TABLE_VALUES_H

#include <sql/entities/column.h>
#include <sql/entities/column_values.h>
#include <sql/entities/table.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace entities {

using namespace generic;

struct table_values
{

  table_values(ptr<table> p_table)
    : m_table(p_table)
  {}

  inline void add(ptr<column_values> p_column_values)
  {
    m_column_values_list.add(p_column_values);
  }

  inline const name& get_name() const { return m_table->get_name(); }

  inline ptr<table> get_table() const { return m_table; }

  inline uint16_t get_num_cols() const
  {
    return m_column_values_list.get_size<uint16_t>();
  }

  inline ptr<column_values> get_column_values(uint16_t p_col_index) const
  {
    return m_column_values_list[p_col_index];
  }

  inline ptr<column_values> find_column_values(const name& p_col_name)
  {
    return m_column_values_list.find(p_col_name);
  }

private:
  typedef collection<column_values> column_values_list;

private:
  ptr<table> m_table;
  column_values_list m_column_values_list;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_VALUES_H

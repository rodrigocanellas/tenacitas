#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H

#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/name.h>
#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table;

struct primary_key
{

  friend std::ostream& operator<<(std::ostream& p_out, const primary_key& p_pk);

  inline explicit primary_key(table* p_table, const name& p_name = "PK")
    : m_name(p_name)
    , m_table(p_table)
  {}

  primary_key(primary_key&& p_pk)
    : m_name(std::move(p_pk.m_name))
    , m_table(p_pk.m_table)
    , m_columns(std::move(p_pk.m_columns))
  {}

  const name& get_name() const { return m_name; }
  void set_name(const name& p_name) { m_name = p_name; }

  uint16_t get_num_cols() const { return m_columns.get_size<uint16_t>(); }

  ptr<column> get_column(uint16_t p_index) { return m_columns[p_index]; }

  inline void add_column(ptr<column> p_column) { m_columns.add(p_column); }

  inline table* get_table() const { return m_table; }

  const name& get_table_name() const;

private:
private:
  name m_name;
  table* m_table;
  columns m_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // primary_key_H
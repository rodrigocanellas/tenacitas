#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H

#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/name.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table;

struct primary_key
{

  typedef typename std::list<columns::const_iterator> pk_columns;

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

  inline void add_column(columns::const_iterator p_column)
  {
    m_columns.push_back(p_column);
  }

  inline pk_columns::const_iterator begin_columns() const
  {
    return m_columns.begin();
  }

  inline pk_columns::const_iterator end_columns() const
  {
    return m_columns.end();
  }

  inline table* get_table() const { return m_table; }

  const name& get_table_name() const;

private:
private:
  name m_name;
  table* m_table;
  pk_columns m_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // primary_key_H

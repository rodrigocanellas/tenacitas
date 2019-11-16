#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H

#include <functional>
#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/foreigners_keys.h>
#include <sql/entities/name.h>
#include <sql/entities/primary_key.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table
{

  friend std::ostream& operator<<(std::ostream& p_out, const table& p_table);

  table() = delete;

  explicit inline table(const name& p_name)
    : m_name(p_name)
    , m_primary_key(this)
  {}

  inline const name& get_name() const { return m_name; }

  columns::const_iterator begin_column() const { return m_columns.begin(); }

  columns::const_iterator end_column() const { return m_columns.end(); }

  inline void add_column(column&& p_column)
  {
    m_columns.add(std::move(p_column));
  }

  foreigners_keys::const_iterator begin_foregign_key() const
  {
    return m_foreigners_keys.begin();
  }

  foreigners_keys::const_iterator end_foregign_key() const
  {
    return m_foreigners_keys.end();
  }

  inline const primary_key* get_primary_key() const { return &m_primary_key; }

  inline void add_to_primary_key(const name& p_column_name)
  {
    columns::const_iterator _col = m_columns.find(p_column_name);
    if (_col != m_columns.end()) {
      m_primary_key.add_column(_col);
    }
  }

  foreigners_keys::const_iterator add_foreign_key(foreign_key&& p_foreign_key)
  {
    return m_foreigners_keys.add(std::move(p_foreign_key));
  }

  inline bool operator==(const table& p_table) const
  {
    return m_name == p_table.get_name();
  }

  inline bool operator<(const table& p_table) const
  {
    return m_name < p_table.get_name();
  }

private:
private:
  name m_name;
  primary_key m_primary_key;
  columns m_columns;
  foreigners_keys m_foreigners_keys;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_H

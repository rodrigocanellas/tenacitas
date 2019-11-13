#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H

#include <list>
#include <functional>

#include <sql/entities/column.h>
#include <sql/entities/columns.h>
#include <sql/entities/name.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/foreign_keys.h>
#include <sql/entities/primary_key.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table {

  typedef primary_key_t<table>  primary_key;

  table()=delete;

  table(const name & p_name)
    : m_name(p_name),
      m_primary_key(this){}

  inline const name & get_name()const{
    return m_name;
  }

  inline void traverse(std::function<void(const column &)> p_function) const {
    for (const column & _column : m_columns) {
      p_function(_column);
    }
  }

  inline void add_column(column && p_column) {
    m_columns.push_back(std::move(p_column));
  }

  inline void add_to_primary_key(const name & p_column_name) {
    columns::const_iterator _ite = find_column(m_columns, p_column_name);
    if (_ite != m_columns.end()) {
      m_primary_key.add_column(_ite);
    }
  }

  void add_foreign_key(foreign_key && p_foreign_key) {
    m_foreign_keys.push_back(std::move(p_foreign_key));
  }

private:

private:
  name m_name;
  primary_key m_primary_key;
  columns m_columns;
  foreign_keys m_foreign_keys;
};


} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_H

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
#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

struct database;

struct table
{

  friend std::ostream& operator<<(std::ostream& p_out, const table& p_table);

  table() = delete;

  explicit inline table(database* p_database, const name& p_name)
    : m_database(p_database)
    , m_name(p_name)
  //    , m_primary_key(make_ptr<primary_key>(this))
  {}

  database* get_database() const;

  inline const name& get_name() const { return m_name; }

  uint16_t get_num_cols() const { return m_columns.get_size<uint16_t>(); }

  ptr<column> get_column(uint16_t p_index) { return m_columns[p_index]; }

  ptr<column> add_column(const name& p_column_name);

  inline uint16_t get_num_fks() const
  {
    return m_foreigners_keys.get_size<uint16_t>();
  }

  inline ptr<foreign_key> get_foregign_key(uint16_t p_index)
  {
    return m_foreigners_keys[p_index];
  }

  inline ptr<primary_key> get_primary_key() const { return m_primary_key; }

  void add_to_primary_key(ptr<column> p_col);

  inline void add_foreign_key(const name& p_fk_name, ptr<primary_key> p_pk)
  {
    ptr<foreign_key> _fk = find_foreign_key(p_fk_name);
    if (_fk == nullptr) {
      _fk = make_ptr<foreign_key>(p_fk_name, p_pk);
      m_foreigners_keys.add(_fk);
    }
  }

  inline ptr<column> find_column(const name& p_column_name)
  {
    return m_columns.find([&p_column_name](const ptr<column>& p_col) -> bool {
      return p_col->get_name() == p_column_name;
    });
  }

  inline ptr<foreign_key> find_foreign_key(const name& p_fk_name)
  {
    return m_foreigners_keys.find(
      [&p_fk_name](const ptr<foreign_key>& p_fk) -> bool {
        return p_fk->get_name() == p_fk_name;
      });
  }

private:
  database* m_database;
  name m_name;
  ptr<primary_key> m_primary_key;
  columns m_columns;
  foreigners_keys m_foreigners_keys;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_H

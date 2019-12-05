#ifndef CAPEMISA_SQL_ENTITIES_FOREIGN_KEY_H
#define CAPEMISA_SQL_ENTITIES_FOREIGN_KEY_H

#include <iostream>
#include <list>

#include <sql/entities/foreign_key_column.h>
#include <sql/entities/primary_key_column.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table;

struct foreign_key
{

  friend std::ostream& operator<<(std::ostream& p_out, const foreign_key& p_fk);

  foreign_key() = delete;
  foreign_key(const foreign_key&) = delete;
  foreign_key(foreign_key&&) = delete;
  foreign_key& operator=(const foreign_key&) = delete;
  foreign_key& operator=(foreign_key&&) = delete;
  ~foreign_key() = default;

  explicit foreign_key(const generic::name& p_name,
                       const table* p_table,
                       bool p_is_null = false)
    : m_name(p_name)
    , m_table(p_table)
    , m_is_null(p_is_null)
  {}

  const table& get_table() const;

  inline bool is_null() const { return m_is_null; }

  inline const generic::name& get_name() const { return m_name; }

  inline generic::ptr<foreign_key_column> add_column(
    const generic::name& p_name,
    generic::ptr<primary_key_column> p_pk_column)
  {
    return m_columns.add(p_name, this, p_pk_column);
  }

  inline generic::ptr<foreign_key_column> find_column(
    const generic::name& p_name)
  {
    return m_columns.find(p_name);
  }

  inline uint16_t get_num_cols() const
  {
    return m_columns.get_size<uint16_t>();
  }

  generic::ptr<foreign_key_column> get_fk_col(uint16_t p_fk_index)
  {
    return m_columns[p_fk_index];
  }

private:
  typedef generic::collection<foreign_key_column> columns;

private:
  generic::name m_name;
  const table* m_table;
  bool m_is_null;
  columns m_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEY_H

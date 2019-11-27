#ifndef CAPEMISA_SQL_ENTITIES_FOREIGN_KEY_H
#define CAPEMISA_SQL_ENTITIES_FOREIGN_KEY_H

#include <iostream>
#include <list>

#include <sql/entities/foreign_key_column.h>
#include <sql/entities/internal/collection.h>
#include <sql/entities/internal/types.h>
#include <sql/entities/name.h>
#include <sql/entities/primary_key_column.h>

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

  explicit foreign_key(const name& p_name, const table* p_table)
    : m_name(p_name)
    , m_table(p_table)
  {}

  const table& get_table() const;

  inline const name& get_name() const { return m_name; }

  inline ptr<foreign_key_column> add_column(const name& p_name,
                                            ptr<primary_key_column> p_pk_column)
  {
    return m_columns.add(p_name, this, p_pk_column);
  }

  inline ptr<foreign_key_column> find_column(const name& p_name)
  {
    return m_columns.find(p_name);
  }

private:
  typedef collection<foreign_key_column> columns;

private:
  name m_name;
  const table* m_table;
  columns m_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEY_H

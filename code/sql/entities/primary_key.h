#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H

#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/internal/collection.h>
#include <sql/entities/primary_key_column.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table;

struct primary_key
{

  friend std::ostream& operator<<(std::ostream& p_out, const primary_key& p_pk);

  primary_key() = delete;
  primary_key(const primary_key&) = delete;
  primary_key(primary_key&&) = delete;
  primary_key& operator=(const primary_key&) = delete;
  primary_key& operator=(primary_key&&) = delete;
  ~primary_key() = default;

  inline explicit primary_key(const table& p_table)
    : m_table(p_table)
  {}

  inline uint16_t get_num_cols() const
  {
    return m_columns.get_size<uint16_t>();
  }

  inline ptr<primary_key_column> get_column(uint16_t p_index)
  {
    return m_columns[p_index];
  }

  inline void add_column(ptr<primary_key_column> p_column)
  {
    m_columns.add(p_column);
  }

  const name& get_table_name() const;

  const table& get_table() const;

private:
  typedef collection<primary_key_column> columns;

private:
  const table& m_table;
  columns m_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // primary_key_H

#ifndef CAPEMISA_SQL_ENTITIES_PRIMARY_KEY_T_H
#define CAPEMISA_SQL_ENTITIES_PRIMARY_KEY_T_H

#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/foreign_key_column.h>
#include <sql/entities/primary_key_column.h>
#include <sql/generic/collection.h>

namespace capemisa {
namespace sql {
namespace entities {

using namespace generic;

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

  inline explicit primary_key(const table* p_table)
    : m_table(p_table)
  {}

  inline uint16_t get_num_pks_cols() const
  {
    return m_pks_columns.get_size<uint16_t>();
  }

  inline ptr<primary_key_column> get_pk_column(uint16_t p_index)
  {
    return m_pks_columns[p_index];
  }

  inline ptr<primary_key_column> add_pk_column(const name& p_name,
                                               column_type p_type,
                                               size p_size,
                                               bool p_is_auto_increment,
                                               bool p_is_unique)
  {
    return m_pks_columns.add(
      p_name, this, p_type, p_size, p_is_auto_increment, p_is_unique);
  }

  inline ptr<primary_key_column> add_pk_column(const name& p_name,
                                               column_type p_type,
                                               bool p_is_auto_increment,
                                               bool p_is_unique)
  {
    return m_pks_columns.add(
      p_name, this, p_type, p_is_auto_increment, p_is_unique);
  }

  inline ptr<primary_key_column> find_pk_column(const name& p_pk_col_name) const
  {
    return m_pks_columns.find(p_pk_col_name);
  }

  inline uint16_t get_num_fks_cols() const
  {
    return m_fks_columns.get_size<uint16_t>();
  }

  inline ptr<foreign_key_column> get_fk_column(uint16_t p_index)
  {
    return m_fks_columns[p_index];
  }

  inline void add_fk_column(ptr<foreign_key_column> p_fk_column)
  {
    m_fks_columns.add(p_fk_column);
  }

  inline ptr<foreign_key_column> find_fk_column(const name& p_fk_col_name) const
  {
    return m_fks_columns.find(p_fk_col_name);
  }

  const table& get_table() const;

private:
  typedef collection<primary_key_column> pks_columns;
  typedef collection<foreign_key_column> fks_columns;

private:
  const table* m_table;
  pks_columns m_pks_columns;
  fks_columns m_fks_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // primary_key_H

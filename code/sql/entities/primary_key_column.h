#ifndef CAPEMISA_SQL_ENTITIES_PRIMARY_KEY_COLUMN_H
#define CAPEMISA_SQL_ENTITIES_PRIMARY_KEY_COLUMN_H

#include <iostream>

#include <sql/entities/column.h>
#include <sql/entities/name.h>
#include <sql/entities/size.h>

namespace capemisa {
namespace sql {
namespace entities {

struct primary_key;

struct primary_key_column : public column
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const primary_key_column& p_pk);

  primary_key_column() = delete;
  primary_key_column(const primary_key_column&) = delete;
  primary_key_column(primary_key_column&&) = delete;
  primary_key_column& operator=(const primary_key_column&) = delete;
  primary_key_column& operator=(primary_key_column&&) = delete;
  ~primary_key_column() override = default;

  primary_key_column(const name& p_name,
                     const primary_key* p_primary_key,
                     column::type p_type,
                     size p_size,
                     bool p_is_auto_increment,
                     bool p_is_unique)
    : column(p_name, p_type, p_size)
    , m_is_autoincrement(p_is_auto_increment)
    , m_is_unique(p_is_unique)
    , m_primary_key(p_primary_key)
  {}

  primary_key_column(const name& p_name,
                     const primary_key* p_primary_key,
                     column::type p_type,
                     bool p_is_auto_increment,
                     bool p_is_unique)
    : column(p_name, p_type)
    , m_is_autoincrement(p_is_auto_increment)
    , m_is_unique(p_is_unique)
    , m_primary_key(p_primary_key)
  {}

  const primary_key& get_primary_key() const;

  inline bool is_autoincrement() const { return m_is_autoincrement; }

  inline bool is_unique() const { return m_is_unique; }

private:
  bool m_is_autoincrement;
  bool m_is_unique;
  const primary_key* m_primary_key;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // PRIMARY_KEY_COLUMN_H

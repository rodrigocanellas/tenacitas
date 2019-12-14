#ifndef CAPEMISA_SQL_ENTITIES_ATTRIBUTE_COLUMN_H
#define CAPEMISA_SQL_ENTITIES_ATTRIBUTE_COLUMN_H

#include <iostream>

#include <sql/entities/column.h>
#include <sql/entities/column_type.h>
#include <sql/entities/column_usage.h>
#include <sql/entities/size.h>
#include <sql/generic/name.h>

namespace capemisa {
namespace sql {
namespace entities {

using namespace generic;

struct table;

struct attribute_column : public column
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const attribute_column& p_attr);

  attribute_column() = delete;
  attribute_column(const attribute_column&) = delete;
  attribute_column(attribute_column&&) = delete;
  attribute_column& operator=(const attribute_column&) = delete;
  attribute_column& operator=(attribute_column&&) = delete;
  virtual ~attribute_column() override = default;

  attribute_column(const name& p_name,
                   const table* p_table,
                   column_type p_type,
                   size p_size,
                   bool p_is_null,
                   bool p_is_unique)
    : column(p_name, p_type, p_size)
    , m_is_null(p_is_null)
    , m_is_unique(p_is_unique)
    , m_table(p_table)
  {}

  attribute_column(const name& p_name,
                   const table* p_table,
                   column_type p_type,
                   bool p_is_null,
                   bool p_is_unique)
    : column(p_name, p_type)
    , m_is_null(p_is_null)
    , m_is_unique(p_is_unique)
    , m_table(p_table)
  {}

  inline bool is_null() const { return m_is_null; }

  inline bool is_unique() const { return m_is_unique; }

  const table& get_table() const;

private:
  bool m_is_null;
  bool m_is_unique;
  const table* m_table;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // ATTRIBUTE_COLUMN_H

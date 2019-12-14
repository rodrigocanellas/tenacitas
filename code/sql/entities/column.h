#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_H

#include <cstdint>
#include <ctime>
#include <iostream>

#include <sql/entities/column_type.h>
#include <sql/entities/column_usage.h>
#include <sql/entities/size.h>
#include <sql/generic/name.h>

namespace capemisa {
namespace sql {
namespace entities {

using namespace generic;

struct column
{

  friend inline std::ostream& operator<<(std::ostream& p_out,
                                         const column& p_column)
  {
    p_out << "\"name\" : \"" << p_column.get_name() << "\", "
          << "\"type\" : \"" << p_column.get_type() << "\", "
          << "\"size\" : \"" << p_column.get_size() << "\"";
    return p_out;
  }

  column() = delete;
  column(const column&) = default;
  column(column&&) = delete;
  column& operator=(const column&) = default;
  column& operator=(column&&) = delete;
  virtual ~column() = default;

  inline const name& get_name() const { return m_name; }

  inline column_type get_type() const { return m_type; }

  inline size get_size() const { return m_size; }

protected:
  explicit inline column(const name& p_name, column_type p_type, size p_size)
    : m_name(p_name)
    , m_type(p_type)
    , m_size(p_size)
  {}

  explicit inline column(const name& p_name, column_type p_type)
    : m_name(p_name)
    , m_type(p_type)
    , m_size(m_type.get_size())
  {}

private:
  name m_name;
  column_type m_type;
  size m_size;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMN_H

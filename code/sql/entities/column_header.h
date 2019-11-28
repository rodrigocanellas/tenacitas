#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_HEADER_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_HEADER_H

#include <list>

#include <sql/entities/column_value.h>

namespace capemisa {
namespace sql {
namespace entities {

struct column_header
{

  typedef std::list<column_value> values;

  virtual void generate() = 0;

  inline const values& get_values() const { return m_values; }

protected:
  values m_values;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMN_HEADER_H

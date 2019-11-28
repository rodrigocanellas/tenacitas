#ifndef CAPEMISA_SQL_ENTITIES_ATTRIBUTE_COLUMN_HEADER_H
#define CAPEMISA_SQL_ENTITIES_ATTRIBUTE_COLUMN_HEADER_H

#include <sql/entities/attribute_column.h>
#include <sql/entities/attribute_value_pattern.h>
#include <sql/entities/column_header.h>
#include <sql/entities/internal/types.h>

namespace capemisa {
namespace sql {
namespace entities {

struct attribute_column_header : public column_header
{

  attribute_column_header() = delete;

  attribute_column_header(ptr<attribute_column> p_attribute_column,
                          attribure_value_pattern p_attribure_value_pattern)
    : m_attribute_column(p_attribute_column)
    , m_attribure_value_pattern(p_attribure_value_pattern)
  {}

  void generate() override;

private:
private:
  ptr<attribute_column> m_attribute_column;
  attribure_value_pattern m_attribure_value_pattern;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // ATTRIBUTE_COLUMN_HEADER_H

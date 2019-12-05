#ifndef CAPEMISA_SQL_BUSINESS_TEXT_VALUE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_TEXT_VALUE_GENERATOR_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include <sql/entities/column.h>
#include <sql/entities/column_values.h>
#include <sql/entities/value.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

struct text_value_generator
{
  typedef entities::column_values column_values;
  typedef generic::ptr<entities::column_values> column_values_ptr;
  typedef entities::column column;

  inline text_value_generator(const std::string& p_str,
                              uint16_t p_size,
                              uint16_t p_base)
    : m_str(p_str)
    , m_size(p_size)
    , m_base(p_base)
  {}

  column_values_ptr operator()(generic::ptr<column> p_column,
                               uint16_t p_num_values);

private:
  std::string m_str;
  uint16_t m_size;
  uint16_t m_base;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // TEXT_VALUE_GENERATOR_H

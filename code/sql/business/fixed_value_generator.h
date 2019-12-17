#ifndef CAPEMISA_SQL_BUSINESS_FIXED_VALUE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_FIXED_VALUE_GENERATOR_H

#include <cstdint>
#include <string>

#include <sql/business/column_generator.h>
#include <sql/entities/attribute_column.h>
#include <sql/entities/column.h>
#include <sql/entities/column_type.h>
#include <sql/entities/column_values.h>
#include <sql/entities/value.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

struct fixed_value_generator : public column_generator<attribute_column>
{

  fixed_value_generator(const std::string& p_value)
    : column_generator<attribute_column>(fixed_value_generator::compatibles())
    , m_value(p_value)
  {}

  static const name id;

  static column_type compatibles()
  {
    return (column_type::int_1 | column_type::int_2 | column_type::int_4 |
            column_type::int_8 | column_type::date | column_type::date_time |
            column_type::small_real | column_type::long_real |
            column_type::fixed_size_text | column_type::var_size_text);
  }

protected:
  ptr<column_values> generate(ptr<attribute_column> p_column,
                              uint16_t p_num_values)
  {
    ptr<column_values> _values(make_ptr<column_values>(p_column));
    for (uint16_t _counter = 0; _counter < p_num_values; ++_counter) {
      if (p_column->get_type() &
          (column_type::fixed_size_text | column_type::var_size_text)) {
        if (static_cast<uint16_t>(m_value.size()) > p_column->get_size()) {
          throw("coluna '" + p_column->get_name() + "' tem tamanho máximo " +
                std::to_string(p_column->get_size()) +
                "', e valor único definido tem tamanho " +
                std::to_string(m_value.size()));
        }
        _values->add(value("'" + m_value + "'"));
      } else {
        _values->add(value(m_value));
      }
    }
    return _values;
  }

private:
  std::string m_value;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // FIXED_VALUE_GENERATOR_H

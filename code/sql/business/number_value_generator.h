#ifndef CAPEMISA_SQL_BUSINESS_NUMBER_VALUE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_NUMBER_VALUE_GENERATOR_H

#include <cstdint>
#include <string>

#include <sql/business/column_generator.h>
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

template<typename t_num_type, typename t_column>
struct number_value_generator : public column_generator<t_column>
{

  number_value_generator(t_num_type p_base,
                         t_num_type p_limit,
                         t_num_type p_increment = 1)
    : column_generator<t_column>(number_value_generator::compatibles())
    , m_base(p_base)
    , m_increment(p_increment)
    , m_limit(p_limit)
  {}

  static const name id;

  static column_type compatibles()
  {
    return (column_type::int_1 | column_type::int_2 | column_type::int_4 |
            column_type::int_8 | column_type::date | column_type::date_time |
            column_type::small_real | column_type::long_real);
  }

protected:
  ptr<column_values> generate(ptr<t_column> p_column, uint16_t p_num_values)
  {

    //    column_type _column_type = p_column->get_type();
    //    if ((_column_type != column_type::int_1) &&
    //      throw std::runtime_error(
    //        "não é possível usar um gerador de valor sequencial na coluna " +
    //        p_column->get_name() + ", pois seu tipo é " +
    //        column_type2str(p_column->get_type()));
    //    }

    t_num_type _value = m_base;
    ptr<column_values> _values(make_ptr<column_values>(p_column));
    for (uint16_t _counter = 0; _counter < p_num_values; ++_counter) {
      _values->add(value(std::to_string(_value)));
      if (_value < m_limit) {
        _value += m_increment;
      }
    }
    return _values;
  }

private:
  t_num_type m_base;
  t_num_type m_increment;
  t_num_type m_limit;
};

template<typename t_num_type, typename t_column>
const name number_value_generator<t_num_type, t_column>::id(
  "gerador de números ");

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // NUMBER_VALUE_GENETATOR_H

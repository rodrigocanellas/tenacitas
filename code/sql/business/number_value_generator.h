#ifndef CAPEMISA_SQL_BUSINESS_NUMBER_VALUE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_NUMBER_VALUE_GENERATOR_H

#include <cstdint>
#include <string>

#include <sql/entities/column.h>
#include <sql/entities/column_values.h>
#include <sql/entities/value.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

template<typename t_num_type>
struct number_value_generator
{

  typedef entities::column_values column_values;
  typedef generic::ptr<entities::column_values> column_values_ptr;
  typedef entities::column column;

  number_value_generator(t_num_type p_base,
                         t_num_type p_limit,
                         t_num_type p_increment = 1)
    : m_base(p_base)
    , m_increment(p_increment)
    , m_limit(p_limit)
  {}

  column_values_ptr operator()(generic::ptr<column> p_column,
                               uint16_t p_num_values)
  {
    typedef entities::value value;

    //    column::type _column_type = p_column->get_type();
    //    if ((_column_type != column::type::int_1) &&
    //        (_column_type != column::type::int_2)) {
    //      throw std::runtime_error(
    //        "não é possível usar um gerador de valor sequencial na coluna " +
    //        p_column->get_name() + ", pois seu tipo é " +
    //        column::type2str(p_column->get_type()));
    //    }

    t_num_type _value = m_base;
    column_values_ptr _values(generic::make_ptr<column_values>(p_column));
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

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // SEQUENTIAL_VALUE_GENETATOR_H

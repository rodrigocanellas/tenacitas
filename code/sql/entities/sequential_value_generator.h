#ifndef CAPEMISA_SQL_ENTITIES_SEQUENTIAL_VALUE_GENERATOR_H
#define CAPEMISA_SQL_ENTITIES_SEQUENTIAL_VALUE_GENERATOR_H

#include <cstdint>
#include <string>

#include <sql/entities/column.h>
#include <sql/entities/value.h>
#include <sql/entities/value_generator.h>
#include <sql/entities/values.h>

namespace capemisa {
namespace sql {
namespace entities {

template<typename t_type>
struct sequential_value_generator : public value_generator
{

  sequential_value_generator(t_type p_base, t_type p_increment = 1)
    : value_generator("SEQ_VAL_GEN",
                      "gera valores sequenciais, a partir de uma base",
                      column::usage::attribute)
    , m_base(p_base)
    , m_increment(p_increment)
  {}

  ~sequential_value_generator() override = default;

  values generate(uint16_t p_num_values) override
  {
    t_type _value = m_base;
    values _values;
    for (uint16_t _counter = 0; _counter < p_num_values; ++_counter) {
      _values.push_back(value(std::to_string(_value)));
      _value += m_increment;
    }
    return _values;
  }

private:
  t_type m_base;
  t_type m_increment;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // SEQUENTIAL_VALUE_GENETATOR_H

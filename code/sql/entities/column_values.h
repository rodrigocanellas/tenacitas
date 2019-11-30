#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_VALUES_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_VALUES_H

#include <list>

#include <sql/entities/column.h>
#include <sql/entities/internal/types.h>
#include <sql/entities/value_generator.h>
#include <sql/entities/values.h>

namespace capemisa {
namespace sql {
namespace entities {

struct column_values
{
  column_values() = delete;

  explicit inline column_values(ptr<column> p_column,
                                ptr<value_generator> p_value_generator)
    : m_column(p_column)
    , m_value_generator(p_value_generator)
  {
    if (m_column->get_usage() != m_value_generator->get_usage()) {
      throw std::runtime_error(
        "Column " + m_column->get_name() + "'s usage (" +
        column::usage2str(m_column->get_usage()) +
        ") is incompatible with value generator usage (" +
        column::usage2str(m_value_generator->get_usage()) + ")");
    }
  }

  void generate_values(uint16_t p_num_values)
  {
    values _value(m_value_generator->generate(p_num_values));
  }

private:
  ptr<column> m_column;
  ptr<value_generator> m_value_generator;
  values m_values;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMN_VALUES_H

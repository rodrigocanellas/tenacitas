#ifndef CAPEMISA_SQL_BUSINESS_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_GENERATOR_H

#include <sql/entities/column.h>
#include <sql/entities/table.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace capemisa::sql::entities;

template<typename t_column>
struct column_generator
{
  typedef t_column column;

  column_generator() = delete;

  virtual ~column_generator() = default;

  inline column_generator(column_type p_types)
    : m_types(p_types)
  {}

  ptr<column_values> operator()(ptr<column> p_column, uint16_t p_num_lines)
  {
    check_type_compatibility(p_column);
    return generate(p_column, p_num_lines);
  }

protected:
  inline void check_type_compatibility(ptr<column> p_col) const
  {
    if (m_types & p_col->get_type()) {
      return;
    }
    throw std::runtime_error("Tipo " + p_col->get_type().to_str() +
                             " da coluna " + p_col->get_name() +
                             " é incompatível com gerador");
  }

  virtual ptr<column_values> generate(ptr<column> p_column,
                                      uint16_t p_num_lines) = 0;

private:
  column_type m_types;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // GENERATOR_H

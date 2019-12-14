#ifndef CAPEMISA_SQL_BUSINESS_TEXT_VALUE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_TEXT_VALUE_GENERATOR_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include <sql/business/column_generator.h>
#include <sql/entities/column.h>
#include <sql/entities/column_values.h>
#include <sql/entities/value.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

template<typename t_column>
struct text_value_generator : public column_generator<t_column>
{
  typedef ptr<column_values> column_values_ptr;

  inline text_value_generator(const std::string& p_str,
                              uint16_t p_size,
                              uint16_t p_base)

    : column_generator<t_column>(text_value_generator::compatibles())
    , m_str(p_str)
    , m_size(p_size)
    , m_base(p_base)
  {}

  static const name id;

  static column_type compatibles()
  {
    return (column_type::fixed_size_text | column_type::var_size_text);
  }

protected:
  column_values_ptr generate(ptr<t_column> p_column, uint16_t p_num_values)
  {
    using namespace std;

    if (p_column->get_size() < m_size) {
      throw std::runtime_error(
        "coluna '" + p_column->get_name() + "' tem tamanho máximo " +
        std::to_string(p_column->get_size()) +
        "', e gerador de valor de texto está definindo tamanho máximo " +
        std::to_string(m_size));
    }

    uint16_t _num = m_base;

    column_values_ptr _values(make_ptr<column_values>(p_column));
    for (uint16_t _counter = 0; _counter < p_num_values; ++_counter) {
      string _num_str = to_string(_num++);

      string::size_type _size_str = m_str.size() + _num_str.size() + 2;

      if (_size_str > m_size) {
        throw std::runtime_error("valor para o campo '" + p_column->get_name() +
                                 "', excedeu o tamanho definido de " +
                                 to_string(m_size) + " na linha " +
                                 to_string(_counter));
      }

      _values->add(
        string("'" + m_str + _num_str + string(m_size - _size_str, ' ') + "'"));
    }
    return _values;
  }

private:
  std::string m_str;
  uint16_t m_size;
  uint16_t m_base;
};

template<typename t_column>
const name text_value_generator<t_column>::id("gerador de textos");

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // TEXT_VALUE_GENERATOR_H

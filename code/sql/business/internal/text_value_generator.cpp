#include <sql/business/text_value_generator.h>

namespace capemisa {
namespace sql {
namespace business {

text_value_generator::column_values_ptr
text_value_generator::operator()(generic::ptr<column> p_column,
                                 uint16_t p_num_values)
{

  using namespace std;
  if ((p_column->get_type() != column::type::var_size_text) &&
      (p_column->get_type() != column::type::fixed_size_text)) {
    throw std::runtime_error(
      "não é possível usar um gerador de valores de texto para a coluna '" +
      p_column->get_name() + "', pois não é do tipo '" +
      column::type2str(column::type::var_size_text) + "' ou '" +
      column::type2str(column::type::var_size_text) + "'");
  }

  if (p_column->get_size() < m_size) {
    throw std::runtime_error(
      "coluna '" + p_column->get_name() + "' tem tamanho máximo " +
      std::to_string(p_column->get_size()) +
      "', e gerador de valor de texto está definindo tamanho máximo " +
      std::to_string(m_size));
  }

  uint16_t _num = m_base;

  column_values_ptr _values(generic::make_ptr<column_values>(p_column));
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

} // namespace business
} // namespace sql
} // namespace capemisa

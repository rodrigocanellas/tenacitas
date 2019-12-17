#include <sql/entities/column_type.h>

namespace capemisa {
namespace sql {
namespace entities {

const column_type column_type::undefined(0);
const column_type column_type::int_1(1);
const column_type column_type::int_2(2);
const column_type column_type::int_4(4);
const column_type column_type::int_8(8);
const column_type column_type::single_char(16);
const column_type column_type::fixed_size_text(32);
const column_type column_type::var_size_text(64);
const column_type column_type::date(128);
const column_type column_type::date_time(256);
const column_type column_type::small_real(512);
const column_type column_type::long_real(1024);

std::ostream&
operator<<(std::ostream& p_out, const column_type& p_column_type)
{
  p_out << p_column_type.to_str();
  return p_out;
}

size
column_type::get_size() const
{
  if (m_value == column_type::int_1.m_value) {
    return 1;
  }
  if (m_value == column_type::int_2.m_value) {
    return 2;
  }
  if (m_value == column_type::int_4.m_value) {
    return 4;
  }
  if (m_value == column_type::int_8.m_value) {
    return 8;
  }
  if (m_value == column_type::date.m_value) {
    return 4;
  }
  if (m_value == column_type::date_time.m_value) {
    return 4;
  }
  if (m_value == column_type::small_real.m_value) {
    return sizeof(float);
  }
  if (m_value == column_type::long_real.m_value) {
    return sizeof(double);
  }
  if (m_value == column_type::single_char.m_value) {
    return 1;
  }
  return -1;
}

std::string
column_type::to_str() const
{
  if (m_value == column_type::int_1.m_value) {
    return "int_1";
  }
  if (m_value == column_type::int_2.m_value) {
    return "int_2";
  }
  if (m_value == column_type::int_4.m_value) {
    return "int_4";
  }
  if (m_value == column_type::int_8.m_value) {
    return "int_8";
  }
  if (m_value == column_type::date.m_value) {
    return "date";
  }
  if (m_value == column_type::date_time.m_value) {
    return "date_time";
  }
  if (m_value == column_type::small_real.m_value) {
    return "small_real";
  }
  if (m_value == column_type::long_real.m_value) {
    return "long_real";
  }
  if (m_value == column_type::single_char.m_value) {
    return "single_char";
  }
  if (m_value == column_type::fixed_size_text.m_value) {
    return "fixed_size_text";
  }
  if (m_value == column_type::var_size_text.m_value) {
    return "var_size_text";
  }
  return "undefined";
}
} // namespace entities
} // namespace sql
} // namespace capemisa

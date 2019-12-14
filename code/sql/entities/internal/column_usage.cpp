#include <sql/entities/column_usage.h>

namespace capemisa {
namespace sql {
namespace entities {

// const column_usage column_usage::primary_key(1);
// const column_usage column_usage::foreign_key(2);
// const column_usage column_usage::attribute(4);

// std::ostream&
// operator<<(std::ostream& p_out, const column_usage& p_column_usage)
//{
//  p_out << p_column_usage.to_str();
//  return p_out;
//}

// std::string
// column_usage::to_str() const
//{
//  if (m_value == column_usage::primary_key.m_value) {
//    return "primary__key";
//  }
//  if (m_value == column_usage::foreign_key.m_value) {
//    return "int_2";
//  }
//  if (m_value == column_usage::attribute.m_value) {
//    return "int_4";
//  }
//  return "undefined";
//}

} // namespace entities
} // namespace sql
} // namespace capemisa

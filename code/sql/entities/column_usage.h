#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_USAGE_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_USAGE_H

#include <cstdint>
#include <iostream>
#include <string>

namespace capemisa {
namespace sql {
namespace entities {

enum class column_usage : int8_t
{
  primary_key = 1,
  foreign_key = 2,
  attribute = 3
};

// struct column_usage
//{

//  friend std::ostream& operator<<(std::ostream& p_out,
//                                  const column_usage& p_column_usage);

//  column_usage() = delete;
//  column_usage(const column_usage&) = default;
//  column_usage(column_usage&&) = default;
//  column_usage& operator=(const column_usage&) = default;
//  column_usage& operator=(column_usage&&) = default;
//  ~column_usage() = default;

//  static const column_usage primary_key;
//  static const column_usage foreign_key;
//  static const column_usage attribute;

//  inline column_usage operator|(column_usage p_usage) const
//  {
//    return column_usage(m_value | p_usage.m_value);
//  }

//  inline bool operator&(column_usage p_usage) const
//  {
//    return p_usage.m_value & m_value;
//  }

// private:
//  column_usage(int8_t p_value)
//    : m_value(p_value)
//  {}

//  std::string to_str() const;

// private:
//  int8_t m_value;
//};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMN_USAGE_H

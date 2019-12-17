#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_TYPE_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_TYPE_H

#include <cstdint>
#include <iostream>
#include <string>

#include <sql/entities/size.h>

namespace capemisa {
namespace sql {
namespace entities {

struct column_type
{

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const column_type& p_column_type);

  column_type() = delete;
  column_type(const column_type&) = default;
  column_type(column_type&&) = default;
  column_type& operator=(const column_type&) = default;
  column_type& operator=(column_type&&) = default;
  ~column_type() = default;

  static const column_type undefined;
  static const column_type int_1;
  static const column_type int_2;
  static const column_type int_4;
  static const column_type int_8;
  static const column_type single_char;
  static const column_type fixed_size_text;
  static const column_type var_size_text;
  static const column_type date;
  static const column_type date_time;
  static const column_type small_real;
  static const column_type long_real;

  inline column_type operator|(column_type p_type) const
  {
    return column_type(m_value | p_type.m_value);
  }

  inline bool operator&(column_type p_type) const
  {
    return p_type.m_value & m_value;
  }

  size get_size() const;

  std::string to_str() const;

private:
  column_type(int16_t p_value)
    : m_value(p_value)
  {}

private:
  int16_t m_value;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMN_TYPE_H

#ifndef CAPEMISA_SQL_ENTITIES_FOREIGN_KEY_VALUE_H
#define CAPEMISA_SQL_ENTITIES_FOREIGN_KEY_VALUE_H

#include <sql/entities/names.h>
#include <sql/entities/column_value_pattern.h>
#include <sql/entities/foreign_key_value_pattern.h>

namespace capemisa {
namespace sql {
namespace entities {

struct foreign_key_value {

  foreign_key_value()=delete;

  inline foreign_key_value(foreign_key_name p_foreign_key_name,
                           foreign_key_value_pattern p_foreign_key_value_pattern,
                           column_value_pattern p_column_value_pattern)
    : m_foreign_key_name(p_foreign_key_name)
    , m_foreign_key_value_pattern(p_foreign_key_value_pattern)
    , m_column_value_pattern(p_column_value_pattern){}

  inline foreign_key_name get_foreign_key_name()const{return m_foreign_key_name;}
  inline foreign_key_value_pattern get_foreign_key_value_pattern() const{
    return m_foreign_key_value_pattern;
  }
  inline column_value_pattern get_column_value_pattern()const{
    return m_column_value_pattern;
  }

  bool operator==(const foreign_key_value & p_foreign_key_value) const {
    return m_foreign_key_name == p_foreign_key_value.m_foreign_key_name;
  }


private:
  foreign_key_name m_foreign_key_name;
  foreign_key_value_pattern m_foreign_key_value_pattern;
  column_value_pattern m_column_value_pattern;
};

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // FOREIGN_KEY_VALUE_H

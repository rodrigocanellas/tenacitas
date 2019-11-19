#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMN_VALUE_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMN_VALUE_H

#include <sql/entities/names.h>
#include <sql/entities/column_value_pattern.h>

namespace capemisa {
namespace sql {
namespace entities {

struct column_value {
  column_value ()=delete ;

  inline column_value(column_name p_column_name,
               column_value_pattern p_value_pattern)
    : m_column_name(p_column_name)
    , m_column_value_pattern(p_value_pattern) {}

  inline column_name get_column_name() const {return m_column_name;}

  inline column_value_pattern get_value_pattern()const{return m_column_value_pattern;}

  bool operator==(const column_value&p_column_value) const {
    return m_column_name == p_column_value.m_column_name;
  }

private:
  column_name m_column_name;
  column_value_pattern m_column_value_pattern;

};

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // COLUMN_VALUE_H

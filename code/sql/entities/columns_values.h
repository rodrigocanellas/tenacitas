#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_VALUES_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_VALUES_H

#include <algorithm>
#include <list>

#include <sql/entities/column_value.h>
#include <sql/entities/names.h>

namespace capemisa {
namespace sql {
namespace entities {

struct columns_values {
  typedef std::list<column_value> column_value_list;
  typedef column_value_list::const_iterator const_iterator;

  const_iterator add(const column_name & p_column_name
                     , column_value_pattern p_column_value_pattern) {

    const_iterator _ite =
        std::find_if(m_list.begin(),
                     m_list.end(),
                     [&p_column_name](const column_value & p_cv)->bool {
      return p_cv.get_column_name()==p_column_name;
    });
    if (_ite != m_list.end()){
      return _ite;
    }
    m_list.push_back(column_value(p_column_name, p_column_value_pattern));
    return std::prev(m_list.end());
  }

  const_iterator begin()const{
    return m_list.begin();
  }

  const_iterator end()const{
    return m_list.end();
  }


private:
  column_value_list m_list;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_VALUES_H

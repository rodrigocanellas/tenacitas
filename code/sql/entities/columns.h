#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H

#include <memory>
#include <algorithm>
#include <list>


#include <sql/entities/name.h>

namespace capemisa {
namespace sql {
namespace entities {

template <typename t_traits>
struct columns_t {

  typedef t_traits traits;
  typedef typename traits::column column;
  typedef std::list<column> column_list;
  typedef typename column_list::iterator column_ptr;
  typedef typename column_list::const_iterator column_const_ptr;

  inline void add(column && p_column) {
    m_column_list.push_back(std::move(p_column));
  }

  inline column_const_ptr find(const name & p_column_name) {
    return std::find_if(m_column_list.begin(),
                        m_column_list.end(),
                        [&p_column_name](const column & p_column)->bool{
      return p_column.get_name() == p_column_name;
    });

  }

  column_const_ptr begin() const {
    return m_column_list.begin();
  }

  column_const_ptr end() const {
    return m_column_list.end();
  }

private:
  column_list m_column_list;
};



} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_H

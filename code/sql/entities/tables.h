#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

struct tables
{

  typedef std::list<table> table_list;

  typedef table_list::const_iterator const_iterator;
  typedef table_list::iterator iterator;

  friend std::ostream& operator<<(std::ostream& p_out, const tables& p_tables);

  inline iterator add(table&& p_table)
  {
    //    table_list::iterator _ite =
    //    m_table_list.emplace(std::move(p_table)).first;
    m_table_list.push_back(std::move(p_table));
    return std::prev(m_table_list.end());
  }

  inline const_iterator end() const { return m_table_list.end(); }

  inline const_iterator begin() const { return m_table_list.begin(); }

  inline iterator end() { return m_table_list.end(); }

  inline iterator begin() { return m_table_list.begin(); }

  inline iterator find(const name& p_table_name)
  {
    return std::find_if(m_table_list.begin(),
                        m_table_list.end(),
                        [&p_table_name](const table& p_table) -> bool {
                          return p_table.get_name() == p_table_name;
                        });
    //    m_table_list.
  }

private:
  table_list m_table_list;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLES_H

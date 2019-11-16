#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H

#include <algorithm>
#include <iterator>
#include <set>

#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

struct tables
{

  typedef std::set<table> table_list;

  typedef table_list::const_iterator const_iterator;
  typedef table_list::iterator iterator;

  inline iterator add(table&& p_table)
  {
    return m_table_list.emplace(std::move(p_table)).first;
  }

  inline const_iterator end() const { return m_table_list.end(); }

  inline const_iterator begin() const { return m_table_list.begin(); }

  inline iterator end() { return m_table_list.end(); }

  inline iterator begin() { return m_table_list.begin(); }

  inline iterator find(const name& p_table_name) const
  {
    return std::find_if(m_table_list.begin(),
                        m_table_list.end(),
                        [&p_table_name](const table& p_table) -> bool {
                          return p_table.get_name() == p_table_name;
                        });
  }

private:
  table_list m_table_list;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLES_H

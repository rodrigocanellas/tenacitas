#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H

#include <algorithm>
#include <memory>
#include <set>

#include <sql/entities/column.h>
#include <sql/entities/name.h>

namespace capemisa {
namespace sql {
namespace entities {

struct columns
{

  typedef std::set<column> column_list;
  typedef column_list::iterator iterator;
  typedef column_list::const_iterator const_iterator;

  const_iterator add(column&& p_column)
  {
    return m_column_list.emplace(std::move(p_column)).first;
  }

  inline const_iterator find(const name& p_column_name) const
  {
    return std::find_if(m_column_list.begin(),
                        m_column_list.end(),
                        [&p_column_name](const column& p_column) -> bool {
                          return p_column.get_name() == p_column_name;
                        });
  }

  inline const_iterator begin() const { return m_column_list.begin(); }

  inline const_iterator end() const { return m_column_list.end(); }

private:
  column_list m_column_list;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_H

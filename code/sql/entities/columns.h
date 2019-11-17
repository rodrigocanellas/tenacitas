#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H

#include <algorithm>
#include <list>
#include <memory>

#include <sql/entities/collection.h>
#include <sql/entities/column.h>
#include <sql/entities/name.h>
#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

typedef collection<column> columns;

// struct columns
//{

//  typedef std::list<ptr<column>> column_list;
//  typedef column_list::iterator iterator;
//  typedef column_list::const_iterator const_iterator;

//  friend std::ostream& operator<<(std::ostream& p_out,
//                                  const columns& p_columns);

//  iterator add(ptr<column> p_column)
//  {
//    //    column_list::iterator _ite =
//    //      m_column_list.emplace(std::move(p_column)).first;
//    //    return _ite;
//    m_column_list.push_back(p_column);
//    return std::prev(m_column_list.end());
//  }

//  inline iterator find(const name& p_column_name)
//  {
//    return std::find_if(m_column_list.begin(),
//                        m_column_list.end(),
//                        [&p_column_name](const ptr<column>& p_column) -> bool
//                        {
//                          return p_column->get_name() == p_column_name;
//                        });
//  }

//  inline const_iterator begin() const { return m_column_list.begin(); }

//  inline const_iterator end() const { return m_column_list.end(); }

//  inline ptr<column> operator[](uint16_t p_index)
//  {
//    if (p_index >= m_column_list.size()) {
//      return ptr<column>();
//    }
//    return *(std::next(m_column_list.begin(), p_index));
//  }

// private:
//  column_list m_column_list;
//};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_H

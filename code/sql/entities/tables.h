#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>

#include <sql/entities/collection.h>
#include <sql/entities/table.h>
#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

typedef collection<table> tables;

// struct tables
//{

//  typedef std::list<ptr<table>> table_list;

//  typedef table_list::const_iterator const_iterator;
//  typedef table_list::iterator iterator;

//  friend std::ostream& operator<<(std::ostream& p_out, const tables&
//  p_tables);

//  inline iterator add(ptr<table> p_table)
//  {
//    //    table_list::iterator _ite =
//    //    m_table_list.emplace(std::move(p_table)).first;
//    m_table_list.push_back(p_table);
//    return std::prev(m_table_list.end());
//  }

//  inline const_iterator end() const { return m_table_list.end(); }

//  inline const_iterator begin() const { return m_table_list.begin(); }

//  inline iterator end() { return m_table_list.end(); }

//  inline iterator begin() { return m_table_list.begin(); }

//  inline iterator find(const name& p_table_name)
//  {
//    return std::find_if(m_table_list.begin(),
//                        m_table_list.end(),
//                        [&p_table_name](const ptr<table>& p_table) -> bool {
//                          return p_table->get_name() == p_table_name;
//                        });
//    //    m_table_list.
//  }

// private:
//  table_list m_table_list;
//};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLES_H

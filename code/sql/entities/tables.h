#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLES_H

#include <list>
#include <iterator>



namespace capemisa {
namespace sql {
namespace entities {

template <typename t_traits>
struct tables_t {
  typedef t_traits traits;
  typedef typename traits::table table;

  typedef typename traits::table_const_ptr table_const_ptr;
  typedef typename traits::table_ptr table_ptr;

  table_ptr add(table && p_table) {
    m_table_list.push_back(std::move(p_table));
    return std::prev(m_table_list.end());
  }

private:
  typedef std::list<table> table_list;
private:
  table_list m_table_list;

};


} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // TABLES_H

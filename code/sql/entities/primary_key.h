#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_H

#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/column.h>
#include <sql/entities/columns.h>
//#include <sql/entities/tables.h>


namespace capemisa {
namespace sql {
namespace entities {

template <typename t_table>
struct primary_key_t {

  typedef t_table table;
  typedef const table* const table_ptr;

  friend std::iostream & operator <<(std::iostream & p_out,
                                     const primary_key_t & p_pk) {
    p_out << "{ table : \"" << p_pk.get_table()->get_name() << "\", "
          << "\"columns\" : [";

  }

  primary_key_t()=delete;

  inline explicit primary_key_t(table_ptr p_table)
    : m_table(p_table) {}

  inline void add_column(columns::const_iterator p_column) {
    m_columns.push_back(p_column);
  }

  inline table_ptr get_table() const {
    return m_table;
  }

private:

  typedef std::list<columns::const_iterator> pk_columns;
private:
  table_ptr m_table;
  pk_columns m_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // PRIMARY_KEY_H

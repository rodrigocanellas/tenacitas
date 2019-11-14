#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_PRIMARY_KEY_T_H

#include <iostream>
#include <list>
#include <memory>

#include <sql/entities/name.h>


namespace capemisa {
namespace sql {
namespace entities {

template <typename t_traits>
struct primary_key_t {

  typedef t_traits traits;
  typedef typename traits::table_const_ptr table_const_ptr;
  typedef typename traits::column_const_ptr column_const_ptr;
  typedef typename std::list<column_const_ptr> pk_columns;
  typedef typename pk_columns::const_iterator pk_columns_const_iterator;

  friend std::ostream & operator <<(std::ostream & p_out,
                                    const primary_key_t & p_pk) {
    p_out  << "\"columns\": [";

    pk_columns_const_iterator _end = p_pk.end_columns();
    for (pk_columns_const_iterator  _ite = p_pk.begin_columns();
         _ite != _end;
         ++_ite) {
      p_out << *(*_ite);
      if (std::next(_ite) != _end) {
        p_out << ", ";
      }
    }
    p_out << "]";
    return p_out;
  }


  primary_key_t()=delete;

  inline explicit primary_key_t(table_const_ptr p_table,
                                const name & p_name="PK")
    : m_name(p_name),
      m_table(p_table) {}

  const name & get_name()const{return m_name;}
  void set_name(const name & p_name){m_name=p_name;}

  inline void add_column(column_const_ptr p_column) {
    m_columns.push_back(p_column);
  }

  inline pk_columns_const_iterator begin_columns() const {
    return m_columns.begin();
  }

  inline pk_columns_const_iterator end_columns() const {
    return m_columns.end();
  }

  inline table_const_ptr get_table() const {return m_table;}

private:


private:
  name m_name;
  table_const_ptr m_table;
  pk_columns m_columns;
};

} // namespace entities
} // namespace sql
} // namespace capemisa


#endif // primary_key_t_H

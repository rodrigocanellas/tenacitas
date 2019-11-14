#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_TABLE_H

#include <memory>
#include <list>
#include <functional>
#include <iostream>

//#include <sql/entities/column.h>
//#include <sql/entities/columns.h>
#include <sql/entities/name.h>
//#include <sql/entities/foreign_key.h>
//#include <sql/entities/foreign_keys.h>
//#include <sql/entities/primary_key.h>

namespace capemisa {
namespace sql {
namespace entities {

template<typename t_traits>
struct table_t {
  typedef t_traits traits;
  typedef typename traits::column column;
  typedef typename traits::columns columns;
  typedef typename traits::column_const_ptr column_const_ptr;
  typedef typename traits::primary_key primary_key;
  typedef typename traits::primary_key_const_ptr primary_key_const_ptr;

//  typedef primary_key_t<table>  primary_key;
//  typedef std::shared_ptr<primary_key> primary_key_ptr;
//  typedef foreign_key_t<table>  foreign_key;
//  typedef std::shared_ptr<foreign_key>  foreign_key_ptr;
//  typedef std::list<foreign_key_ptr>  foreign_keys;

  friend std::ostream & operator<<(std::ostream & p_out, const table_t & p_table) {
    p_out << "{ \"name\": \"" << p_table.get_name() << "\", "
          << "\"columns\": [";
    column_const_ptr  _end = p_table.end_column();
    for (column_const_ptr  _ite = p_table.begin_column();
         _ite != _end;
         ++_ite) {
      p_out << *_ite;
      if (std::next(_ite) != _end) {
        p_out << ",";
      }
    }
    p_out << "] ";

    p_out << ",\"pk\": {" << (*p_table.get_primary_key()) << "}";

    p_out << "}";
    return p_out;

  }

  table_t()=delete;

  table_t(const name & p_name)
    : m_name(p_name)
    ,m_primary_key(this)
  {}

  inline const name & get_name()const{
    return m_name;
  }

  inline void traverse_columns(std::function<void(const column &)> p_function) const {
    for (const column & _column : m_columns) {
      p_function(_column);
    }
  }

  column_const_ptr begin_column() const {
    return m_columns.begin();
  }

  column_const_ptr end_column() const {
    return m_columns.end();
  }

//  foreign_keys::const_iterator begin_foregign_key() const   {
//    return m_foreign_keys.begin();
//  }

//  foreign_keys::const_iterator end_foregign_key() const   {
//    return m_foreign_keys.end();
//  }

  inline primary_key_const_ptr get_primary_key() const {
    return &m_primary_key;
  }

  inline void add_column(column && p_column) {
    m_columns.add(std::move(p_column));
  }

  inline void add_to_primary_key(const name & p_column_name) {
    column_const_ptr _col = m_columns.find(p_column_name);
    if (_col != m_columns.end()) {
      m_primary_key.add_column(_col);
    }
  }

//  void add_foreign_key(foreign_key_ptr p_foreign_key) {
//    m_foreign_keys.push_back(p_foreign_key);
//  }

private:

private:
  name m_name;
  primary_key m_primary_key;
  columns m_columns;
//  foreign_keys m_foreign_keys;
};


} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_H

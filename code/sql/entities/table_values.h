#ifndef CAPEMISA_SQL_ENTITIES_TABLE_VALUES_H
#define CAPEMISA_SQL_ENTITIES_TABLE_VALUES_H

#include <sql/entities/column_values.h>
#include <sql/entities/table.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace entities {

struct table_values
{
  typedef generic::collection<column_values> list;

  table_values(generic::ptr<table> p_table)
    : m_table(p_table)
  {}

  inline void add(generic::ptr<column_values> p_column_values)
  {
    m_list.add(p_column_values);
  }

  inline const generic::name& get_name() const { return m_table->get_name(); }

  void merge(generic::ptr<table_values> p_table_values)
  {
    m_list.merge(p_table_values->m_list);
  }

  inline generic::ptr<table> get_table() const { return m_table; }

  inline uint16_t get_num_cols() const { return m_list.get_size<uint16_t>(); }

  inline generic::ptr<column_values> get_column_values(uint16_t p_col_index)
  {
    return m_list[p_col_index];
  }

private:
  generic::ptr<table> m_table;
  list m_list;
};

// struct table_values
//{

//  void add(column_values&& p_attrs)
//  {
//    m_attrs_list.push_back(std::move(p_attrs));
//  }

//  void add(column_values_list&& p_fks_values_list)
//  {
//    foreign_key_column_values_list ::const_iterator _end =
//      p_fks_values_list.end();
//    for (foreign_key_column_values_list ::const_iterator _ite =
//           p_fks_values_list.begin();
//         _ite != _end;
//         ++_ite) {
//      m_fks_list.push_back(std::move(*_ite));
//    }
//  }

//  void add(primary_key_column_values_list&& p_fks_values_list)
//  {
//    primary_key_column_values_list ::const_iterator _end =
//      p_fks_values_list.end();
//    for (primary_key_column_values_list ::const_iterator _ite =
//           p_fks_values_list.begin();
//         _ite != _end;
//         ++_ite) {
//      m_pks_list.push_back(std::move(*_ite));
//    }
//  }

//  inline attribute_column_values_list::const_iterator end_attrs() const
//  {
//    return m_attrs_list.end();
//  }

//  inline attribute_column_values_list::const_iterator begin_attrs() const
//  {
//    return m_attrs_list.begin();
//  }

//  inline foreign_key_column_values_list::const_iterator end_fks() const
//  {
//    return m_fks_list.end();
//  }

//  inline foreign_key_column_values_list::const_iterator begin_fks() const
//  {
//    return m_fks_list.begin();
//  }

//  inline primary_key_column_values_list::const_iterator end_pks() const
//  {
//    return m_pks_list.end();
//  }

//  inline primary_key_column_values_list::const_iterator begin_pks() const
//  {
//    return m_pks_list.begin();
//  }

//  inline uint16_t get_num_lines() const
//  {
//    return m_attrs_list.begin()->get_size();
//  }

//  inline uint16_t get_num_cols_attrs() const { return m_attrs_list.size(); }

//  inline const value& get_attr(uint16_t p_attr_index, uint16_t p_line) const
//  {
//    return std::next(m_attrs_list.begin(), p_attr_index)->get_value(p_line);
//  }

//  inline uint16_t get_num_cols_pks() const { return m_pks_list.size(); }

//  inline const value& get_pk(uint16_t p_attr_index, uint16_t p_line) const
//  {
//    return std::next(m_pks_list.begin(), p_attr_index)->get_value(p_line);
//  }

//  inline uint16_t get_num_cols_fks() const { return m_fks_list.size(); }

//  inline const value& get_fk(uint16_t p_attr_index, uint16_t p_line) const
//  {
//    return std::next(m_fks_list.begin(), p_attr_index)->get_value(p_line);
//  }

// private:
//  column_values_list m_attrs_list;
//  foreign_key_column_values_list m_fks_list;
//  primary_key_column_values_list m_pks_list;
//};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // TABLE_VALUES_H

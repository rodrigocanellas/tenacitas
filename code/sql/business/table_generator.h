#ifndef CAPEMISA_SQL_BUSINESS_TABLE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_TABLE_GENERATOR_H

#include <cstdint>
#include <list>
#include <map>
#include <utility>

#include <sql/business/attribute_column_generator.h>
#include <sql/business/foreign_key_generator.h>
#include <sql/business/primary_key_column_generator.h>
#include <sql/entities/attribute_column.h>
#include <sql/entities/foreign_key.h>
#include <sql/entities/foreign_key_column.h>
#include <sql/entities/primary_key.h>
#include <sql/entities/primary_key_column.h>
#include <sql/entities/table.h>
#include <sql/entities/table_values.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

struct table_generator
{
  typedef std::list<generic::name> tables_names;
  typedef generic::ptr<std::list<generic::name>> tables_names_ptr;
  typedef generic::ptr<entities::table_values> table_values_ptr;


  table_generator() = delete;

  explicit table_generator(generic::ptr<entities::table> p_table,
                           uint16_t p_num_lines)
    : m_table(p_table)
    , m_num_lines(p_num_lines)
  {}

  //  void operator()(const entities::table& p_table, uint16_t p_num_lines)
  //  {
  //    generate_pks(p_table, p_num_lines);
  //    generate_attrs(p_table, p_num_lines);
  //    generate_fks(p_table, p_num_lines);
  //  }

  void add_attr_generator(const generic::name& p_attr_col_name,
                          attribute_column_generator&& p_attr_col_gen)
  {
    m_attrs_generators.insert({ p_attr_col_name, std::move(p_attr_col_gen) });
  }

  void add_pk_generator(const generic::name& p_pk_col_name,
                        primary_key_column_generator&& p_pk_col_gen)
  {
    m_pks_generators.insert({ p_pk_col_name, std::move(p_pk_col_gen) });
  }

  void add_fk_generator(const generic::name& p_fk_name,
                        foreign_key_generator&& p_fk_gen)
  {
    m_fks_generators.insert({ p_fk_name, std::move(p_fk_gen) });
  }

  generic::ptr<const entities::table_values> generate_attrs()
  {
    using namespace sql::entities;
    using namespace sql::generic;
    ptr<table_values> _attrs_values(make_ptr<table_values>(m_table));

    uint16_t _num_attrs = m_table->get_num_attrs();
    for (uint8_t _count_attrs = 0; _count_attrs < _num_attrs; ++_count_attrs) {
      ptr<attribute_column> _col = m_table->get_attr(_count_attrs);
      attributes_generators::iterator _ite =
          m_attrs_generators.find(_col->get_name());
      if (_ite != m_attrs_generators.end()) {
        _attrs_values->add(_ite->second(_col, m_num_lines));
      }
    }
    return _attrs_values;
  }

  generic::ptr<const entities::table_values> generate_pks(
      generic::ptr<entities::tables_values> p_pks)
  {
    using namespace sql::entities;
    using namespace sql::generic;

    ptr<table_values> _pks_values(make_ptr<table_values>(m_table));

    ptr<primary_key> _pk = m_table->get_primary_key();

    uint16_t _num_pk_cols = _pk->get_num_pks_cols();
    for (uint8_t _count_pk_cols = 0; _count_pk_cols < _num_pk_cols;
         ++_count_pk_cols) {
      ptr<primary_key_column> _col = _pk->get_pk_column(_count_pk_cols);
      pks_generators::iterator _ite = m_pks_generators.find(_col->get_name());
      if (_ite != m_pks_generators.end()) {
        _pks_values->add(_ite->second(_col, m_num_lines));
      }
    }
    p_pks->add(_pks_values);
    return _pks_values;
  }

  std::pair<table_values_ptr, tables_names_ptr> generate_fks(
      generic::ptr<const entities::tables_values> p_pks)
  {

    using namespace sql::entities;
    using namespace sql::generic;


    tables_names_ptr _tables_names;
    uint16_t _num_fks = m_table->get_num_fks();
    for (uint16_t _count_fks = 0; _count_fks < _num_fks; ++_count_fks) {
      ptr<foreign_key> _fk = m_table->get_fk(_count_fks);
      fks_generators::iterator _ite = m_fks_generators.find(_fk->get_name());
      if (_ite != m_fks_generators.end()) {
        _tables_names = make_ptr<tables_names>(tables_not_populated(_fk, p_pks));
        //        if (!_tables_names.empty()) {
        //          _ite->second(p_pks, _fks_values, _fk, m_num_lines);
        //        }
      }
    }

    if (!_tables_names->empty()) {

      return {table_values_ptr(), _tables_names };
    }

    ptr<table_values> _fks_values(make_ptr<table_values>(m_table));

    for (uint16_t _count_fks = 0; _count_fks < _num_fks; ++_count_fks) {
      ptr<foreign_key> _fk = m_table->get_fk(_count_fks);
      fks_generators::iterator _ite = m_fks_generators.find(_fk->get_name());
      if (_ite != m_fks_generators.end()) {
        _ite->second(p_pks, _fks_values, _fk, m_num_lines);
      }
    }

    return {_fks_values, tables_names_ptr()};
  }

private:
  typedef std::map<generic::name, attribute_column_generator>
  attributes_generators;
  typedef std::map<generic::name, primary_key_column_generator> pks_generators;
  typedef std::map<generic::name, foreign_key_generator> fks_generators;

private:
  std::list<generic::name>
  tables_not_populated(generic::ptr<const entities::foreign_key> p_fk
                       , generic::ptr<const entities::tables_values> p_pks) const{
    using namespace sql::entities;
    using namespace sql::generic;

    std::list<generic::name> _tables_names;

    uint16_t _num_cols = p_fk->get_num_cols();
    for (uint16_t _count_col = 0; _count_col < _num_cols; ++_count_col) {
      ptr<foreign_key_column> _fk_col = p_fk->get_fk_col(_count_col);
      ptr<primary_key_column> _pk_col = _fk_col->get_pk_column();
      const primary_key& _pk = _pk_col->get_primary_key();
      const name& _table_name = _pk.get_table().get_name();

      ptr<table_values> _pk_table_values = p_pks->find(_table_name);
      if (_pk_table_values == nullptr) {
        _tables_names.push_back(_table_name);
      }
    }
    return _tables_names;
  }

private:
  generic::ptr<entities::table> m_table;
  uint16_t m_num_lines;

  pks_generators m_pks_generators;
  attributes_generators m_attrs_generators;
  fks_generators m_fks_generators;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // TABLE_GENERATOR_H

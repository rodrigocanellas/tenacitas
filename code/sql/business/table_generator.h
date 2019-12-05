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

  table_generator() = delete;

  explicit table_generator(generic::ptr<entities::table> p_table)
    : m_table(p_table)
    , m_attrs_values(generic::make_ptr<entities::table_values>(p_table))
    , m_pks_values(generic::make_ptr<entities::table_values>(p_table))
    , m_fks_values(generic::make_ptr<entities::table_values>(p_table))
  {}

  //  void operator()(const entities::table& p_table, uint16_t p_num_lines)
  //  {
  //    generate_pks(p_table, p_num_lines);
  //    generate_attrs(p_table, p_num_lines);
  //    generate_fks(p_table, p_num_lines);
  //  }

  void generate_attrs(uint16_t p_num_lines)
  {
    using namespace sql::entities;
    using namespace sql::generic;

    uint16_t _num_attrs = m_table->get_num_attrs();
    for (uint8_t _count_attrs = 0; _count_attrs < _num_attrs; ++_count_attrs) {
      ptr<attribute_column> _col = m_table->get_attr(_count_attrs);
      attributes_generators::iterator _ite =
        m_attrs_generators.find(_col->get_name());
      if (_ite != m_attrs_generators.end()) {
        m_attrs_values->add(_ite->second(_col, p_num_lines));
      }
    }
  }

  void generate_pks(uint16_t p_num_lines,
                    generic::ptr<entities::tables_values> p_pks)
  {
    using namespace sql::entities;
    using namespace sql::generic;

    ptr<primary_key> _pk = m_table->get_primary_key();

    uint16_t _num_pk_cols = _pk->get_num_pks_cols();
    for (uint8_t _count_pk_cols = 0; _count_pk_cols < _num_pk_cols;
         ++_count_pk_cols) {
      ptr<primary_key_column> _col = _pk->get_pk_column(_count_pk_cols);
      pks_generators::iterator _ite = m_pks_generators.find(_col->get_name());
      if (_ite != m_pks_generators.end()) {
        m_pks_values->add(_ite->second(_col, p_num_lines));
      }
    }
    p_pks->add(m_pks_values);
  }

  void generate_fks(uint16_t p_num_lines,
                    generic::ptr<const entities::tables_values>& p_pks)
  {

    using namespace sql::entities;
    using namespace sql::generic;

    uint16_t _num_fks = m_table->get_num_fks();
    for (uint16_t _count_fks = 0; _count_fks < _num_fks; ++_count_fks) {
      ptr<foreign_key> _fk = m_table->get_fk(_count_fks);
      fks_generators::iterator _ite = m_fks_generators.find(_fk->get_name());
      if (_ite != m_fks_generators.end()) {
        ptr<table_values> _fks(_ite->second(p_pks, m_table, _fk, p_num_lines));
        uint16_t _num_cols = _fks->get_num_cols();
        for (uint16_t _count_cols = 0; _count_cols < _num_cols; ++_count_cols) {
          m_fks_values->add(_fks->get_column_values(_count_cols));
        }
      }
    }
  }

  inline generic::ptr<const entities::table_values> get_pk_values() const
  {
    return m_pks_values;
  }

  inline generic::ptr<const entities::table_values> get_fk_values() const
  {
    return m_fks_values;
  }

  inline generic::ptr<const entities::table_values> get_attr_values() const
  {
    return m_attrs_values;
  }

private:
  typedef std::map<generic::name, attribute_column_generator>
    attributes_generators;
  typedef std::map<generic::name, primary_key_column_generator> pks_generators;
  typedef std::map<generic::name, foreign_key_generator> fks_generators;

private:
  generic::ptr<entities::table> m_table;

  generic::ptr<entities::table_values> m_attrs_values;
  generic::ptr<entities::table_values> m_pks_values;
  generic::ptr<entities::table_values> m_fks_values;

  pks_generators m_pks_generators;
  attributes_generators m_attrs_generators;
  fks_generators m_fks_generators;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // TABLE_GENERATOR_H

#ifndef CAPEMISA_SQL_BUSINESS_ONE_PK_ALL_FKS_H
#define CAPEMISA_SQL_BUSINESS_ONE_PK_ALL_FKS_H

#include <cstdint>

#include <sql/entities/primary_key.h>
#include <sql/entities/primary_key_column.h>
#include <sql/entities/table.h>
#include <sql/entities/table_values.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

struct one_pk_all_fks
{

  typedef generic::ptr<entities::table_values> table_values_ptr;
  typedef generic::ptr<const entities::tables_values> tables_values_const_ptr;
  typedef generic::ptr<entities::foreign_key> foreign_key_ptr;

  ///
  /// \brief foreign_key_generator
  ///
  /// \param p_pks are the values of the all the PK columns of all tables
  /// \param foreign_key_column_ptr FK to which colums values will be generated
  /// \param uint16_t is the amount of lines

  void operator()(tables_values_const_ptr p_pks,
                  table_values_ptr p_fk_values,
                  foreign_key_ptr p_fk,
                  uint16_t p_num_lines)
  {
    using namespace sql::entities;
    using namespace sql::generic;

    uint16_t _num_cols = p_fk->get_num_cols();
    for (uint16_t _count_col = 0; _count_col < _num_cols; ++_count_col) {
      ptr<foreign_key_column> _fk_col = p_fk->get_fk_col(_count_col);
      ptr<primary_key_column> _pk_col = _fk_col->get_pk_column();
      const name& _pk_col_name = _pk_col->get_name();
      const primary_key& _pk = _pk_col->get_primary_key();
      const name& _table_name = _pk.get_table().get_name();

      ptr<table_values> _pk_table_values = p_pks->find(_table_name);
      if (_pk_table_values != nullptr) {

        ptr<column_values> _pk_col_values = _pk_table_values->find(_pk_col_name);

        const value& _pk_col_value = _pk_col_values->get_value(0);

        ptr<column_values> _fk_col_values(make_ptr<column_values>(_fk_col));

        for (uint16_t _count_line = 0; _count_line < p_num_lines; ++_count_line) {
          _fk_col_values->add(_pk_col_value);
        }

        p_fk_values->add(_fk_col_values);
      }
    }
  }
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // ONE_PK_ALL_FKS_H

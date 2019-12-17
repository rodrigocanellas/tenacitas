#include <sql/business/rotate_pks_in_fks.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

const name rotate_pks_in_fks::id("rodar as PKs");

void
rotate_pks_in_fks::operator()(ptr<const tables_values> p_all_pks,
                              ptr<table_values> p_fks_columns_values,
                              ptr<foreign_key> p_fk,
                              uint16_t p_num_lines)
{
  uint16_t _num_cols = p_fk->get_num_cols();
  for (uint16_t _count_col = 0; _count_col < _num_cols; ++_count_col) {
    ptr<foreign_key_column> _fk_col = p_fk->get_fk_col(_count_col);
    ptr<primary_key_column> _pk_col = _fk_col->get_pk_column();
    const name& _pk_col_name = _pk_col->get_name();
    const primary_key& _pk = _pk_col->get_primary_key();
    const name& _table_name = _pk.get_table().get_name();

    ptr<table_values> _pk_table_values = p_all_pks->find(_table_name);
    if (_pk_table_values != nullptr) {

      ptr<column_values> _pk_col_values =
        _pk_table_values->find_column_values(_pk_col_name);

      //      const value& _pk_col_value = _pk_col_values->get_value(0);

      uint16_t _num_pks = _pk_col_values->get_size();
      uint16_t _count_pks = 0;

      ptr<column_values> _fk_col_values(make_ptr<column_values>(_fk_col));

      for (uint16_t _count_line = 0; _count_line < p_num_lines; ++_count_line) {
        if (_count_pks >= _num_pks) {
          _count_pks = 0;
        }
        _fk_col_values->add(_pk_col_values->get_value(_count_pks++));
      }

      p_fks_columns_values->add(_fk_col_values);
    }
  }
}

} // namespace business
} // namespace sql
} // namespace capemisa

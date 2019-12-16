#include <sql/business/internal/tables_not_populated.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace sql::entities;
using namespace sql::generic;
using namespace std;

tables_not_populated::tables_names_ptr
tables_not_populated::operator()(ptr<const entities::table> p_table,
                                 ptr<const entities::tables_values> p_pks)
{

  tables_names_ptr _tables_names_not_created(make_ptr<tables_names>());
  tables_names _tables_names_created;

  const name _this_table_name(p_table->get_name());

  m_queue.push(p_table.get());
  while (!m_queue.empty()) {
    const table* _table = m_queue.front();
    m_queue.pop();

    // inspect table
    name _table_name(_table->get_name());
    if (_table_name != _this_table_name) {
      ptr<table_values> _pk_table_values = p_pks->find(_table_name);
      if (_pk_table_values != nullptr) {
        if (!find(_tables_names_created, _table_name)) {
          _tables_names_created.push_back(_table_name);
        }
      } else if (!find(*_tables_names_not_created, _table_name)) {
        _tables_names_not_created->push_front(_table_name);
      }
    }

    // visit related tables
    uint16_t _num_fks = _table->get_num_fks();
    for (uint16_t _count_fks = 0; _count_fks < _num_fks; ++_count_fks) {
      ptr<foreign_key> _fk = _table->get_foreign_key(_count_fks);
      ptr<foreign_key_column> _fk_col = _fk->get_fk_col(0);
      ptr<primary_key_column> _pk_col = _fk_col->get_pk_column();
      const primary_key& _pk = _pk_col->get_primary_key();
      const table& _pk_table = _pk.get_table();
      const name& _pk_table_name = _pk_table.get_name();
      if ((_pk_table_name != _this_table_name) &&
          (!find(*_tables_names_not_created, _pk_table_name)) &&
          (!find(_tables_names_created, _pk_table_name))) {
        m_queue.push(&_pk_table);
      }
    }
  }

  return _tables_names_not_created;
}

} // namespace business
} // namespace sql
} // namespace capemisa

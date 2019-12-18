#include <sql/business/traverse_tables.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;
using namespace generic;

void
traverse_tables::operator()(const table * const p_table,
                            std::function<void(const table * const)> p_handle_visited)

{
  std::set<name> _visited;
  const name & _this_table_name = p_table->get_name();
  m_queue.push(p_table);
  while (!m_queue.empty()) {
    const table* const _table = m_queue.front();
    m_queue.pop();

    const name & _table_name = _table->get_name();

    if  (_table_name != _this_table_name) {
      p_handle_visited(_table);
    }

    // visit related tables
    uint16_t _num_fks = _table->get_num_fks();
    for (uint16_t _count_fks = 0; _count_fks < _num_fks; ++_count_fks) {
      ptr<foreign_key> _fk = _table->get_foreign_key(_count_fks);
      ptr<foreign_key_column> _fk_col = _fk->get_fk_col(0);
      ptr<primary_key_column> _pk_col = _fk_col->get_pk_column();
      const primary_key& _pk = _pk_col->get_primary_key();
      const table& _pk_table = _pk.get_table();
      const name &_pk_name = _pk_table.get_name();
      if (
          ( _pk_name != _this_table_name) &&
          (_visited.find(_pk_name) == _visited.end())
          ) {
        m_queue.push(&_pk_table);
      }
    }
  }
}

} // namespace business
} // namespace sql
} // namespace capemisa

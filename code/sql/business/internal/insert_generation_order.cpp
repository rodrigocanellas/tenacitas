#include <sql/business/insert_generation_order.h>

namespace capemisa {
namespace sql {
namespace business {

std::list<name> insert_generation_order::operator()(ptr<database> p_db) const {
  ptr<table> _table = p_db->get_table(0);
  std::list<name> _names;
  traverse_tables _traverse_tables;

  while (_table != nullptr) {
    _traverse_tables(_table.get(), [&_names](const table * const p_table) -> void {
      if (std::find_if(_names.begin(), _names.end(),
                       [p_table](const name & p_name) -> bool
      {
                       return p_table->get_name() == p_name;
    }) == _names.end()) {
        _names.push_back(p_table->get_name());
      }
    });


    _table = ptr<table>();
    for (uint16_t _i = 0; _i < p_db->get_num_tables();++_i) {
      name _aux = p_db->get_table(_i)->get_name();
      bool _found = false;
      for (const name & _name_2 : _names) {
        if ( _aux == _name_2) {
          _found = true;
          break;
        }
      }
      if (!_found) {
        _table = p_db->find(_aux);
        break;
      }
    }
  }
  return _names;

}


} // namespace business
} // namespace sql
} // namespace capemisa

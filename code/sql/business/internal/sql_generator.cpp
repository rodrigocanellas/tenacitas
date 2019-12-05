#include <sql/business/sql_generator.h>

namespace capemisa {
namespace sql {
namespace business {

std::string
sql_generator::operator()(generic::ptr<const entities::table_values> p_pks,
                          generic::ptr<const entities::table_values> p_attrs,
                          generic::ptr<const entities::table_values> p_fks)
{
  using namespace std;
  using namespace capemisa::sql::entities;
  using namespace capemisa::sql::generic;
  using namespace capemisa::sql::business;

  stringstream _stream;

  uint16_t _num_lines = 0;
  name _name;
  if ((p_pks != nullptr) && (p_pks->get_num_cols() > 0)) {
    _name = p_pks->get_name();
    _num_lines = p_pks->get_column_values(0)->get_size();
  } else if ((p_attrs != nullptr) && (p_attrs->get_num_cols() > 0)) {
    _name = p_attrs->get_name();
    _num_lines = p_attrs->get_column_values(0)->get_size();
  } else if ((p_fks != nullptr) && (p_fks->get_num_cols() > 0)) {
    _name = p_fks->get_name();
    _num_lines = p_fks->get_column_values(0)->get_size();
  }

  for (uint16_t _line_count = 0; _line_count < _num_lines; ++_line_count) {

    if ((p_pks != nullptr) && (p_pks->get_num_cols() > 0)) {
      uint16_t _num_cols = p_pks->get_num_cols();
      _stream << "insert into " << _name << " (";
      for (uint16_t _col_count = 0; _col_count < _num_cols; ++_col_count) {
        _stream << p_pks->get_column_values(_col_count)->get_name();
        if (_col_count != (_num_cols - 1)) {
          _stream << ", ";
        }
      }
    }

    if ((p_attrs != nullptr) && (p_attrs->get_num_cols() > 0)) {
      _stream << ", ";
      uint16_t _num_cols = p_attrs->get_num_cols();
      for (uint16_t _col_count = 0; _col_count < _num_cols; ++_col_count) {
        _stream << p_attrs->get_column_values(_col_count)->get_name();
        if (_col_count != (_num_cols - 1)) {
          _stream << ", ";
        }
      }
    }

    if ((p_fks != nullptr) && (p_fks->get_num_cols() > 0)) {
      _stream << ", ";
      uint16_t _num_cols = p_fks->get_num_cols();
      for (uint16_t _col_count = 0; _col_count < _num_cols; ++_col_count) {
        _stream << p_fks->get_column_values(_col_count)->get_name();
        if (_col_count != (_num_cols - 1)) {
          _stream << ", ";
        }
      }
    }

    _stream << ") values (";

    if ((p_pks != nullptr) && (p_pks->get_num_cols() > 0)) {
      uint16_t _num_cols = p_pks->get_num_cols();
      for (uint16_t _col_count = 0; _col_count < _num_cols; ++_col_count) {
        _stream << p_pks->get_column_values(_col_count)
                     ->get_value(_line_count)
                     .get_value();
        if (_col_count != (_num_cols - 1)) {
          _stream << ", ";
        }
      }
    }

    if ((p_attrs != nullptr) && (p_attrs->get_num_cols() > 0)) {
      _stream << ", ";
      uint16_t _num_cols = p_attrs->get_num_cols();
      for (uint16_t _col_count = 0; _col_count < _num_cols; ++_col_count) {
        _stream << p_attrs->get_column_values(_col_count)
                     ->get_value(_line_count)
                     .get_value();
        if (_col_count != (_num_cols - 1)) {
          _stream << ", ";
        }
      }
    }

    if ((p_fks != nullptr) && (p_fks->get_num_cols() > 0)) {
      _stream << ", ";
      uint16_t _num_cols = p_fks->get_num_cols();
      for (uint16_t _col_count = 0; _col_count < _num_cols; ++_col_count) {
        _stream << p_fks->get_column_values(_col_count)
                     ->get_value(_line_count)
                     .get_value();
        if (_col_count != (_num_cols - 1)) {
          _stream << ", ";
        }
      }
    }

    _stream << ")" << endl;
  }
  return _stream.str();
}

} // namespace business
} // namespace sql
} // namespace capemisa

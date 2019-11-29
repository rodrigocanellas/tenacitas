#include <sql/entities/column.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const column& p_column)
{
  p_out << "\"name\" : \"" << p_column.get_name() << "\", "
        << "\"type\" : \"" << p_column.type2str(p_column.get_type()) << "\", "
        << "\"size\" : \"" << p_column.get_size() << "\", "
        << "\"category\" : \"" << category2str(p_column.get_category()) << "\"";
  return p_out;
}

size
column::type2size(column::type p_type)
{
  switch (p_type) {
    case type::INT_1:
      return 1;
    case type::INT_2:
      return 2;
    case type::INT_4:
      return 4;
    case type::INT_8:
      return 8;
    case type::DATE:
      return 4;
    case type::DATE_TIME:
      return 4;
    case type::SMALL_REAL:
      return sizeof(float);
    case type::LONG_REAL:
      return sizeof(double);
    case type::CHAR:
      return 1;
    default:
      return -1;
  }
}

std::string
column::type2str(column::type p_type)
{
  switch (p_type) {
    case type::INT_1:
      return "INT_1";
    case type::INT_2:
      return "INT_2";
    case type::INT_4:
      return "INT_4";
    case type::INT_8:
      return "INT_8";
    case type::DATE:
      return "DATE";
    case type::DATE_TIME:
      return "DATE_TIME";
    case type::SMALL_REAL:
      return "SMALL_REAL";
    case type::LONG_REAL:
      return "LONG_REAL";
    case type::CHAR:
      return "CHAR";
    case type::FIXED_SIZE_CHAR:
      return "FIXED_SIZE_CHAR";
    case type::VAR_SIZE_CHAR:
      return "VAR_SIZE_CHAR";
    case type::BLOB:
      return "BLOB";
    default:
      return "UNDEFINED";
  }
}

// table*
// column::get_table()
//{
//  return m_table;
//}

// void
// column::set_fixed_size_string(const std::string& p_value)
//{
//  if (m_type != type::FIXED_SIZE_CHAR) {
//    throw std::runtime_error("column is not FIXED_SIZE_CHAR type");
//  }
//  size _size = static_cast<size>(p_value.size());
//  if (_size > m_size) {
//    m_value = p_value.substr(0, m_size);
//  } else {
//    m_value = p_value;
//    if (_size < m_size) {
//      m_value.append(m_size - _size, ' ');
//    }
//  }
//}

// void
// column::set_var_size_string(const std::string& p_value)
//{
//  if (m_type != type::VAR_SIZE_CHAR) {
//    throw std::runtime_error("column is not VAR_SIZE_CHAR type");
//  }

//  size _size = static_cast<size>(p_value.size());
//  if (_size > m_size) {
//    m_value = p_value.substr(0, m_size);
//  } else {
//    m_value = p_value;
//  }
//}

// void
// column::set_blob(std::string&& p_blob)
//{

//  if (m_type != type::BLOB) {
//    throw std::runtime_error("column is not BLOB type");
//  }

//  m_size = static_cast<size>(p_blob.size());
//  m_value = std::move(p_blob);
//}

// void
// column::set_date(time_t p_time)
//{
//  if (m_type != type::DATE) {
//    throw std::runtime_error("column is not DATE type");
//  }

//  m_value = std::to_string(p_time);
//}

// void
// column::set_datetime(time_t p_time)
//{
//  if (m_type != type::DATE_TIME) {
//    throw std::runtime_error("column is not DATE_TIME type");
//  }

//  m_value = std::to_string(p_time);
//}

// void
// column::set_char(char p_char)
//{
//  if (m_type != type::CHAR) {
//    throw std::runtime_error("column is not CHAR type");
//  }
//  m_value = std::string(1, p_char);
//}

} // namespace entities
} // namespace sql
} // namespace capemisa

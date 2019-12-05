#include <sql/entities/column.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const column& p_column)
{
  p_out << "\"name\" : \"" << p_column.get_name() << "\", "
        << "\"type\" : \"" << column::type2str(p_column.get_type()) << "\", "
        << "\"size\" : \"" << p_column.get_size() << "\"";
  return p_out;
}

// const column::type column::type::undefined(0);
// const column::type column::type::int_1(1);
// const column::type column::type::int_2(2);
// const column::type column::type::int_4(4);
// const column::type column::type::int_8(8);
// const column::type column::type::single_char(16);
// const column::type column::type::fixed_size_text(32);
// const column::type column::type::var_size_text(64);
// const column::type column::type::date(128);
// const column::type column::type::date_time(256);
// const column::type column::type::blob(512);
// const column::type column::type::small_real(1024);
// const column::type column::type::long_real(2048);

// size
// column::type::get_size() const
//{
//  if (m_value == type::int_1.m_value) {
//    return 1;
//  }
//  if (m_value == type::int_2.m_value) {
//    return 2;
//  }
//  if (m_value == type::int_4.m_value) {
//    return 4;
//  }
//  if (m_value == type::int_8.m_value) {
//    return 8;
//  }
//  if (m_value == type::date.m_value) {
//    return 4;
//  }
//  if (m_value == type::date_time.m_value) {
//    return 4;
//  }
//  if (m_value == type::small_real.m_value) {
//    return sizeof(float);
//  }
//  if (m_value == type::long_real.m_value) {
//    return sizeof(double);
//  }
//  if (m_value == type::single_char.m_value) {
//    return 1;
//  }
//  return -1;
//}

// std::string
// column::type::to_str() const
//{
//  if (m_value == type::int_1.m_value) {
//    return "int_1";
//  }
//  if (m_value == type::int_2.m_value) {
//    return "int_2";
//  }
//  if (m_value == type::int_4.m_value) {
//    return "int_4";
//  }
//  if (m_value == type::int_8.m_value) {
//    return "int_8";
//  }
//  if (m_value == type::date.m_value) {
//    return "date";
//  }
//  if (m_value == type::date_time.m_value) {
//    return "date_time";
//  }
//  if (m_value == type::small_real.m_value) {
//    return "small_real";
//  }
//  if (m_value == type::long_real.m_value) {
//    return "long_real";
//  }
//  if (m_value == type::single_char.m_value) {
//    return "single_char";
//  }
//  return "undefined";
//}

// const column::usage column::usage::primary_key(2);
// const column::usage column::usage::foreign_key(4);
// const column::usage column::usage::attribute(8);

// std::string
// column::usage::to_str() const
//{
//  if (m_value == usage::attribute.m_value) {
//    return "attribute";
//  }
//  if (m_value == usage::primary_key.m_value) {
//    return "primary_key";
//  }
//  return "foreign_key";
//}

size
column::type2size(column::type p_type)
{
  //  return p_type.get_size();

  if (p_type == type::int_1) {
    return 1;
  }
  if (p_type == type::int_2) {
    return 2;
  }
  if (p_type == type::int_4) {
    return 4;
  }
  if (p_type == type::int_8) {
    return 8;
  }
  if (p_type == type::date) {
    return 4;
  }
  if (p_type == type::date_time) {
    return 4;
  }
  if (p_type == type::small_real) {
    return sizeof(float);
  }
  if (p_type == type::long_real) {
    return sizeof(double);
  }
  if (p_type == type::single_char) {
    return 1;
  }
  return -1;
}

std::string
column::type2str(column::type p_type)
{
  //  return p_type.to_str();
  if (p_type == type::int_1) {
    return "int_1";
  }
  if (p_type == type::int_2) {
    return "int_2";
  }
  if (p_type == type::int_4) {
    return "int_4";
  }
  if (p_type == type::int_8) {
    return "int_8";
  }
  if (p_type == type::date) {
    return "date";
  }
  if (p_type == type::date_time) {
    return "date_time";
  }
  if (p_type == type::small_real) {
    return "small_real";
  }
  if (p_type == type::long_real) {
    return "long_real";
  }
  if (p_type == type::single_char) {
    return "single_char";
  }
  return "undefined";
}

// std::string
// column::usage2str(usage p_usage)
//{
//  return p_usage.to_str();
//}

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

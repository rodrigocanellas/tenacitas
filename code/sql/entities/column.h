#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMN_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMN_H

#include <iostream>
#include <cstdint>
#include <ctime>

#include <sql/entities/name.h>
#include <sql/entities/size.h>

namespace capemisa {
namespace sql {
namespace entities {

template <typename t_traits>
struct column_t {

  friend std::ostream  & operator<<(std::ostream & p_out,
                                     const column_t & p_column) {
    p_out << "{ \"name\" : \"" << p_column.get_name() << "\", "
          << "\"type\" : \"" << p_column.type2str(p_column.get_type()) << "\", "
          << "\"size\" : \"" <<p_column.get_size() << "\", "
          << "\"value\" : \"" << p_column.get_value() << "\" }";
    return p_out;
  }

  enum class type : int8_t {
    UNDEFINED = 0,
    INT_1 = 1,
    INT_2 = 2,
    INT_4 = 3,
    INT_8 = 4,
    CHAR = 5,
    FIXED_SIZE_CHAR = 6,
    VAR_SIZE_CHAR = 7,
    DATE = 8,
    DATE_TIME = 9,
    BLOB = 10,
    SMALL_REAL = 11,
    LONG_REAL = 12
  };

  column_t()=delete;
  column_t(const name & p_name)
    : m_name(p_name),
      m_type(type::UNDEFINED),
      m_size(type2size(m_type)){}

  inline const name & get_name()const{return m_name;}

  inline type get_type() const {return m_type;}

  inline size get_size() const {return m_size;}

  template<typename t_int>
  void set_int(t_int p_value) {
    m_type = int2type<t_int>();
    m_size = type2size(m_type);
    m_value = std::to_string(p_value);
  }

  template<typename t_real>
  void set_real(t_real p_value) {
    m_type = real2type<t_real>();
    m_size = type2size(m_type);
    m_value = std::to_string(p_value);
  }

  void set_fixed_size_string(const std::string & p_value) {
    m_type = type::FIXED_SIZE_CHAR;
    m_size = static_cast<size>(p_value.size());
    m_value = p_value;
  }

  void set_var_size_string(const std::string & p_value) {
    m_type = type::VAR_SIZE_CHAR;
    m_size = static_cast<size>(p_value.size());
    m_value = p_value;
  }

  void set_blob(std::string && p_blob) {
    m_type = type::BLOB;
    m_size = static_cast<size>(p_blob.size());
    m_value = std::move(p_blob);
  }

  void set_date(time_t p_time) {
    m_type = type::DATE;
    m_size = type2size(m_type);
    m_value = std::to_string(p_time);
  }

  void set_datetime(time_t p_time) {
    m_type = type::DATE_TIME;
    m_size = type2size(m_type);
    m_value = std::to_string(p_time);
  }

  void set_char(char p_char) {
    m_type = type::CHAR;
    m_size = type2size(m_type);
    m_value = std::string(1, p_char);
  }


  const std::string & get_value() const {
    return m_value;
  }

  std::string type2str(type p_type)const {
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


private:


  size type2size(type p_type) {
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
        return sizeof (float);
      case type::LONG_REAL:
        return sizeof (double);
      case type::CHAR:
        return 1;
      default:
        return -1;
    }
  }

  template<typename t_int>
  type int2type() {
    if (sizeof (t_int) == sizeof (int8_t)) {
      return type::INT_1;
    }
    if (sizeof (t_int) == sizeof (int16_t)) {
      return type::INT_2;
    }
    if (sizeof (t_int) == sizeof (int32_t)) {
      return type::INT_4;
    }
    return type::INT_8;
  }


  template<typename t_real>
  type real2type() {
    if (sizeof (t_real) == sizeof (float)) {
      return type::SMALL_REAL;
    }
    return type::LONG_REAL;
  }

private:
  name m_name;
  type m_type;
  size m_size;
  bool m_is_null = {false};
  bool m_is_auto_increment={false};
  std::string m_value;

};


} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // NAME_H

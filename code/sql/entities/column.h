#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMN_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMN_H

#include <cstdint>
#include <ctime>
#include <iostream>

#include <sql/entities/name.h>
#include <sql/entities/size.h>

namespace capemisa {
namespace sql {
namespace entities {

struct column
{

  friend std::ostream& operator<<(std::ostream& p_out, const column& p_column)
  {
    p_out << "{ \"name\" : \"" << p_column.get_name() << "\", "
          << "\"type\" : \"" << p_column.type2str(p_column.get_type()) << "\", "
          << "\"size\" : \"" << p_column.get_size() << "\", "
          << "\"value\" : \"" << p_column.get_value() << "\" }";
    return p_out;
  }

  enum class type : int8_t
  {
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

  column() = delete;

  explicit inline column(const name& p_name)
    : m_name(p_name)
    , m_type(type::UNDEFINED)
    , m_size(type2size(m_type))
  {}

  inline const name& get_name() const { return m_name; }

  inline type get_type() const { return m_type; }

  inline size get_size() const { return m_size; }

  template<typename t_int>
  void set_int(t_int p_value)
  {
    m_type = int2type<t_int>();
    m_size = type2size(m_type);
    m_value = std::to_string(p_value);
  }

  template<typename t_real>
  void set_real(t_real p_value)
  {
    m_type = real2type<t_real>();
    m_size = type2size(m_type);
    m_value = std::to_string(p_value);
  }

  void set_fixed_size_string(const std::string& p_value);

  void set_var_size_string(const std::string& p_value);

  void set_blob(std::string&& p_blob);

  void set_date(time_t p_time);

  void set_datetime(time_t p_time);

  void set_char(char p_char);

  inline const std::string& get_value() const { return m_value; }

  std::string type2str(type p_type) const;

  inline bool operator==(const column& p_column) const
  {
    return m_name == p_column.get_name();
  }

  inline bool operator<(const column& p_column) const
  {
    return m_name < p_column.get_name();
  }

private:
  size type2size(type p_type);

  template<typename t_int>
  type int2type()
  {
    if (sizeof(t_int) == sizeof(int8_t)) {
      return type::INT_1;
    }
    if (sizeof(t_int) == sizeof(int16_t)) {
      return type::INT_2;
    }
    if (sizeof(t_int) == sizeof(int32_t)) {
      return type::INT_4;
    }
    return type::INT_8;
  }

  template<typename t_real>
  type real2type()
  {
    if (sizeof(t_real) == sizeof(float)) {
      return type::SMALL_REAL;
    }
    return type::LONG_REAL;
  }

private:
  name m_name;
  type m_type;
  size m_size;
  //  bool m_is_null = {false};
  //  bool m_is_auto_increment={false};
  std::string m_value;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // NAME_H

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

struct table;

struct column
{

  friend std::ostream& operator<<(std::ostream& p_out, const column& p_column);

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

  explicit inline column(table* p_table,
                         const name& p_name,
                         type p_type,
                         size p_size)
    : m_table(p_table)
    , m_name(p_name)
    , m_type(p_type)
    , m_size(p_size)
  {}

  explicit inline column(table* p_table, const name& p_name, type p_type)
    : m_table(p_table)
    , m_name(p_name)
    , m_type(p_type)
    , m_size(column::type2size(m_type))
  {}

  table* get_table();

  inline const name& get_name() const { return m_name; }

  inline type get_type() const { return m_type; }

  inline size get_size() const { return m_size; }

  const std::string& get_value() const { return m_value; }

  inline bool get_autoincrement() const { return m_is_auto_increment; }

  inline bool get_is_null() { return m_is_null; }

  // values

  template<typename t_int>
  void set_int(t_int p_value)
  {
    if ((m_type != type::INT_1) && (m_type != type::INT_2) &&
        (m_type != type::INT_4) && (m_type != type::INT_8)) {
      throw std::runtime_error("type is not integer");
    }

    if ((m_type == type::INT_1) && (sizeof(t_int) != 1)) {
      throw std::runtime_error("integer size is not 1");
    }

    if ((m_type == type::INT_2) && (sizeof(t_int) != 2)) {
      throw std::runtime_error("integer size is not 2");
    }

    if ((m_type == type::INT_4) && (sizeof(t_int) != 4)) {
      throw std::runtime_error("integer size is not 4");
    }

    if ((m_type == type::INT_8) && (sizeof(t_int) != 8)) {
      throw std::runtime_error("integer size is not 8");
    }
    m_value = std::to_string(p_value);
  }

  template<typename t_real>
  void set_real(t_real p_value = 0.0)
  {
    if ((m_type != type::SMALL_REAL) && (m_type != type::LONG_REAL)) {
      throw std::runtime_error("type is not real");
    }

    if ((m_type == type::SMALL_REAL) && (sizeof(t_real) != sizeof(float))) {
      throw("real size is not " + type2str(type::SMALL_REAL));
    }

    if ((m_type == type::LONG_REAL) && (sizeof(t_real) != sizeof(double))) {
      throw("real size is not " + type2str(type::LONG_REAL));
    }
    m_value = std::to_string(p_value);
  }

  void set_fixed_size_string(const std::string& p_value);

  void set_var_size_string(const std::string& p_value);

  void set_blob(std::string&& p_blob);

  void set_date(time_t p_time);

  void set_datetime(time_t p_time);

  void set_char(char p_char);

  inline void set_value(const std::string& p_value) { m_value = p_value; }

  std::string type2str(type p_type) const;

  void set_autoincrement(bool p_value);

  void set_is_null(bool p_value);

private:
  static size type2size(type p_type);

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
  table* m_table;
  name m_name;
  type m_type;
  size m_size;
  bool m_is_null = { false };
  bool m_is_auto_increment = { false };
  std::string m_value;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // NAME_H
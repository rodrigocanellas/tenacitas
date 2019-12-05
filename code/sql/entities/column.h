#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_H

#include <cstdint>
#include <ctime>
#include <iostream>

#include <sql/entities/size.h>
#include <sql/generic/name.h>

namespace capemisa {
namespace sql {
namespace entities {

struct column
{

  friend std::ostream& operator<<(std::ostream& p_out, const column& p_column);

  enum class type : int8_t
  {
    undefined = 0,
    int_1 = 1,
    int_2 = 2,
    int_4 = 3,
    int_8 = 4,
    single_char = 5,
    fixed_size_text = 6,
    var_size_text = 7,
    date = 8,
    date_time = 9,
    blob = 10,
    small_real = 11,
    long_real = 12
  };

  //  struct type
  //  {
  //    type() = delete;
  //    type(const type&) = default;
  //    type(type&&) = default;
  //    type& operator=(const type&) = default;
  //    type& operator=(type&&) = default;
  //    ~type() = default;

  //    static const type undefined;
  //    static const type int_1;
  //    static const type int_2;
  //    static const type int_4;
  //    static const type int_8;
  //    static const type single_char;
  //    static const type fixed_size_text;
  //    static const type var_size_text;
  //    static const type date;
  //    static const type date_time;
  //    static const type blob;
  //    static const type small_real;
  //    static const type long_real;

  //    inline type operator|(type p_type) const
  //    {
  //      return type(m_value | p_type.m_value);
  //    }

  //    inline bool operator&(type p_type) const
  //    {
  //      return p_type.m_value & m_value;
  //    }

  //    size get_size() const;

  //    std::string to_str() const;

  //  private:
  //    type(int16_t p_value)
  //      : m_value(p_value)
  //    {}

  //  private:
  //    int16_t m_value;
  //  };

  //  enum class usage : int8_t
  //  {
  //    primary_key = 0x02,
  //    foreign_key = 0x04,
  //    attribute = 0x08
  //  };

  //  struct usage
  //  {

  //    usage() = delete;
  //    usage(const usage&) = default;
  //    usage(usage&&) = default;
  //    usage& operator=(const usage&) = default;
  //    usage& operator=(usage&&) = default;
  //    ~usage() = default;

  //    static const usage primary_key;
  //    static const usage foreign_key;
  //    static const usage attribute;

  //    inline usage operator|(usage p_usage) const
  //    {
  //      return usage(m_value | p_usage.m_value);
  //    }

  //    inline bool operator&(usage p_usage) const
  //    {
  //      return p_usage.m_value & m_value;
  //    }

  //    std::string to_str() const;

  //  private:
  //    usage(int8_t p_value)
  //      : m_value(p_value)
  //    {}

  //  private:
  //    int8_t m_value;
  //  };

  column() = delete;
  column(const column&) = default;
  column(column&&) = delete;
  column& operator=(const column&) = default;
  column& operator=(column&&) = delete;
  virtual ~column() = default;

  inline const generic::name& get_name() const { return m_name; }

  inline type get_type() const { return m_type; }

  inline size get_size() const { return m_size; }

  //  inline usage get_usage() const { return m_usage; }

  //  bool type_compatible(type p_type) const {
  // if ( (m_type == type::int_1) ||
  // return false;
  //  }

  static std::string type2str(type p_type);
  //  static std::string usage2str(usage p_usage);

  //  // values

  //  template<typename t_int>
  //  void set_int(t_int p_value)
  //  {
  //    if ((m_type != type::INT_1) && (m_type != type::INT_2) &&
  //        (m_type != type::INT_4) && (m_type != type::INT_8)) {
  //      throw std::runtime_error("type is not integer");
  //    }

  //    if ((m_type == type::INT_1) && (sizeof(t_int) != 1)) {
  //      throw std::runtime_error("integer size is not 1");
  //    }

  //    if ((m_type == type::INT_2) && (sizeof(t_int) != 2)) {
  //      throw std::runtime_error("integer size is not 2");
  //    }

  //    if ((m_type == type::INT_4) && (sizeof(t_int) != 4)) {
  //      throw std::runtime_error("integer size is not 4");
  //    }

  //    if ((m_type == type::INT_8) && (sizeof(t_int) != 8)) {
  //      throw std::runtime_error("integer size is not 8");
  //    }
  //    m_value = std::to_string(p_value);
  //  }

  //  template<typename t_real>
  //  void set_real(t_real p_value = 0.0)
  //  {
  //    if ((m_type != type::SMALL_REAL) && (m_type != type::LONG_REAL)) {
  //      throw std::runtime_error("type is not real");
  //    }

  //    if ((m_type == type::SMALL_REAL) && (sizeof(t_real) != sizeof(float))) {
  //      throw("real size is not " + type2str(type::SMALL_REAL));
  //    }

  //    if ((m_type == type::LONG_REAL) && (sizeof(t_real) != sizeof(double))) {
  //      throw("real size is not " + type2str(type::LONG_REAL));
  //    }
  //    m_value = std::to_string(p_value);
  //  }

  //  void set_fixed_size_string(const std::string& p_value);

  //  void set_var_size_string(const std::string& p_value);

  //  void set_blob(std::string&& p_blob);

  //  void set_date(time_t p_time);

  //  void set_datetime(time_t p_time);

  //  void set_char(char p_char);

  //  inline void set_value(const std::string& p_value) { m_value = p_value; }

protected:
  explicit inline column(const generic::name& p_name, type p_type, size p_size)
    : m_name(p_name)
    , m_type(p_type)
    , m_size(p_size)
  {}

  explicit inline column(const generic::name& p_name, type p_type)
    : m_name(p_name)
    , m_type(p_type)
    , m_size(column::type2size(m_type))
  {}

private:
  static size type2size(type p_type);

  template<typename t_int>
  type int2type()
  {
    if (sizeof(t_int) == sizeof(int8_t)) {
      return type::int_1;
    }
    if (sizeof(t_int) == sizeof(int16_t)) {
      return type::int_2;
    }
    if (sizeof(t_int) == sizeof(int32_t)) {
      return type::int_4;
    }
    return type::int_8;
  }

  template<typename t_real>
  type real2type()
  {
    if (sizeof(t_real) == sizeof(float)) {
      return type::small_real;
    }
    return type::long_real;
  }

private:
  generic::name m_name;
  type m_type;
  size m_size;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // NAME_H

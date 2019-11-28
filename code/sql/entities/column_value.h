#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_VALUE_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_VALUE_H

#include <string>

namespace capemisa {
namespace sql {
namespace entities {

struct column_value
{

  column_value() = delete;

  column_value(const column_value&) = default;
  column_value(column_value&&) noexcept = default;

  ~column_value() = default;

  inline column_value(const std::string& p_value, bool p_is_user_input = false)
    : m_value(p_value)
    , m_is_user_input(p_is_user_input)
  {}

  inline column_value(std::string&& p_value, bool p_is_user_input = false)
    : m_value(std::move(p_value))
    , m_is_user_input(p_is_user_input)
  {}

  inline column_value(const char* p_value, bool p_is_user_input = false)
    : m_value(p_value)
    , m_is_user_input(p_is_user_input)
  {}

  inline std::string get_value() const { return m_value; }

  inline bool is_user_input() const { return m_is_user_input; }

  inline void set_value(const std::string& p_value)
  {
    m_value = p_value;
    m_is_user_input = true;
  }

  inline void set_value(std::string&& p_value)
  {
    m_value = std::move(p_value);
    m_is_user_input = true;
  }

  inline void set_value(const char* p_value)
  {
    m_value = std::move(p_value);
    m_is_user_input = true;
  }

  column_value& operator=(const column_value&) = default;
  column_value& operator=(column_value&&) noexcept = default;

private:
  std::string m_value;
  bool m_is_user_input = false;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMN_HEADER_H

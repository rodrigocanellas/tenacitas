#ifndef CAPEMISA_SQL_ENTITIES_COLUMN_VALUE_H
#define CAPEMISA_SQL_ENTITIES_COLUMN_VALUE_H

#include <string>

namespace capemisa {
namespace sql {
namespace entities {

struct value
{

  value() = delete;

  value(const value&) = default;
  value(value&&) noexcept = default;

  ~value() = default;

  inline value(const std::string& p_value, bool p_is_user_input = false)
    : m_value(p_value)
    , m_is_user_input(p_is_user_input)
  {}

  inline value(std::string&& p_value, bool p_is_user_input = false)
    : m_value(std::move(p_value))
    , m_is_user_input(p_is_user_input)
  {}

  inline value(const char* p_value, bool p_is_user_input = false)
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

  value& operator=(const value&) = default;
  value& operator=(value&&) noexcept = default;

private:
  std::string m_value;
  bool m_is_user_input = false;
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMN_HEADER_H

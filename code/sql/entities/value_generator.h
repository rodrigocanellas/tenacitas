#ifndef CAPEMISA_SQL_ENTITIES_VALUE_PATTERN_H
#define CAPEMISA_SQL_ENTITIES_VALUE_PATTERN_H

#include <cstdint>
#include <list>
#include <string>

#include <sql/entities/column.h>
#include <sql/entities/values.h>

namespace capemisa {
namespace sql {
namespace entities {

struct value_generator
{
  value_generator() = delete;

  virtual values generate(uint16_t p_num_values) = 0;
  virtual ~value_generator() {}

  inline std::string id() const { return m_id; }

  inline const std::string& explanation() const { return m_explanation; }

  inline column::usage get_usage() const { return m_usage; }

  bool operator==(const value_generator& p_value_generator) const
  {
    return m_id == p_value_generator.m_id;
  }

protected:
  explicit inline value_generator(std::string&& p_id,
                                  std::string&& p_explanation,
                                  column::usage p_usage)
    : m_id(p_id)
    , m_explanation(p_explanation)
    , m_usage(p_usage)
  {}

private:
  std::string m_id;
  std::string m_explanation;
  column::usage m_usage;
};

// enum class attribure_value_pattern : int8_t
//{
//  user_input = 1,
//  random = 2,
//  algorithm_generated = 3,
//  auto_increment = 4,
//  null_value = 5,
//  inferior_limit = 6,
//  superior_limit = 7
//};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // VALUE_PATTERN_H

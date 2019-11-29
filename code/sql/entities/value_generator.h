#ifndef CAPEMISA_SQL_ENTITIES_VALUE_PATTERN_H
#define CAPEMISA_SQL_ENTITIES_VALUE_PATTERN_H

#include <cstdint>
#include <string>

namespace capemisa {
namespace sql {
namespace entities {

struct value_generator {
  virtual std::string operator()()=0;
};

//enum class attribure_value_pattern : int8_t
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

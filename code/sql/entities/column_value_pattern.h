#ifndef CAPEMISA_SQL_ENTITIES_VALUE_PATTERN_H
#define CAPEMISA_SQL_ENTITIES_VALUE_PATTERN_H

#include <cstdint>

namespace capemisa {
namespace sql {
namespace entities {

enum class column_value_pattern : int8_t {
  user_input = 1,
  random = 2,
  algorithm_generated = 3
};

} // namespace entities
} // namespace sql
} // namespace capemisa



#endif // VALUE_PATTERN_H

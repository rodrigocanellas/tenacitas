#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_VALUE_PATTERN_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGN_KEY_VALUE_PATTERN_H

#include <cstdint>

namespace capemisa {
namespace sql {
namespace entities {

enum class foreign_key_value_pattern : int8_t {
  one_per_table_line = 1,
  one_for_all_table_lines = 2
};

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEY_AMOUNT_H

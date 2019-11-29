#ifndef CAPEMISA_SQL_ENTITIES_CATEGORY_H
#define CAPEMISA_SQL_ENTITIES_CATEGORY_H

#include <cstdint>
#include <string>

namespace capemisa {
namespace sql {
namespace entities {

enum class category : int8_t {
  primary_key = 0x02,
  foreign_key = 0x04,
  attribute = 0x08
};

static inline std::string category2str(category p_category) {
  if (p_category == category::attribute) {
    return "attribute";
  }
  if (p_category == category::primary_key) {
    return "primary_key";
  }
  return "foreign_key";
}

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // CATEGORY_H

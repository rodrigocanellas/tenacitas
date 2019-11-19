#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGNERS_KEYS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_FOREIGNERS_KEYS_H

#include <iostream>
#include <list>

#include <sql/entities/internal/collection.h>
#include <sql/entities/foreign_key.h>

namespace capemisa {
namespace sql {
namespace entities {

typedef collection<foreign_key> foreigners_keys;

} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEYS_H

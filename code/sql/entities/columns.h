#ifndef CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H
#define CAPEMISA_TEST_AUTOMATION_ENTITIES_COLUMNS_H

#include <algorithm>
#include <list>
#include <memory>

#include <sql/entities/internal/collection.h>
#include <sql/entities/column.h>
#include <sql/entities/name.h>
#include <sql/entities/types.h>

namespace capemisa {
namespace sql {
namespace entities {

typedef collection<column> columns;


} // namespace entities
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_H

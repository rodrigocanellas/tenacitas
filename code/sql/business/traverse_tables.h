#ifndef CAPEMISA_SQL_BUSINESS_TRAVERSE_TABLES_H
#define CAPEMISA_SQL_BUSINESS_TRAVERSE_TABLES_H

#include <algorithm>
#include <set>
#include <queue>

#include <sql/entities/foreign_key.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;
using namespace generic;

struct traverse_tables {

  void operator()(const table * const p_table,
                  std::function<void(const table * const)> p_handle_visited);
private:
  std::queue<const table * > m_queue;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // TRAVERSE_TABLES_H

#ifndef CAPEMISA_SQL_BUSINESS_INSERT_GENERATION_ORDER_H
#define CAPEMISA_SQL_BUSINESS_INSERT_GENERATION_ORDER_H

#include <list>

#include <sql/generic/name.h>
#include <sql/generic/ptr.h>
#include <sql/entities/database.h>
#include <sql/business/traverse_tables.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;
using namespace generic;

struct insert_generation_order {
  std::list<name> operator()(ptr<database> p_db) const;
};

} // namespace business
} // namespace sql
} // namespace capemisa


#endif // INSERT_GENERATION_ORDER_H

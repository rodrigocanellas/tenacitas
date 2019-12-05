#ifndef CAPEMISA_SQL_BUSINESS_SQL_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_SQL_GENERATOR_H

#include <sstream>
#include <string>

#include <sql/entities/table_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

struct sql_generator
{

  std::string operator()(generic::ptr<const entities::table_values> p_pks,
                         generic::ptr<const entities::table_values> p_attrs,
                         generic::ptr<const entities::table_values> p_fks);
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // SQL_GENERATOR_H

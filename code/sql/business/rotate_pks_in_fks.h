#ifndef CAPEMISA_SQL_BUSINESS_ROTATE_PKS_IN_FKS_H
#define CAPEMISA_SQL_BUSINESS_ROTATE_PKS_IN_FKS_H

#include <cstdint>

#include <sql/business/column_generator.h>
#include <sql/business/foreign_key_generator.h>
#include <sql/entities/column.h>
#include <sql/entities/foreign_key_column.h>
#include <sql/entities/table.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

struct rotate_pks_in_fks : public foreign_key_generator
{
  static const name id;

  void operator()(ptr<const tables_values> p_all_pks,
                  ptr<table_values> p_fks_columns_values,
                  ptr<foreign_key> p_fk,
                  uint16_t p_num_lines);
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // ROTATE_PKS_IN_FKS_H

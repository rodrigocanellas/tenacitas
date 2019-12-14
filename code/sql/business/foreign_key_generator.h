#ifndef CAPEMISA_SQL_BUSINESS_FOREIGN_KEY_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_FOREIGN_KEY_GENERATOR_H

#include <cstdint>

#include <sql/business/column_generator.h>
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

struct foreign_key_generator
{
  ///
  /// \brief foreign_key_generator
  ///
  /// \param p_pks are the values of the all the PK columns of all tables
  /// \param [out] p_fks_columns_values is a table with the values of the FK
  /// columns of \p p_fk \param p_fk FK to which colums values will be
  //  generated
  /// \param uint16_t is the amount of lines
  virtual void operator()(ptr<const tables_values> p_all_pks,
                          ptr<table_values> p_fks_columns_values,
                          ptr<foreign_key> p_fk,
                          uint16_t p_num_lines) = 0;
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEY_GENERATOR_H

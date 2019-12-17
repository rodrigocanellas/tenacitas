#ifndef CAPEMISA_SQL_BUSINESS_ONE_PK_ALL_FKS_H
#define CAPEMISA_SQL_BUSINESS_ONE_PK_ALL_FKS_H

#include <cstdint>

#include <sql/business/foreign_key_generator.h>
#include <sql/entities/primary_key.h>
#include <sql/entities/primary_key_column.h>
#include <sql/entities/table.h>
#include <sql/entities/table_values.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace capemisa::sql::entities;
using namespace capemisa::sql::generic;

struct one_pk_all_fks : public foreign_key_generator
{

  typedef ptr<table_values> table_values_ptr;
  typedef ptr<const tables_values> tables_values_const_ptr;
  typedef ptr<foreign_key> foreign_key_ptr;

  static const name id;

  ///
  /// \brief foreign_key_generator
  ///
  /// \param p_pks are the values of the all the PK columns of all tables
  /// \param foreign_key_column_ptr FK to which colums values will be
  ///  generated
  /// \param uint16_t is the amount of lines
  ///
  void operator()(ptr<const tables_values> p_all_pks,
                  ptr<table_values> p_fks_columns_values,
                  ptr<foreign_key> p_fk,
                  uint16_t p_num_lines);
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // ONE_PK_ALL_FKS_H

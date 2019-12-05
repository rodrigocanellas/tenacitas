#ifndef CAPEMISA_SQL_BUSINESS_ONE_PK_ALL_FKS_H
#define CAPEMISA_SQL_BUSINESS_ONE_PK_ALL_FKS_H

#include <cstdint>

#include <sql/entities/table.h>
#include <sql/entities/table_values.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

struct one_pk_all_fks
{

  typedef generic::ptr<entities::table> table_ptr;
  typedef generic::ptr<entities::table_values> table_values;
  typedef generic::ptr<entities::table_values> table_values_ptr;
  typedef generic::ptr<const entities::tables_values> tables_values_const_ptr;
  typedef generic::ptr<entities::tables_values> tables_values_ptr;
  typedef generic::ptr<entities::foreign_key> foreign_key_ptr;

  ///
  /// \brief foreign_key_generator
  ///
  /// \param const table_values& are the values of the PK columns
  /// \param foreign_key_column_ptr FK to which colums values will be generated
  /// \param uint16_t is the amount of lines

  table_values_ptr operator()(tables_values_const_ptr p_pks,
                              table_ptr p_table,
                              foreign_key_ptr p_kf,
                              uint16_t p_num_lines)
  {
    table_values_ptr _table_values(generic::make_ptr<table_values>(p_table));

    return _table_values;
  }
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // ONE_PK_ALL_FKS_H

#ifndef CAPEMISA_SQL_BUSINESS_FOREIGN_KEY_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_FOREIGN_KEY_GENERATOR_H

#include <cstdint>
#include <functional>
#include <list>

#include <sql/entities/foreign_key_column.h>
#include <sql/entities/table.h>
#include <sql/entities/tables_values.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

typedef generic::ptr<entities::table_values> table_values_ptr;
typedef generic::ptr<const entities::tables_values> tables_values_const_ptr;
typedef generic::ptr<entities::foreign_key> foreign_key_ptr;

///
/// \brief foreign_key_generator
///
/// \param p_pks are the values of the all the PK columns of all tables
/// \param foreign_key_column_ptr FK to which colums values will be generated
/// \param uint16_t is the amount of lines
typedef std::function<
  void(tables_values_const_ptr, table_values_ptr, foreign_key_ptr, uint16_t)>
  foreign_key_generator;

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEY_GENERATOR_H

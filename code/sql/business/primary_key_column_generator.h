#ifndef CAPEMISA_SQL_BUSINESS_PRIMARY_KEY_COLUMN_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_PRIMARY_KEY_COLUMN_GENERATOR_H

#include <cstdint>
#include <functional>
#include <list>

#include <sql/entities/column_values.h>
#include <sql/entities/primary_key_column.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

typedef generic::ptr<entities::column_values> column_values_ptr;
typedef generic::ptr<entities::primary_key_column> primary_key_column_ptr;

///
/// \brief primary_key_generator
///
/// \param uint16_t is the amount of lines
typedef std::function<column_values_ptr(primary_key_column_ptr, uint16_t)>
  primary_key_column_generator;

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // PRIMARY_KEY_GENERATOR_H

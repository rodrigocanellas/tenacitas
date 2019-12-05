#ifndef CAPEMISA_SQL_BUSINESS_ATTRIBUTE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_ATTRIBUTE_GENERATOR_H

#include <cstdint>
#include <functional>

#include <sql/entities/attribute_column.h>
#include <sql/entities/column_values.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

typedef generic::ptr<entities::column_values> column_values_ptr;
typedef generic::ptr<entities::attribute_column> attribute_column_ptr;

///
/// \brief attribute_generator
///
/// \param uint16_t is the amount of lines
typedef std::function<column_values_ptr(attribute_column_ptr, uint16_t)>
  attribute_column_generator;

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // ATTRIBUTE_GENERATOR_H

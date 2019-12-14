#ifndef CAPEMISA_SQL_BUSINESS_PRIMARY_KEY_COLUMN_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_PRIMARY_KEY_COLUMN_GENERATOR_H

#include <cstdint>
#include <functional>
#include <list>

#include <sql/business/column_generator.h>
#include <sql/entities/column_values.h>
#include <sql/entities/primary_key_column.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

// using namespace capemisa::sql::entities;
// using namespace capemisa::sql::generic;

// struct primary_key_column_generator : public column_generator
//{

//  virtual ~primary_key_column_generator() override = default;

//  ptr<column_values> operator()(ptr<primary_key_column> p_pk_column,
//                                uint16_t p_num_lines)
//  {
//    check_type_compatibility(p_pk_column);
//    return generate(p_pk_column, p_num_lines);
//  }

// protected:
//  primary_key_column_generator(column_type p_types)
//    : column_generator(p_types)

//  {}

//  virtual ptr<column_values> generate(ptr<primary_key_column> p_pk_col,
//                                      uint16_t p_num_lines) = 0;
//};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // PRIMARY_KEY_GENERATOR_H

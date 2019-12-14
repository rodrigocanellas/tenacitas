#ifndef CAPEMISA_SQL_BUSINESS_ATTRIBUTE_GENERATOR_H
#define CAPEMISA_SQL_BUSINESS_ATTRIBUTE_GENERATOR_H

#include <cstdint>
#include <functional>

#include <sql/business/column_generator.h>
#include <sql/entities/attribute_column.h>
#include <sql/entities/column.h>
#include <sql/entities/column_type.h>
#include <sql/entities/column_values.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

// using namespace capemisa::sql::entities;
// using namespace capemisa::sql::generic;

// struct attribute_column_generator : public column_generator
//{

//  virtual ~attribute_column_generator() override = default;

//  ptr<column_values> operator()(ptr<attribute_column> p_attr_column,
//                                uint16_t p_num_lines)
//  {
//    check_type_compatibility(p_attr_column);
//    return generate(p_attr_column, p_num_lines);
//  }

// protected:
//  attribute_column_generator(column_type p_types, const name& p_name)
//    : column_generator(p_types, p_name)
//  {}

//  virtual ptr<column_values> generate(ptr<attribute_column> p_attr_column,
//                                      uint16_t p_num_lines) = 0;
//};

//(column_type::int_1 | column_type::int_2 | column_type::int_4,
// column_type::int_8 | column_type::date | column_type::date_time |
//   column_type::small_real | column_type::long_real),

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // ATTRIBUTE_GENERATOR_H

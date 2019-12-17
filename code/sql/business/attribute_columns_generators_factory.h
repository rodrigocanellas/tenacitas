#ifndef CAPEMISA_SQL_BUSINESS_ATTRIBUTE_COLUMNS_GENERATORS_FACTORY_H
#define CAPEMISA_SQL_BUSINESS_ATTRIBUTE_COLUMNS_GENERATORS_FACTORY_H

#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <sql/business/column_generator.h>
#include <sql/business/columns_generators_factory.h>
#include <sql/business/fixed_value_generator.h>
#include <sql/business/number_value_generator.h>
#include <sql/business/text_value_generator.h>
#include <sql/entities/attribute_column.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>
#include <sql/generic/string_split.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;
using namespace generic;

struct attribute_columns_generators_factory
{
  typedef ptr<column_generator<attribute_column>> generator_ptr;

  static generator_ptr create(ptr<attribute_column> p_col,
                              const name& p_generator_name,
                              const std::string& p_params)
  {

    if (p_generator_name == fixed_value_generator::id) {
      return make_ptr<fixed_value_generator>(p_params);
    }

    generator_ptr _ret = columns_generators_factory_t<attribute_column>::create(
      p_col, p_generator_name, p_params);
    if (_ret) {
      return _ret;
    }

    return generator_ptr();
  }

  static std::set<name> list(column_type p_type)
  {
    std::set<name> _set(
      columns_generators_factory_t<attribute_column>::list(p_type));

    if (fixed_value_generator::compatibles() & p_type) {
      _set.insert(fixed_value_generator::id);
    }

    return _set;
  }
};
} // namespace business
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_GENERATORS_H

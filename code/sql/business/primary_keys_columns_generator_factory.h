#ifndef CAPEMISA_SQL_BUSINESS_PRIMARY_KEYS_COLUMNS_GENERATOR_FACTORY_H
#define CAPEMISA_SQL_BUSINESS_PRIMARY_KEYS_COLUMNS_GENERATOR_FACTORY_H

#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <sql/business/column_generator.h>
#include <sql/business/columns_generators_factory.h>
#include <sql/business/number_value_generator.h>
#include <sql/business/text_value_generator.h>
#include <sql/entities/primary_key_column.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;
using namespace generic;

struct primary_keys_columns_generators_factory
{
  typedef ptr<column_generator<primary_key_column>> generator_ptr;
  static generator_ptr create(ptr<primary_key_column> p_col,
                              const name& p_generator_name,
                              const std::string& p_params)
  {

    generator_ptr _ret =
      columns_generators_factory_t<primary_key_column>::create(
        p_col, p_generator_name, p_params);
    if (_ret) {
      return _ret;
    }

    return generator_ptr();
  }

  static std::set<name> list(column_type p_type)
  {
    std::set<name> _set(
      columns_generators_factory_t<primary_key_column>::list(p_type));
    return _set;
  }
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // PRIMARY_KEYS_COLUMNS_GENERATOR_FACTORY_H

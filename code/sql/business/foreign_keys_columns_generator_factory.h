#ifndef CAPEMISA_SQL_BUSINESS_FOREIGN_KEYS_COLUMNS_GENERATOR_FACTORY_H
#define CAPEMISA_SQL_BUSINESS_FOREIGN_KEYS_COLUMNS_GENERATOR_FACTORY_H

#include <set>
#include <string>

#include <sql/business/foreign_key_generator.h>
#include <sql/business/one_pk_all_fks.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>
#include <sql/generic/string_split.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;
using namespace generic;

struct foreign_keys_columns_generator_factory
{
  typedef ptr<foreign_key_generator> generator_ptr;

  static generator_ptr create(const name& p_generator_name,
                              const std::string& /*p_params*/)
  {
    if (p_generator_name == one_pk_all_fks::id) {
      return make_ptr<one_pk_all_fks>();
    }

    return generator_ptr();
  }

  static std::set<name> list()
  {
    std::set<name> _set;
    _set.insert(one_pk_all_fks::id);
    return _set;
  }
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // FOREIGN_KEYS_COLUMNS_GENERATOR_FACTORY_H

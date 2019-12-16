#ifndef CAPEMISA_SQL_BUSINESS_COLUMNS_GENERATORS_FACTORY_H
#define CAPEMISA_SQL_BUSINESS_COLUMNS_GENERATORS_FACTORY_H

#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <sql/business/column_generator.h>
#include <sql/business/number_value_generator.h>
#include <sql/business/text_value_generator.h>
#include <sql/entities/column.h>
#include <sql/generic/collection.h>
#include <sql/generic/name.h>
#include <sql/generic/ptr.h>
#include <sql/generic/string_split.h>

namespace capemisa {
namespace sql {
namespace business {

using namespace entities;
using namespace generic;

template<typename t_column>
struct columns_generators_factory_t
{
  typedef ptr<column_generator<t_column>> generator_ptr;

  static generator_ptr create(ptr<t_column> p_col,
                              const name& p_generator_name,
                              const std::string& p_params)
  {
    if (p_generator_name == number_value_generator<int8_t, t_column>::id) {
      if (number_value_generator<int8_t, t_column>::compatibles() &
          p_col->get_type()) {

        std::vector<int32_t> _params;
        string_split _string_split;
        _string_split(p_params, [&_params](std::string&& p_split) -> void {
          if (p_split.empty()) {
            _params.push_back(0);
          } else {
            _params.push_back(std::stol(p_split));
          }
        });

        return make_ptr<number_value_generator<int32_t, t_column>>(
          _params[0], _params[1], _params[2]);
      }
    } else if (p_generator_name == text_value_generator<t_column>::id) {
      if (text_value_generator<t_column>::compatibles() & p_col->get_type()) {

        std::vector<std::string> _params;

        string_split _string_split;
        _string_split(p_params, [&_params](std::string&& p_split) -> void {
          _params.push_back(p_split);
        });

        return make_ptr<text_value_generator<t_column>>(
          _params[0],
          p_col->get_size(),
          (_params[1].empty() ? 1 : std::stoi(_params[1])));
      }
    }
    return generator_ptr();
  }

  static std::set<name> list(column_type p_type)
  {
    std::set<name> _set;
    if (number_value_generator<int8_t, t_column>::compatibles() & p_type) {
      _set.insert(number_value_generator<int8_t, t_column>::id);
    }
    if (text_value_generator<t_column>::compatibles() & p_type) {
      _set.insert(text_value_generator<t_column>::id);
    }
    return _set;
  }
};
} // namespace business
} // namespace sql
} // namespace capemisa

#endif // COLUMNS_GENERATORS_FACTORY_H

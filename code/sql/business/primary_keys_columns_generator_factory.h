#ifndef PRIMARY_KEYS_COLUMNS_GENERATOR_FACTORY_H
#define PRIMARY_KEYS_COLUMNS_GENERATOR_FACTORY_H

#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <sql/business/column_generator.h>
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
    if (p_generator_name ==
        number_value_generator<int8_t, primary_key_column>::id) {
      if (number_value_generator<int8_t, primary_key_column>::compatibles() &
          p_col->get_type()) {

        std::vector<int32_t> _params(3);
        std::vector<int32_t>::size_type _counter = 0;
        std::string::size_type last = 0;
        std::string::size_type next = 0;

        while ((next = p_params.find(';', last)) != std::string::npos) {
          _params[_counter++] = std::stol(p_params.substr(last, next - last));
          last = next + 1;
        }
        _params[_counter] = std::stol(p_params.substr(last));
        return make_ptr<number_value_generator<int32_t, primary_key_column>>(
          _params[0], _params[1], _params[2]);
      }
    } else if (p_generator_name ==
               text_value_generator<primary_key_column>::id) {
      if (text_value_generator<primary_key_column>::compatibles() &
          p_col->get_type()) {

        std::vector<std::string> _params(3);
        std::vector<std::string>::size_type _counter = 0;
        std::string::size_type last = 0;
        std::string::size_type next = 0;

        while ((next = p_params.find(';', last)) != std::string::npos) {
          _params[_counter++] = p_params.substr(last, next - last);
          last = next + 1;
        }
        _params[_counter] = p_params.substr(last);

        return make_ptr<text_value_generator<primary_key_column>>(
          _params[0], std::stoi(_params[1]), std::stoi(_params[2]));
      }
    }
    return generator_ptr();
  }

  static std::set<name> list(column_type p_type)
  {
    std::set<name> _set;
    if (number_value_generator<int8_t, primary_key_column>::compatibles() &
        p_type) {
      _set.insert(number_value_generator<int8_t, primary_key_column>::id);
    }
    if (text_value_generator<primary_key_column>::compatibles() & p_type) {
      _set.insert(text_value_generator<primary_key_column>::id);
    }
    return _set;
  }
};

} // namespace business
} // namespace sql
} // namespace capemisa

#endif // PRIMARY_KEYS_COLUMNS_GENERATOR_FACTORY_H

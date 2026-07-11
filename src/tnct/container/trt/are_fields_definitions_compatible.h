/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_ARE_FIELDS_DEFINITION_COMPATIBLE_H
#define TNCT_CONTAINER_TRT_ARE_FIELDS_DEFINITION_COMPATIBLE_H

#include <tuple>

#include "tnct/container/cpt/field_definition.h"
#include "tnct/tuple/bus/traverse.h"
#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::container::trt {

template <cpt::field_definition... t_fields_definitions>
static constexpr bool field_compatibility_helper() {
  bool _are_compatible{true};
  using fields_definitions = std::tuple<t_fields_definitions...>;

  using first_type =
      typename std::tuple_element_t<0, fields_definitions>::object_type;

  auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
    using current_type =
        typename std::tuple_element_t<t_pos, t_tuple>::object_type;
    if constexpr (!std::is_same_v<first_type, current_type>) {
      _are_compatible = false;
      return false;
    }

    return true;
  }};

  tuple::bus::traverse<fields_definitions, decltype(_visit)>(_visit);

  return _are_compatible;
}

template <cpt::field_definition... t_fields_definitions>
struct are_fields_definition_compatible {
  static constexpr bool value =
      (sizeof...(t_fields_definitions) == 1) ||
      ((sizeof...(t_fields_definitions) > 0) &&
       field_compatibility_helper<t_fields_definitions...>());
};

template <cpt::field_definition... t_fields_definitions>
static constexpr bool are_fields_definition_compatible_v =
    are_fields_definition_compatible<t_fields_definitions...>::value;

} // namespace tnct::container::trt

#endif

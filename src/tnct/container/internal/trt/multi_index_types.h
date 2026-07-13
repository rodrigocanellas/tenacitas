/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_INTERNAL_TRT_MULTI_INDEX_TYPES_H
#define TNCT_CONTAINER_INTERNAL_TRT_MULTI_INDEX_TYPES_H

#include <tuple>

#include "tnct/container/cpt/field_definition.h"
#include "tnct/container/trt/index_traits.h"

namespace tnct::container::internal::trt {

template <typename t_record, cpt::field_definition... t_fields_definitions>
struct multi_index_types {
  using record = t_record;

  using record_ref = std::reference_wrapper<record>;

  using indexes = std::tuple<typename container::trt::index_traits<
      typename t_fields_definitions::index_traits_id,
      typename t_fields_definitions::field_type, record_ref>::index...>;

  template <std::size_t t_field_pos>
  using index_t = std::tuple_element_t<t_field_pos, indexes>;

  using index_iterators = std::tuple<typename container::trt::index_traits<
      typename t_fields_definitions::index_traits_id,
      typename t_fields_definitions::field_type, record_ref>::iterator...>;

  template <std::size_t t_field_pos>
  using index_iterator_t = std::tuple_element_t<t_field_pos, index_iterators>;
};

} // namespace tnct::container::internal::trt

#endif

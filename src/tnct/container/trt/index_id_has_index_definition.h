/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_INDEX_ID_HAS_INDEX_DEFINITION_H
#define TNCT_CONTAINER_TRT_INDEX_ID_HAS_INDEX_DEFINITION_H

#include "tnct/container/cpt/field_definition.h"
#include "tnct/container/cpt/index_definition.h"
#include "tnct/container/trt/index_definition.h"

namespace tnct::container::trt {

template <typename t_rec_opt_ref,
          container::cpt::field_definition... t_fields_definitions>
struct index_id_has_index_definition {

public:
  static constexpr bool value =
      ((container::cpt::index_definition<

            container::trt::index_definition<
                typename t_fields_definitions::index_id,
                typename t_fields_definitions::field_type, t_rec_opt_ref>,

            typename t_fields_definitions::index_id,
            typename t_fields_definitions::field_type, t_rec_opt_ref>,
        ...));
};

template <typename t_rec_opt_ref,
          container::cpt::field_definition... t_fields_definitions>
static constexpr bool index_id_has_index_definition_v =
    index_id_has_index_definition<t_rec_opt_ref,
                                  t_fields_definitions...>::value;

} // namespace tnct::container::trt

#endif

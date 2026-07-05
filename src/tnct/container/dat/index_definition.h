/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_INDEX_DEFINITION_H
#define TNCT_CONTAINER_DAT_INDEX_DEFINITION_H

#include "tnct/container/cpt/index.h"
#include "tnct/container/trt/index_type.h"
#include "tnct/container/trt/object_indexed.h"

namespace tnct::container::dat {

template <typename t_index_id, typename t_object_type, typename t_key_type,
          typename t_key_getter>
  requires(
      cpt::index<
          trt::index_type_t<t_index_id, t_key_type,
                            typename trt::object_indexed<t_object_type>::ref>,
          t_key_type, typename trt::object_indexed<t_object_type>::ref> &&
      std::is_invocable_r_v<t_key_type, t_key_getter, const t_object_type &>)
struct index_definition {

  using index_id = t_index_id;
  using key_type = t_key_type;
  using object_type = t_object_type;
  using key_getter = t_key_getter;
};

} // namespace tnct::container::dat

#endif

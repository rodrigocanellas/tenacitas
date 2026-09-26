/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_DEFINITION_H
#define TNCT_CONTAINER_CPT_INDEX_DEFINITION_H

#include "tnct/container/cpt/index.h"

namespace tnct::container::cpt {

template <typename t, typename t_index_id, typename t_key, typename t_value>
concept index_definition = requires {
  typename t::index_type;

  requires container::cpt::index<typename t::index_type>;

  requires std::same_as<std::remove_cv_t<decltype(t::is_calculated)>, bool>;
};

} // namespace tnct::container::cpt

#endif

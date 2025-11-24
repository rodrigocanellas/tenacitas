/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_GENERIC_CPT_CREATOR_H
#define TNCT_GENERIC_CPT_CREATOR_H

#include <functional>
#include <optional>

namespace tnct::generic::cpt
{

template <typename t, typename t_obj>
concept creator = requires(t &&p_t) {
  std::invoke_r<std::optional<t_obj>>(std::forward<t>(p_t));
};

} // namespace tnct::generic::cpt

#endif

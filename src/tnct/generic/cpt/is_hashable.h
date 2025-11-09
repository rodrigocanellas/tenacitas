/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_GENERIC_CPT_IS_HASHABLE_H
#define TNCT_GENERIC_CPT_IS_HASHABLE_H

#include <functional>
#include <type_traits>

namespace tnct::generic::cpt
{

template <typename t>
concept is_hashable =
    std::is_invocable_r_v<std::size_t, std::hash<std::remove_cvref_t<t>>,
                          std::remove_cvref_t<t>>
    ||

    requires(t p_t) {
      {
        p_t()
      } -> std::same_as<std::size_t>;
    };

} // namespace tnct::generic::cpt

#endif

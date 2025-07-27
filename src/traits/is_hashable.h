/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_IS_HASHABLE_H
#define TENACITAS_SRC_TRAITS_IS_HASHABLE_H

#include <functional>
#include <type_traits>

namespace tenacitas::src::traits
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

} // namespace tenacitas::src::traits

#endif

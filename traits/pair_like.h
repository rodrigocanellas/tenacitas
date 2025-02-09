/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_PAIR_LIKE_H
#define TENACITAS_LIB_TRAITS_PAIR_LIKE_H

#include <utility>

namespace tenacitas::lib::traits {

template <typename T>
concept pair_like = requires {
  typename T::first_type;
  typename T::second_type;
}
&&std::is_same_v<T, std::pair<typename T::first_type, typename T::second_type>>;

} // namespace tenacitas::lib::traits
#endif

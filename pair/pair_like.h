/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_PAIR_PAIR_LIKE_H
#define TENACITAS_LIB_PAIR_PAIR_LIKE_H

#include <utility>

namespace tenacitas::lib::pair {

template <typename T>
concept pair_like = requires {
  typename T::first_type;
  typename T::second_type;
}
&&std::is_same_v<T, std::pair<typename T::first_type, typename T::second_type>>;

} // namespace tenacitas::lib::pair
#endif

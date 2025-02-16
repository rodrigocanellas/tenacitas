/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_PAIR_LIKE_H
#define TNCT_TRAITS_PAIR_LIKE_H

#include <concepts>

namespace tnct::traits {

// template <typename T>
// concept pair_like = requires {
//   typename T::first_type;
//   typename T::second_type;
// }
// &&std::is_same_v<T, std::pair<typename T::first_type, typename
// T::second_type>>;

template <typename T>
concept pair_like = requires(T t) {
  { t.first } -> std::convertible_to<typename T::first_type>;
  { t.second } -> std::convertible_to<typename T::second_type>;
};

} // namespace tnct::traits
#endif

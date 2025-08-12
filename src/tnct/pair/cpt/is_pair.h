/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_PAIR_CPT_IS_PAIR_H
#define TNCT_PAIR_CPT_IS_PAIR_H

#include <concepts>

namespace tnct::pair::cpt
{

template <typename T>
concept is_pair = requires(T t) {
  requires std::same_as<std::remove_cvref_t<decltype(t.first)>,
                        typename T::first_type>

               &&

               std::same_as<std::remove_cvref_t<decltype(t.second)>,
                            typename T::second_type>;
};

} // namespace tnct::pair::cpt
#endif

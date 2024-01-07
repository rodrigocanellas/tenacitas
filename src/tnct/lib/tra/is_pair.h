/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_TRA_IS_PAIR_H
#define TNCT_LIB_TRA_IS_PAIR_H

#include <type_traits>
#include <utility>

namespace tnct::lib::tra {

/// \brief Type trait that identifies if a type is not a \p pair
template <typename> struct is_pair : std::false_type {};

/// \brief Type trait that identifies if a type is a tuple
template <typename... t> struct is_pair<std::pair<t...>> : std::true_type {};

} // namespace tnct::lib::tra

namespace tlt = tnct::lib::tra;

#endif

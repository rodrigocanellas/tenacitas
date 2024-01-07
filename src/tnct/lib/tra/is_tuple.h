/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_TRA_IS_TUPLE_H
#define TNCT_LIB_TRA_IS_TUPLE_H

#include <tuple>

namespace tnct::lib::tra {

/// \brief Type trait that identifies if an object is not a tuple
template <typename> struct is_tuple : std::false_type {};

/// \brief Type trait that identifies if an object is a tuple
template <typename... t> struct is_tuple<std::tuple<t...>> : std::true_type {};

} // namespace tnct::lib::tra

namespace tlt = tnct::lib::tra;

#endif

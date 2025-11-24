/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_GENERIC_BUS_CREATOR_H
#define TNCT_GENERIC_BUS_CREATOR_H

#include <functional>
#include <optional>

namespace tnct::generic::bus
{

template <typename t_obj>
using creator = std::function<std::optional<t_obj>()>;

} // namespace tnct::generic::bus

#endif

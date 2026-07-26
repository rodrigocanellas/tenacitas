/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_SUPPLIER_CPT_SUPPLIER_H
#define TNCT_SUPPLIER_CPT_SUPPLIER_H

#include <concepts>

namespace tnct::supplier::cpt::internal {

template <typename t_supplier>
concept supplier = requires {
  requires(!std::copyable<std::remove_cvref_t<t_supplier>>);
  requires(!std::movable<std::remove_cvref_t<t_supplier>>);
};
} // namespace tnct::supplier::cpt::internal
#endif

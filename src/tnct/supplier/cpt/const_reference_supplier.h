/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_SUPPLIER_CPT_CONST_REFERENCE_SUPPLIER_H
#define TNCT_SUPPLIER_CPT_CONST_REFERENCE_SUPPLIER_H

#include <concepts>
#include <optional>

#include "tnct/generic/cpt/meta_class.h"
#include "tnct/supplier/cpt/internal/supplier.h"

namespace tnct::supplier::cpt {

template <typename t_supplier, typename t_object, typename... t_parameters>
concept const_reference_supplier = requires {
  requires(tnct::supplier::cpt::internal::supplier<
              std::remove_cvref_t<t_supplier>>)

              &&

              requires(const std::remove_cvref_t<t_supplier> &p_supplier) {
                {
                  p_supplier.get(generic::cpt::meta_class<t_object>{},
                                 std::declval<t_parameters>()...)
                } -> std::same_as<
                      std::optional<std::reference_wrapper<const t_object>>>;
              };
};
} // namespace tnct::supplier::cpt
#endif

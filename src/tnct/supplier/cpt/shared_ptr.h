/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_SUPPLIER_CPT_SHARED_PTR_SUPPLIER_H
#define TNCT_SUPPLIER_CPT_SHARED_PTR_SUPPLIER_H

#include <concepts>
#include <memory>

#include "tnct/supplier/cpt/internal/supplier.h"
#include "tnct/supplier/cpt/meta_class.h"

namespace tnct::supplier::cpt {

template <typename t_supplier, typename t_object, typename... t_parameters>
concept shared_ptr_supplier = requires {
  requires(tnct::supplier::cpt::internal::supplier<
              std::remove_cvref_t<t_supplier>>)

              &&

              requires(std::remove_cvref_t<t_supplier> &p_supplier) {
                {
                  p_supplier.get(meta_class<t_object>{},
                                 std::declval<t_parameters>()...)
                } -> std::same_as<std::shared_ptr<t_object>>;
              };
};
} // namespace tnct::supplier::cpt
#endif

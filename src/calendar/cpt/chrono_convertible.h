#ifndef TENACITAS_LIB_CONCEPTS_CPT_CHRONO_CONVERTIBLE_H
#define TENACITAS_LIB_CONCEPTS_CPT_CHRONO_CONVERTIBLE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas.lib.async directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <concepts>
#include <iostream>

namespace tenacitas::lib::src::calendar::cpt {

template <typename t_to, typename t_from>
concept chrono_convertible = requires(t_from p_time) {
  std::chrono::duration_cast<t_to>(p_time);
};

template <typename t_from>
concept convertible_to_ms =
    chrono_convertible<std::chrono::milliseconds, t_from>;

template <typename t_from>
concept convertible_to_ns =
    chrono_convertible<std::chrono::nanoseconds, t_from>;

} // namespace tenacitas::lib::src::calendar::cpt

#endif

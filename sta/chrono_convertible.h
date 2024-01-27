/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_STA_CHRONO_CONVERTIBLE_H
#define TENACITAS_LIB_STA_CHRONO_CONVERTIBLE_H

#include <chrono>

namespace tenacitas::lib::sta {

template <typename t_to, typename t_from>
concept chrono_convertible = requires(t_from p_time) {
  std::chrono::duration_cast<t_to>(p_time);
};

template <typename t_from>
concept convertible_to_milli =
    chrono_convertible<std::chrono::milliseconds, t_from>;

template <typename t_from>
concept convertible_to_sec = chrono_convertible<std::chrono::seconds, t_from>;

template <typename t_from>
concept convertible_to_nano =
    chrono_convertible<std::chrono::nanoseconds, t_from>;

} // namespace tenacitas::lib::sta

#endif

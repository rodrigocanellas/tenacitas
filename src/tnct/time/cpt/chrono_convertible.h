/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TIME_CPT_CHRONO_CONVERTIBLE_H
#define TNCT_TIME_CPT_CHRONO_CONVERTIBLE_H

#include <chrono>
#include <type_traits>

namespace tnct::time::cpt
{

// the code below is an improved version by ChatGPT from a previous version I
// wrote

template <typename t_from>
concept is_chrono_duration = requires {
  typename t_from::rep;
  typename t_from::period;
  requires std::is_same_v<
      t_from,
      std::chrono::duration<typename t_from::rep, typename t_from::period>>;
};

template <typename t_to, typename t_from>
concept chrono_convertible =
    is_chrono_duration<t_from> && requires(t_from p_time) {
      {
        std::chrono::duration_cast<t_to>(p_time)
      } -> std::same_as<t_to>;
    };

template <typename t_from>
concept convertible_to_milli =
    chrono_convertible<std::chrono::milliseconds, t_from>;

template <typename t_from>
concept convertible_to_sec = chrono_convertible<std::chrono::seconds, t_from>;

template <typename t_from>
concept convertible_to_nano =
    chrono_convertible<std::chrono::nanoseconds, t_from>;

} // namespace tnct::time::cpt

#endif

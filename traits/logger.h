/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_LOGGER_H
#define TENACITAS_LIB_TRAITS_LOGGER_H

#include <source_location>
#include <string_view>

#include <tenacitas.lib/traits/new_operator.h>

namespace tenacitas::lib::traits {

template <typename t>
concept logger = requires(t p_t) {
  !std::copy_constructible<t>;

  !std::move_constructible<t>;

  no_new_operator<t>;

  {
    p_t.tra(std::declval<std::string_view>(),
            std::declval<std::source_location>() =
                std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.deb(std::declval<std::string_view>(),
            std::declval<std::source_location>() =
                std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.inf(std::declval<std::string_view>(),
            std::declval<std::source_location>() =
                std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.war(std::declval<std::string_view>(),
            std::declval<std::source_location>() =
                std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.err(std::declval<std::string_view>(),
            std::declval<std::source_location>() =
                std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.fat(std::declval<std::string_view>(),
            std::declval<std::source_location>() =
                std::source_location::current())
    } -> std::same_as<void>;

  { p_t.set_tra() } -> std::same_as<void>;

  { p_t.set_deb() } -> std::same_as<void>;

  { p_t.set_inf() } -> std::same_as<void>;

  { p_t.set_war() } -> std::same_as<void>;

  { p_t.save_level() } -> std::same_as<void>;

  { p_t.restore_level() } -> std::same_as<void>;

  { p_t.can_tra() } -> std::same_as<bool>;

  { p_t.can_deb() } -> std::same_as<bool>;

  { p_t.can_inf() } -> std::same_as<bool>;

  { p_t.can_war() } -> std::same_as<bool>;
};

} // namespace tenacitas::lib::traits

#endif

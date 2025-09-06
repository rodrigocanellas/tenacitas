/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_CPT_LOGGER_H
#define TNCT_LOG_CPT_LOGGER_H

#include <source_location>
#include <string_view>

#include "tnct/memory/cpt/has_new_operator.h"

namespace tnct::log::cpt
{

template <typename t>
concept logger = !std::copy_constructible<t> &&

                 !std::move_constructible<t> &&

                 !std::is_copy_assignable_v<t> &&

                 !std::is_move_assignable_v<t> &&

                 !memory::cpt::has_new_operator_v<t> &&

                 requires(t p_t) {
                   {
                     p_t.tra(std::declval<std::string_view>(),
                             std::source_location::current())
                   } -> std::same_as<void>;

                   {
                     p_t.deb(std::declval<std::string_view>(),
                             std::source_location::current())
                   } -> std::same_as<void>;

                   {
                     p_t.inf(std::declval<std::string_view>(),
                             std::source_location::current())
                   } -> std::same_as<void>;

                   {
                     p_t.war(std::declval<std::string_view>(),
                             std::source_location::current())
                   } -> std::same_as<void>;

                   {
                     p_t.err(std::declval<std::string_view>(),
                             std::source_location::current())
                   } -> std::same_as<void>;

                   {
                     p_t.fat(std::declval<std::string_view>(),
                             std::source_location::current())
                   } -> std::same_as<void>;

                   {
                     p_t.set_tra()
                   } -> std::same_as<void>;

                   {
                     p_t.set_deb()
                   } -> std::same_as<void>;

                   {
                     p_t.set_inf()
                   } -> std::same_as<void>;

                   {
                     p_t.set_war()
                   } -> std::same_as<void>;

                   {
                     p_t.save_level()
                   } -> std::same_as<void>;

                   {
                     p_t.restore_level()
                   } -> std::same_as<void>;

                   {
                     p_t.can_tra()
                   } -> std::same_as<bool>;

                   {
                     p_t.can_deb()
                   } -> std::same_as<bool>;

                   {
                     p_t.can_inf()
                   } -> std::same_as<bool>;

                   {
                     p_t.can_war()
                   } -> std::same_as<bool>;
                 };

} // namespace tnct::log::cpt

#endif

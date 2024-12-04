/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_LOGGER_H
#define TENACITAS_LIB_TRAITS_LOGGER_H

#include <source_location>
#include <string_view>

#include "tenacitas.lib/traits/has_new_operator.h"

#define TNCT_LOG_TRA(logger, msg)                                              \
  do {                                                                         \
    if (logger.can_tra()) {                                                    \
      logger.tra(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_DEB(logger, msg)                                              \
  do {                                                                         \
    if (logger.can_deb()) {                                                    \
      logger.deb(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_INF(logger, msg)                                              \
  do {                                                                         \
    if (logger.can_inf()) {                                                    \
      logger.inf(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_WAR(logger, msg)                                              \
  do {                                                                         \
    if (logger.can_war()) {                                                    \
      logger.war(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_ERR(logger, msg) logger.err(msg);

#define TNCT_LOG_FAT(logger, msg) logger.err(msg);

#define TNCT_LOG_TST(logger, msg) logger.tst(msg);

namespace tenacitas::lib::traits {

template <typename t>
concept logger = !std::copy_constructible<t> &&

                 !std::move_constructible<t> &&

                 !std::is_copy_assignable_v<t> &&

                 !std::is_move_assignable_v<t> &&

                 !has_new_operator_v<t> &&

                 requires(t p_t) {

  {
    p_t.tra(std::declval<std::string_view>(), std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.deb(std::declval<std::string_view>(), std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.inf(std::declval<std::string_view>(), std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.war(std::declval<std::string_view>(), std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.err(std::declval<std::string_view>(), std::source_location::current())
    } -> std::same_as<void>;

  {
    p_t.fat(std::declval<std::string_view>(), std::source_location::current())
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

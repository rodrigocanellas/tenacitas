/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_LOG_LOGGER_H
#define TENACITAS_LIB_LOG_LOGGER_H

#include <tenacitas.lib/generic/fmt.h>
#include <tenacitas.lib/traits/logger.h>

#define TNCT_LOG_TRA(logger, p_params...)                                      \
  if (logger.can_tra()) {                                                      \
    logger.tra(tenacitas::lib::generic::fmt(p_params));                        \
  }

#define TNCT_LOG_DEB(logger, p_params...)                                      \
  if (logger.can_deb()) {                                                      \
    logger.deb(tenacitas::lib::generic::fmt(p_params));                        \
  }

#define TNCT_LOG_INF(logger, p_params...)                                      \
  if (logger.can_inf()) {                                                      \
    logger.inf(tenacitas::lib::generic::fmt(p_params));                        \
  }

#define TNCT_LOG_WAR(logger, p_params)                                         \
  if (logger.can_war()) {                                                      \
    logger.war(tenacitas::lib::generic::fmt(p_params));                        \
  }

#define TNCT_LOG_ERR(logger, p_params)                                         \
  logger.err(tenacitas::lib::generic::fmt(p_params));

#define TNCT_LOG_FAT(logger, p_params)                                         \
  logger.fat(tenacitas::lib::generic::fmt(p_params));

#define TNCT_LOG_TST(logger, p_params...)                                      \
  logger.tst(tenacitas::lib::generic::fmt(p_params));

#endif

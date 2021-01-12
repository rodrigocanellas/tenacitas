#ifndef TENACITAS_H
#define TENACITAS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifdef TENACITAS_LOG

/// \brief Wraps to the debug log function
#define DEB(p_log, p_params...)                                                \
  p_log.debug(this, __LINE__, p_params)

/// \brief Wraps to the info log function
#define INF(p_log, p_params...)                                                \
  p_log.info(this, __LINE__, p_params)

/// \brief Wraps to the warn log function
#define WAR(p_log, p_params...)                                                \
  p_log.warn(this, __LINE__, p_params)

/// \brief Wraps to the error log function
#define ERR(p_log, p_params...)                                                \
  p_log.error(this, __LINE__, p_params)

/// \brief Wraps to the fatal log function
#define FAT(p_log, p_params...)                                                \
  p_log.fatal(this, __LINE__, p_params)

#else

/// \brief Wraps to the debug log function
#define DEB(p_log, p_params)

/// \brief Wraps to the info log function
#define INF(p_log, p_params)

/// \brief Wraps to the warn log function
#define WARN(p_log, p_params)

/// \brief Wraps to the error log function
#define ERR(p_log, p_params)

/// \brief Wraps to the fatal log function
#define FAT(p_log, p_params)

#endif

#endif

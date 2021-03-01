#ifndef TENACITAS_MACROS_H
#define TENACITAS_MACROS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \brief Wraps to the debug log function
#define DEB(p_log, p_params...) p_log.debug(this, __func__, __LINE__, p_params)

/// \brief Wraps to the info log function
#define INF(p_log, p_params...) p_log.info(this, __func__, __LINE__, p_params)

/// \brief Wraps to the warn log function
#define WAR(p_log, p_params...) p_log.warn(this, __func__, __LINE__, p_params)

/// \brief Wraps to the error log function
#define ERR(p_log, p_params...) p_log.error(this, __func__, __LINE__, p_params)

/// \brief Wraps to the fatal log function
#define FAT(p_log, p_params...) p_log.fatal(this, __func__, __LINE__, p_params)

#endif

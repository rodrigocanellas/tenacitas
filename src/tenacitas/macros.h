#ifndef TENACITAS_MACROS_H
#define TENACITAS_MACROS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \brief Wraps to the debug p_log function
#define DEB(log, p_params...) log::debug(__FILE__, __LINE__, p_params)

/// \brief Wraps to the info log function
#define INF(log, p_params...) log::info(__FILE__, __LINE__, p_params)

/// \brief Wraps to the warn log function
#define WAR(log, p_params...) log::warn(__FILE__, __LINE__, p_params)

/// \brief Wraps to the error log function
#define ERR(log, p_params...) log::error(__FILE__, __LINE__, p_params)

/// \brief Wraps to the fatal log function
#define FAT(log, p_params...) log::fatal(__FILE__, __LINE__, p_params)

#endif

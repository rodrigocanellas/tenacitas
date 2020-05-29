#ifndef TENACITAS_COMMUNICATION_LOG_H
#define TENACITAS_COMMUNICATION_LOG_H

#include <cstring>
#include <string>

#include <experimental/filesystem>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

#define comm_log_debug(logger, p_params...)                                    \
  logger::debug(__FILE__, __LINE__, p_params)

#define comm_log_info(logger, p_params...)                                     \
  logger::info(__FILE__, __LINE__, p_params)

#define comm_log_warn(logger, p_params...)                                     \
  logger::warn(__FILE__, __LINE__, p_params)

#define comm_log_error(logger, p_params...)                                    \
  logger::error(__FILE__, __LINE__, p_params)

#define comm_log_fatal(logger, p_params...)                                    \
  logger::fatal(__FILE__, __LINE__, p_params)

} // namespace communication
} // namespace tenacitas

#endif // LOG_H

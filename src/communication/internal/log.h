#ifndef TENACITAS_COMMUNICATION_LOG_H
#define TENACITAS_COMMUNICATION_LOG_H

#include <cstring>
#include <string>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

std::string filename(const char *p_path);

#define comm_log_debug(logger, p_params...)                                    \
  logger::debug(tenacitas::communication::filename(__FILE__), __LINE__,        \
                p_params)

#define comm_log_info(logger, p_params...)                                     \
  logger::info(tenacitas::communication::filename(__FILE__), __LINE__, p_params)

#define comm_log_warn(logger, p_params...)                                     \
  logger::warn(tenacitas::communication::filename(__FILE__), __LINE__, p_params)

#define comm_log_error(logger, p_params...)                                    \
  logger::error(tenacitas::communication::filename(__FILE__), __LINE__,        \
                p_params)

#define comm_log_fatal(logger, p_params...)                                    \
  logger::fatal(tenacitas::communication::filename(__FILE__), __LINE__,        \
                p_params)

} // namespace communication
} // namespace tenacitas

#endif // LOG_H

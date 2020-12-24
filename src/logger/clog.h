#ifndef TENACITAS_LOGGER_CLOG_H
#define TENACITAS_LOGGER_CLOG_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

#include <logger/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {

/// \brief The log struct logs message to \p std::cout
struct clog : public log {
  inline explicit clog(std::string &&p_class)
      : log(std::move(p_class),
            [](std::string &&p_str) -> void { std::clog << p_str; }) {}
  inline explicit clog(const char *p_class)
      : log(p_class, [](std::string &&p_str) -> void { std::clog << p_str; }) {}
};

} // namespace logger
} // namespace tenacitas

#endif

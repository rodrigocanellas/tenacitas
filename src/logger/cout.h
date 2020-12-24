#ifndef TENACITAS_LOGGER_COUT_H
#define TENACITAS_LOGGER_COUT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <string>

#include <logger/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {

/// \brief The log struct logs message to \p std::cout
struct cout : public log {
  inline explicit cout(std::string &&p_class)
      : log(std::move(p_class),
            [](std::string &&p_str) -> void { std::cout << p_str; }) {}
  inline explicit cout(const char *p_class)
      : log(p_class, [](std::string &&p_str) -> void { std::cout << p_str; }) {}
};

} // namespace logger
} // namespace tenacitas

#endif

#ifndef TENACITAS_LOGGER_BUS_COUT_MEDIA_H
#define TENACITAS_LOGGER_BUS_COUT_MEDIA_H

#include <iostream>
#include <mutex>
#include <string>

#include <logger/business/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace business {

///
/// \brief The cout_media struct implement a \p t_media type to be used in a
/// tenacitas::logger::business::log class, allowing log messages to be written to a
/// std::cout
///
struct cout_media
{
    cout_media() = default;
    cout_media(cout_media&&) noexcept = default;
    cout_media(const cout_media&) = delete;
    cout_media& operator=(const cout_media&) = delete;
    cout_media& operator=(cout_media&&) noexcept = default;
    ~cout_media() = default;

    ///
    /// \brief operator () writes a string to std::cout
    /// \param p_str the string to be writtern
    ///
    inline void operator()(std::string&& p_str) { std::cout << p_str; }

    ///
    /// \brief type returns a string identifying the media type
    /// \return "cout"
    ///
    inline static std::string type() { return "cout"; }
};

} // namespace business
} // namespace logger
} // namespace tenacitas

#endif // COUT_MEDIA_H

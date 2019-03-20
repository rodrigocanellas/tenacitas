#ifndef TENACITAS_LOGGER_BUS_CERR_MEDIA_H
#define TENACITAS_LOGGER_BUS_CERR_MEDIA_H

#include <iostream>
#include <mutex>
#include <string>

#include <logger/bus/internal/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace logger {
/// \brief namespace of the group of classes
namespace bus {

///
/// \brief The cerr_media struct implement a \p t_media type to be used in a
/// tenacitas::logger::bus::log class, allowing log messages to be written to a
/// std::cerr
///
struct cerr_media
{
    cerr_media() = default;
    cerr_media(cerr_media&&) noexcept = default;
    cerr_media(const cerr_media&) = delete;
    cerr_media& operator=(const cerr_media&) = delete;
    cerr_media& operator=(cerr_media&&) noexcept = default;
    ~cerr_media() = default;

    ///
    /// \brief operator () writes a string to std::cerr
    /// \param p_str the string to be writtern
    ///
    inline void operator()(std::string&& p_str)
    {
        std::cerr << p_str;
        std::cerr.flush();
    }

    ///
    /// \brief type returns a string identifying the media type
    /// \return "cerr"
    ///
    inline static std::string type() { return "cerr"; }
};

} // namespace bus
} // namespace logger
} // namespace tenacitas

#endif // CERR_MEDIA_H

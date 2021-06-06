#ifndef TENACITAS_CALENDAR_H
#define TENACITAS_CALENDAR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

/// \brief master namespace
namespace tenacitas {

/// \brief classes to manipulate calendar and time
namespace calendar {

/// \brief Converts time formats
///
/// \tparam t_to destination type
///
/// \tparam t_from origin type
///
/// \param p_from origin value
///
/// \return p_from converted to t_to type
template <typename t_to, typename t_from>
inline t_to convert(const t_from &p_from) {
    return std::chrono::duration_cast<t_to>(p_from);
}

/// \brief Calculates 'now' in different time units
///
/// \tparam use exists to make \p log to be compiled only it is really used
template <bool use = true>
struct now {

    /// \brief 'now' in microseconds
    template <typename t_int = uint64_t>
    inline static t_int microsecs() {
        return static_cast<t_int>(
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count());
    }

    /// \brief 'now' as a string in the sformat YYYY-MM-DD HH::MM::SS,999999
    static std::string iso8601_microsecs() {
        using namespace std;
        using namespace chrono;

        const auto _microsecs = microsecs();
        const time_t _time_t = static_cast<time_t>(_microsecs / 1000000);

        const auto _remainder = _microsecs % 1000000;
        stringstream _stream;
        _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
                << std::setw(6) << std::setfill('0') << std::left << _remainder;
        return _stream.str();
    }

    /// \brief A specific time as a string in the format 'YYYY-MM-DD
    /// HH::MM::SS,999999'
    ///
    /// \param p_microsecs is the time one wants to convert to string
    static std::string iso8601_microsecs(uint64_t p_microsecs) {
        using namespace std;
        using namespace chrono;

        const time_t _time_t = static_cast<time_t>(p_microsecs / 1000000);

        const auto _remainder = p_microsecs % 1000000;
        stringstream _stream;
        _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
                << std::setw(6) << std::setfill('0') << std::left << _remainder;
        return _stream.str();
    }

    /// \brief 'now' in milliseconds
    template <typename t_int = uint64_t>
    inline static t_int millisecs() {
        return static_cast<t_int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count());
    }

    /// \brief 'now' as a string in the format 'YYYY-MM-DD HH::MM::SS,999'
    // TODO test iso8601_millisecs
    static std::string iso8601_millisecs() {
        using namespace std;
        using namespace chrono;

        const auto _microsecs = millisecs();
        const time_t _time_t = static_cast<time_t>(_microsecs / 1000);

        const auto _remainder = _microsecs % 1000;
        stringstream _stream;
        _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
                << std::setw(3) << std::setfill('0') << std::left << _remainder;
        return _stream.str();
    }

    /// \brief 'now' in seconds
    template <typename t_int = uint64_t>
    inline static t_int secs() {
        return static_cast<t_int>(
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count());
    }

    /// \brief 'now' as a string in the format 'YYYY-MM-DD HH::MM::SS'
    // TODO test iso8601_secs
    static std::string iso8601_secs() {
        using namespace std;
        using namespace chrono;

        const time_t _time_t = secs<time_t>();

        stringstream _stream;
        _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S");
        return _stream.str();
    }

    /// \brief 'now' in minutes
    // TODO test minutes
    template <typename t_int = uint64_t>
    inline static t_int minutes() {
        return static_cast<t_int>(
            std::chrono::duration_cast<std::chrono::minutes>(
                std::chrono::system_clock::now().time_since_epoch())
                .count());
    }

    /// \brief 'now' in hours
    // TODO test hours
    template <typename t_int = uint64_t>
    inline static t_int hours() {
        return static_cast<t_int>(
            std::chrono::duration_cast<std::chrono::hours>(
                std::chrono::system_clock::now().time_since_epoch())
                .count());
    }

    /// \brief 'now' in days
    // TODO test days
    template <typename t_int = uint64_t>
    inline static t_int days() {
        return static_cast<t_int>(
            std::chrono::duration_cast<std::chrono::hours>(
                std::chrono::system_clock::now().time_since_epoch())
                .count() /
            24);
    }

    /// \brief 'now' in months
    // TODO test months
    template <typename t_int = uint64_t>
    inline static t_int months() {
        return static_cast<t_int>(days() / 30);
    }

    /// \brief 'now' in years
    // TODO test years
    template <typename t_int = uint64_t>
    inline static t_int years() {
        return static_cast<t_int>(months() / 12);
    }
};

} // namespace calendar

} // namespace tenacitas

#endif

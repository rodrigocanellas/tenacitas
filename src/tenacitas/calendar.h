#ifndef TENACITAS_CALENDAR_H
#define TENACITAS_CALENDAR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <string>

/// \brief master namespace
namespace tenacitas {

/// \brief about calendar and time
namespace calendar {

/// \brief functions that return 'now' in different time units
template <bool use = true> struct now {

  /// \brief now in microseconds
  template <typename t_int = uint64_t> inline static t_int microsecs_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief YYYY-MM-DD HH::MM::SS,999999
  static std::string microsecs_str() {
    using namespace std;
    using namespace chrono;

    const auto _microsecs = microsecs_num();
    const time_t _time_t = static_cast<time_t>(_microsecs / 1000000);

    const auto _remainder = _microsecs % 1000000;
    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
            << std::setw(6) << std::setfill('0') << std::left << _remainder;
    return _stream.str();
  }

  /// \brief now in milliseconds
  template <typename t_int = uint64_t> inline static t_int millisecs_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief YYYY-MM-DD HH::MM::SS,999
  // TODO test millisecs_str
  static std::string millisecs_str() {
    using namespace std;
    using namespace chrono;

    const auto _microsecs = millisecs_num();
    const time_t _time_t = static_cast<time_t>(_microsecs / 1000);

    const auto _remainder = _microsecs % 1000;
    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S,")
            << std::setw(3) << std::setfill('0') << std::left << _remainder;
    return _stream.str();
  }

  /// \brief now in seconds
  template <typename t_int = uint64_t> inline static t_int secs_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief YYYY-MM-DD HH::MM::SS
  // TODO test secs_str
  static std::string secs_str() {
    using namespace std;
    using namespace chrono;

    const time_t _time_t = secs_num<time_t>();

    stringstream _stream;
    _stream << put_time(std::localtime(&_time_t), "%Y-%m-%d %H:%M:%S");
    return _stream.str();
  }

  /// \brief now in minutes
  // TODO test minutes_num
  template <typename t_int = uint64_t> inline static t_int minutes_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief now in hours
  // TODO test hours_num
  template <typename t_int = uint64_t> inline static t_int hours_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  }

  /// \brief now in days
  // TODO test days_num
  template <typename t_int = uint64_t> inline static t_int days_num() {
    return static_cast<t_int>(
        std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now().time_since_epoch())
            .count() /
        24);
  }

  /// \brief now in months
  // TODO test months_num
  template <typename t_int = uint64_t> inline static t_int months_num() {
    return static_cast<t_int>(days_num() / 30);
  }

  /// \brief now in years
  // TODO test years_num
  template <typename t_int = uint64_t> inline static t_int years_num() {
    return static_cast<t_int>(months_num() / 12);
  }
};
} // namespace calendar

} // namespace tenacitas

#endif

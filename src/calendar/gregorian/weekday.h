#ifndef TENACITAS_CALENDAR_GREGORIAN_WEEKDAY_H
#define TENACITAS_CALENDAR_GREGORIAN_WEEKDAY_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/day.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/hour.h>
#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/second.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The weekday struct represents valid a weekday value
///
enum class weekday : uint8_t {
  sun = 0,
  mon = 1,
  tue = 2,
  wed = 3,
  thu = 4,
  fri = 5,
  sat = 6
};

///
/// \brief operator - how many days between two weekdays
/// \param p_left left operand
/// \param p_right right operand
/// \return  amount of days betwen the weekdays
///
constexpr days operator-(const weekday &p_left, const weekday &p_right) {
  if (p_left > p_right) {
    return days(static_cast<uint8_t>(p_left) - static_cast<uint8_t>(p_right));
  }
  if (p_left == p_right) {
    return days(0);
  }

  days _d0(weekday::sat - p_right);
  days _d1(p_left - weekday::sun);
  _d1++;
  return days(_d0 - _d1);
}

///
/// \brief operator ++ next weekday
/// \param p_w weekday to be advanced
///
inline constexpr void operator++(weekday &p_w) {
  p_w = (p_w == weekday::sat ? weekday::sun
                             : weekday(static_cast<uint8_t>(p_w) + 1));
  //  if (p_w == weekday::sat) {
  //    p_w = weekday::sun;
  //  } else {
  //    p_w = weekday(static_cast<uint8_t>(p_w) + 1);
  //  }
}

///
/// \brief operator ++ next weekday
/// \param p_w weekday to be advanced
///
constexpr void operator++(weekday &p_w, int) {
  p_w = (p_w == weekday::sat ? weekday::sun
                             : weekday(static_cast<uint8_t>(p_w) + 1));

  //  if (p_w == weekday::sat) {
  //    p_w = weekday::sun;
  //  } else {
  //    p_w = weekday(static_cast<uint8_t>(p_w) + 1);
  //  }
}

///
/// \brief operator -- previous weekday
/// \param p_w weekday to be go back
///
constexpr void operator--(weekday &p_w) {
  if (p_w == weekday::sun) {
    p_w = weekday::sat;
  } else {
    p_w = weekday(static_cast<uint8_t>(p_w) - 1);
  }
}

///
/// \brief operator -- previous weekday
/// \param p_w weekday to be go back
///
constexpr void operator--(weekday &p_w, int) {
  if (p_w == weekday::sun) {
    p_w = weekday::sat;
  } else {
    p_w = weekday(static_cast<uint8_t>(p_w) - 1);
  }
}

///
/// \brief operator << prints the weekday
/// \param p_out output stream
/// \param p_weekday weekday to be printed
/// \return output stream
///
inline std::ostream &operator<<(std::ostream &p_out, const weekday &p_weekday) {
  if (p_weekday == weekday::sun) {
    p_out << "sun";
  } else if (p_weekday == weekday::mon) {
    p_out << "mon";
  } else if (p_weekday == weekday::tue) {
    p_out << "tue";
  } else if (p_weekday == weekday::wed) {
    p_out << "wed";
  } else if (p_weekday == weekday::thu) {
    p_out << "thu";
  } else if (p_weekday == weekday::fri) {
    p_out << "fri";
  } else {
    p_out << "sat";
  }

  return p_out;
}

// template <typename t_from, typename t_to> constexpr t_to convert();

// template <> constexpr days convert<weekday, days>() { return days(7); }

/////
///// \brief The weekday struct represents valid a weekday value
/////
// struct weekday {

//  //  typedef amount_t<weekday> amount;

//  weekday() = delete;

//  static constexpr uint8_t sun = {0};
//  static constexpr uint8_t mon = {1};
//  static constexpr uint8_t tue = {2};
//  static constexpr uint8_t wed = {3};
//  static constexpr uint8_t thu = {4};
//  static constexpr uint8_t fri = {5};
//  static constexpr uint8_t sat = {6};

//  inline friend std::ostream &operator<<(std::ostream &p_out,
//                                         const weekday &p_weekday) {
//    //    if (p_weekday == weekday::sun) {
//    //      p_out << 'D';
//    //    } else if (p_weekday == weekday::sat) {
//    //      p_out << 'S';
//    //    } else {
//    //      p_out << static_cast<uint32_t>(p_weekday.m_value + 1);
//    //    }
//    if (p_weekday == weekday::sun) {
//      p_out << "sun";
//    } else if (p_weekday == weekday::mon) {
//      p_out << "mon";
//    } else if (p_weekday == weekday::tue) {
//      p_out << "tue";
//    } else if (p_weekday == weekday::wed) {
//      p_out << "wed";
//    } else if (p_weekday == weekday::thu) {
//      p_out << "thu";
//    } else if (p_weekday == weekday::fri) {
//      p_out << "fri";
//    } else {
//      p_out << "sat";
//    }

//    return p_out;
//  }

//  template <typename t_int> static weekday create(t_int p_value) {
//    switch (static_cast<decltype(m_value)>(p_value)) {
//    case 0:
//      return sun;
//    case 1:
//      return mon;
//    case 2:
//      return tue;
//    case 3:
//      return wed;
//    case 4:
//      return thu;
//    case 5:
//      return fri;
//    case 6:
//      return sat;
//    default:
//      throw std::runtime_error(std::to_string(p_value) +
//                               " is not a valid number for weekday");
//    }
//  }

//  constexpr inline bool operator>(const weekday &p_weekday) const {
//    return m_value > p_weekday.m_value;
//  }

//  constexpr inline bool operator<(const weekday &p_weekday) const {
//    return m_value < p_weekday.m_value;
//  }

//  constexpr inline bool operator>=(const weekday &p_weekday) const {
//    return m_value >= p_weekday.m_value;
//  }

//  constexpr inline bool operator<=(const weekday &p_weekday) const {
//    return m_value <= p_weekday.m_value;
//  }

//  constexpr inline bool operator==(const weekday &p_weekday) const {
//    return m_value == p_weekday.m_value;
//  }

//  constexpr inline bool operator!=(const weekday &p_weekday) const {
//    return m_value != p_weekday.m_value;
//  }

//  //  static inline days get_days() { return days(7); }

//  //  static inline hours get_hours() { return day::get_hours() * get_days();
//  }

//  //  static inline minutes get_minutes() {
//  //    return hour::get_minutes() * get_hours();
//  //  }

//  //  static inline seconds get_seconds() {
//  //    return minute::get_seconds() * get_minutes();
//  //  }

//  constexpr days operator-(const weekday &p_weekday) const {
//    if (m_value > p_weekday.m_value) {
//      return days(m_value - p_weekday.m_value);
//    }
//    if (m_value == p_weekday.m_value) {
//      return days(0);
//    }

//    return days((sat - p_weekday.m_value) + (m_value - sun + 1));
//  }

//  constexpr inline days from_sunday() const { return (*this - weekday::sun); }

//  constexpr inline days until_saturday() const {
//    return (weekday(sat) - *this);
//  }

// private:
//  constexpr inline uint8_t value() const { return m_value; }

//  constexpr inline weekday(uint8_t p_value) : m_value(p_value) {}

// private:
//  uint8_t m_value;
//};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // WEEKDAY_H

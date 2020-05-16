#ifndef TENACITAS_CALENDAR_GREGORIAN_WEEKDAY_H
#define TENACITAS_CALENDAR_GREGORIAN_WEEKDAY_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/internal/amount.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

struct weekday {

  /// \brief constructor creates a valid \p weekday if possible
  ///
  /// \tparam type of number used to create a \p weekday object
  ///
  /// \param p_value value used to create a \p weekday object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p weekday object
  ///
  template <typename t_int> inline explicit constexpr weekday(t_int p_value) {
    if ((p_value > 7) || (p_value < 1)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid weekday value");
    }
    m_value = static_cast<decltype(m_value)>(std::move(p_value));
  }

  ///
  /// \brief  constructor from a \p amount_t<weekday>, i.e., \p weekdays
  /// \param p_amount amount of weekdays used to create a \p weekday
  ///
  constexpr explicit inline weekday(amount_t<weekday> p_amount)
      : weekday(p_amount.value()) {}

  ///
  /// \brief operator << prints the weekday
  /// \param p_out output stream
  /// \param p_weekday weekday to be printed
  /// \return output stream
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const weekday &p_weekday) {
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

  /// \brief possible weekdays values

  static const weekday sun;
  static const weekday mon;
  static const weekday tue;
  static const weekday wed;
  static const weekday thu;
  static const weekday fri;
  static const weekday sat;

  ///
  /// \brief day assignment from a \p amount_t<day>, i.e., \p days
  /// \param p_amount amount of days used to create a \p day
  ///
  constexpr inline weekday &operator=(amount_t<weekday> p_amount) {
    assign(p_amount.value());
    return *this;
  }

  ///
  /// \brief operator >
  /// \param p_weekday
  /// \return
  ///
  inline constexpr bool operator>(const weekday &p_weekday) const {
    return m_value > p_weekday.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_weekday
  /// \return
  ///
  inline constexpr bool operator<(const weekday &p_weekday) const {
    return m_value < p_weekday.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_weekday
  /// \return
  ///
  inline constexpr bool operator>=(const weekday &p_weekday) const {
    return m_value >= p_weekday.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_weekday
  /// \return
  ///
  inline constexpr bool operator<=(const weekday &p_weekday) const {
    return m_value <= p_weekday.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_weekday
  /// \return
  ///
  inline constexpr bool operator==(const weekday &p_weekday) const {
    return m_value == p_weekday.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_weekday
  /// \return
  ///
  inline constexpr bool operator!=(const weekday &p_weekday) const {
    return m_value != p_weekday.m_value;
  }

  constexpr auto value() const { return m_value; }

private:
  template <typename t_int> constexpr void assign(t_int p_value) {
    if ((p_value > 7) || (p_value < 1)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid weekday value");
    }
    m_value = static_cast<decltype(m_value)>(std::move(p_value));
  }

private:
  uint16_t m_value;
};

/////
///// \brief operator - how many days between two weekdays
///// \param p_left left operand
///// \param p_right right operand
///// \return  amount of days betwen the weekdays
/////
// constexpr days operator-(const weekday &p_left, const weekday &p_right) {
//  if (p_left > p_right) {
//    return days(static_cast<uint8_t>(p_left) - static_cast<uint8_t>(p_right));
//  }
//  if (p_left == p_right) {
//    return days(0);
//  }

//  days _d0(weekday::sat - p_right);
//  days _d1(p_left - weekday::sun);
//  _d1++;
//  return days(_d0 - _d1);
//}

/////
///// \brief operator ++ next weekday
///// \param p_w weekday to be advanced
/////
// inline constexpr void operator++(weekday &p_w) {
//  p_w = (p_w == weekday::sat ? weekday::sun
//                             : weekday(static_cast<uint8_t>(p_w) + 1));
//}

/////
///// \brief operator ++ next weekday
///// \param p_w weekday to be advanced
/////
// inline constexpr void operator++(weekday &p_w, int) {
//  p_w = (p_w == weekday::sat ? weekday::sun
//                             : weekday(static_cast<uint8_t>(p_w) + 1));
//}

/////
///// \brief operator -- previous weekday
///// \param p_w weekday to be go back
/////
// inline constexpr void operator--(weekday &p_w) {
//  p_w = ((p_w == weekday::sun) ? weekday::sat
//                               : weekday(static_cast<uint8_t>(p_w) - 1));
//}

/////
///// \brief operator -- previous weekday
///// \param p_w weekday to be go back
/////
// constexpr void operator--(weekday &p_w, int) {
//  p_w = (p_w == weekday::sun) ? weekday::sat
//                              : weekday(static_cast<uint8_t>(p_w) - 1);
//}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // WEEKDAY_H

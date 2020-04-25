#ifndef TENACITAS_CALENDAR_GREGORIAN_MONTH_H
#define TENACITAS_CALENDAR_GREGORIAN_MONTH_H

#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The month struct represents a valid month value
///
struct month {

  /// \brief constructor creates a valid \p month if possible
  ///
  /// \tparam type of number used to create a \p month object
  ///
  /// \param p_value value used to create a \p month object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p month object
  ///
  template <typename t_int> inline explicit month(t_int p_value) {
    if ((p_value > 12) || (p_value <= 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid month value");
    }
    m_value = static_cast<decltype(m_value)>(std::move(p_value));
  }

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_month
  /// \return
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const month &p_month) {

    if (p_month == month::jan) {
      p_out << "jan";
    } else if (p_month == month::feb) {
      p_out << "feb";
    } else if (p_month == month::mar) {
      p_out << "mar";
    } else if (p_month == month::apr) {
      p_out << "apr";
    } else if (p_month == month::may) {
      p_out << "may";
    } else if (p_month == month::jun) {
      p_out << "jun";
    } else if (p_month == month::jul) {
      p_out << "jul";
    } else if (p_month == month::ago) {
      p_out << "ago";
    } else if (p_month == month::sep) {
      p_out << "sep";
    } else if (p_month == month::oct) {
      p_out << "oct";
    } else if (p_month == month::nov) {
      p_out << "nov";
    } else if (p_month == month::dec) {
      p_out << "dec";
    }
    return p_out;
  }

  /// \brief valid month values

  static const month jan;
  static const month feb;
  static const month mar;
  static const month apr;
  static const month may;
  static const month jun;
  static const month jul;
  static const month ago;
  static const month sep;
  static const month oct;
  static const month nov;
  static const month dec;

  ///
  /// \brief operator >
  /// \param p_month
  /// \return
  ///
  inline constexpr bool operator>(const month &p_month) const {
    return m_value > p_month.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_month
  /// \return
  ///
  inline constexpr bool operator<(const month &p_month) const {
    return m_value < p_month.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_month
  /// \return
  ///
  inline constexpr bool operator>=(const month &p_month) const {
    return m_value >= p_month.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_month
  /// \return
  ///
  inline constexpr bool operator<=(const month &p_month) const {
    return m_value <= p_month.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_month
  /// \return
  ///
  inline constexpr bool operator==(const month &p_month) const {
    return m_value == p_month.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_month
  /// \return
  ///
  inline constexpr bool operator!=(const month &p_month) const {
    return m_value != p_month.m_value;
  }

private:
  ///
  /// \brief m_value
  ///
  uint8_t m_value;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // MONTH_H

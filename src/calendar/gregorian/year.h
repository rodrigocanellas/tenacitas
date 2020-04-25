#ifndef TENACITAS_CALENDAR_GREGORIAN_YEAR_H
#define TENACITAS_CALENDAR_GREGORIAN_YEAR_H

#include <cstdint>
#include <iostream>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The year struct represents a year
///
struct year {

  /// \brief constructor creates a valid \p year
  ///
  /// \tparam type of number used to create a \p year object
  ///
  /// \param p_value value used to create a \p minute object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p minute object
  ///
  template <typename t_int>
  inline explicit constexpr year(t_int p_value)
      : m_value(static_cast<decltype(m_value)>(std::move(p_value))) {}

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_year
  /// \return
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const year &p_year) {
    p_out << static_cast<uint16_t>(p_year.m_value);
    return p_out;
  }

  ///
  /// \brief is_leap
  /// \return
  ///
  inline constexpr bool is_leap() const {
    return m_value % 4 == 0 && (m_value % 100 != 0 || m_value % 400 == 0);
  }

  ///
  /// \brief operator >
  /// \param p_year
  /// \return
  ///
  inline constexpr bool operator>(const year &p_year) const {
    return m_value > p_year.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_year
  /// \return
  ///
  inline constexpr bool operator<(const year &p_year) const {
    return m_value < p_year.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_year
  /// \return
  ///
  inline constexpr bool operator>=(const year &p_year) const {
    return m_value >= p_year.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_year
  /// \return
  ///
  inline constexpr bool operator<=(const year &p_year) const {
    return m_value <= p_year.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_year
  /// \return
  ///
  inline constexpr bool operator==(const year &p_year) const {
    return m_value == p_year.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_year
  /// \return
  ///
  inline constexpr bool operator!=(const year &p_year) const {
    return m_value != p_year.m_value;
  }

  //  static inline months get_months() { return months(12); }

  //  static inline days get_days(const year &p_year) {
  //    return month::jan.get_days(month::jan, p_year) +
  //           month::feb.get_days(month::feb, p_year) +
  //           month::mar.get_days(month::mar, p_year) +
  //           month::apr.get_days(month::apr, p_year) +
  //           month::may.get_days(month::may, p_year) +
  //           month::jun.get_days(month::jun, p_year) +
  //           month::jul.get_days(month::jul, p_year) +
  //           month::ago.get_days(month::ago, p_year) +
  //           month::sep.get_days(month::sep, p_year) +
  //           month::oct.get_days(month::oct, p_year) +
  //           month::nov.get_days(month::nov, p_year) +
  //           month::dec.get_days(month::dec, p_year);
  //  }

  //  static inline hours gett_hours(const year &p_year) {
  //    return month::jan.get_hours(month::jan, p_year) +
  //           month::feb.get_hours(month::feb, p_year) +
  //           month::mar.get_hours(month::mar, p_year) +
  //           month::apr.get_hours(month::apr, p_year) +
  //           month::may.get_hours(month::may, p_year) +
  //           month::jun.get_hours(month::jun, p_year) +
  //           month::jul.get_hours(month::jul, p_year) +
  //           month::ago.get_hours(month::ago, p_year) +
  //           month::sep.get_hours(month::sep, p_year) +
  //           month::oct.get_hours(month::oct, p_year) +
  //           month::nov.get_hours(month::nov, p_year) +
  //           month::dec.get_hours(month::dec, p_year);
  //  }

  //  static inline minutes get_minutes(const year &p_year) {
  //    return month::jan.get_minutes(month::jan, p_year) +
  //           month::feb.get_minutes(month::feb, p_year) +
  //           month::mar.get_minutes(month::mar, p_year) +
  //           month::apr.get_minutes(month::apr, p_year) +
  //           month::may.get_minutes(month::may, p_year) +
  //           month::jun.get_minutes(month::jun, p_year) +
  //           month::jul.get_minutes(month::jul, p_year) +
  //           month::ago.get_minutes(month::ago, p_year) +
  //           month::sep.get_minutes(month::sep, p_year) +
  //           month::oct.get_minutes(month::oct, p_year) +
  //           month::nov.get_minutes(month::nov, p_year) +
  //           month::dec.get_minutes(month::dec, p_year);
  //  }

  //  static inline seconds get_seconds(const year &p_year) {
  //    return month::jan.get_seconds(month::jan, p_year) +
  //           month::feb.get_seconds(month::feb, p_year) +
  //           month::mar.get_seconds(month::mar, p_year) +
  //           month::apr.get_seconds(month::apr, p_year) +
  //           month::may.get_seconds(month::may, p_year) +
  //           month::jun.get_seconds(month::jun, p_year) +
  //           month::jul.get_seconds(month::jul, p_year) +
  //           month::ago.get_seconds(month::ago, p_year) +
  //           month::sep.get_seconds(month::sep, p_year) +
  //           month::oct.get_seconds(month::oct, p_year) +
  //           month::nov.get_seconds(month::nov, p_year) +
  //           month::dec.get_seconds(month::dec, p_year);
  //  }

private:
  ///
  /// \brief m_value
  ///
  int32_t m_value;
};

///
/// \brief operator <invalid>
/// \param p_value
/// \return
///
inline constexpr year operator"" _year(unsigned long long p_value) {
  return year(p_value);
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // YEAR_H

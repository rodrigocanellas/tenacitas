#ifndef TENACITAS_MEASURES_MONTH_H
#define TENACITAS_MEASURES_MONTH_H

#include <cstdint>
#include <iostream>

#include <measures/internal/amount_t.h>

namespace tenacitas {
namespace measures {

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
    assign(p_value);
  }

  ///
  /// \brief month constructor from a \p amount_t<month>, i.e., \p months
  /// \param p_amount amount of months used to create a \p month
  ///
  explicit inline month(amount_t<month> p_amount) : month(p_amount.value()) {}

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_month
  /// \return
  ///
  friend std::ostream &operator<<(std::ostream &p_out, const month &p_month);

  /// \brief valid month values

  //  static const month jan;
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
  /// \brief month assignment from a \p amount_t<month>, i.e., \p months
  /// \param p_amount amount of months used to create a \p month
  ///
  inline month &operator=(amount_t<month> p_amount) {
    assign(p_amount.value());
    return *this;
  }

  ///
  /// \brief operator >
  /// \param p_month
  /// \return
  ///
  inline bool operator>(const month &p_month) const {
    return m_value > p_month.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_month
  /// \return
  ///
  inline bool operator<(const month &p_month) const {
    return m_value < p_month.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_month
  /// \return
  ///
  inline bool operator>=(const month &p_month) const {
    return m_value >= p_month.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_month
  /// \return
  ///
  inline bool operator<=(const month &p_month) const {
    return m_value <= p_month.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_month
  /// \return
  ///
  inline bool operator==(const month &p_month) const {
    return m_value == p_month.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_month
  /// \return
  ///
  inline bool operator!=(const month &p_month) const {
    return m_value != p_month.m_value;
  }

  template <typename t_value> t_value value() const {
    return static_cast<t_value>(m_value);
  }

private:
  template <typename t_int> void assign(t_int p_value) {
    if ((static_cast<decltype(m_value)>(p_value) > 12) || (p_value <= 0)) {
      throw std::runtime_error(std::to_string(p_value) +
                               " is not a valid month value");
    }
    m_value = static_cast<decltype(m_value)>(p_value);
  }

private:
  ///
  /// \brief m_value
  ///
  uint16_t m_value;
};

} // namespace measures
} // namespace tenacitas

#endif // MONTH_H

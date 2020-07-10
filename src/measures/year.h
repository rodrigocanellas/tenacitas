#ifndef TENACITAS_MEASURES_YEAR_H
#define TENACITAS_MEASURES_YEAR_H

#include <cstdint>
#include <iostream>

#include <measures/internal/amount_t.h>

namespace tenacitas {
namespace measures {

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
  inline explicit year(t_int p_value)
      : m_value(static_cast<decltype(m_value)>(p_value)) {}

  ///
  /// \brief year constructor from a \p amount_t<year>, i.e., \p years
  /// \param p_amount amount of years used to create a \p year
  ///
  explicit inline year(amount_t<year> p_amount) : year(p_amount.value()) {}

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
  inline bool is_leap() const {
    return m_value % 4 == 0 && (m_value % 100 != 0 || m_value % 400 == 0);
  }

  ///
  /// \brief year assignment from a \p amount_t<year>, i.e., \p years
  /// \param p_amount amount of years used to create a \p year
  ///
  inline year &operator=(amount_t<year> p_amount) {
    m_value = p_amount.value();
    return *this;
  }

  ///
  /// \brief operator >
  /// \param p_year
  /// \return
  ///
  inline bool operator>(const year &p_year) const {
    return m_value > p_year.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_year
  /// \return
  ///
  inline bool operator<(const year &p_year) const {
    return m_value < p_year.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_year
  /// \return
  ///
  inline bool operator>=(const year &p_year) const {
    return m_value >= p_year.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_year
  /// \return
  ///
  inline bool operator<=(const year &p_year) const {
    return m_value <= p_year.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_year
  /// \return
  ///
  inline bool operator==(const year &p_year) const {
    return m_value == p_year.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_year
  /// \return
  ///
  inline bool operator!=(const year &p_year) const {
    return m_value != p_year.m_value;
  }

  template <typename t_value> t_value value() const {
    return static_cast<t_value>(m_value);
  }

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
inline year operator"" _year(unsigned long long p_value) {
  return year(p_value);
}

} // namespace measures
} // namespace tenacitas

#endif // YEAR_H

#ifndef TENACITAS_MEASURES_MILLISECOND_H
#define TENACITAS_MEASURES_MILLISECOND_H

#include <cstdint>
#include <iostream>

#include <measures/internal/amount_t.h>

namespace tenacitas {
namespace measures {
namespace time {

///
/// \brief The millisecond struct represents a millisecond
///
struct millisecond {

  /// \brief constructor creates a valid \p millisecond
  ///
  /// \tparam type of number used to create a \p millisecond object
  ///
  /// \param p_value value used to create a \p minute object
  ///
  /// \exception std::runtime_error if \p p_value is not a valid value to create
  /// a \p minute object
  ///
  template <typename t_int>
  inline explicit millisecond(t_int p_value)
      : m_value(static_cast<decltype(m_value)>(p_value)) {}

  ///
  /// \brief millisecond constructor from a \p amount_t<millisecond>, i.e., \p
  /// milliseconds \param p_amount amount of milliseconds used to create a \p
  /// millisecond
  ///
  explicit inline millisecond(amount_t<millisecond> p_amount)
      : millisecond(p_amount.value()) {}

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_millisecond
  /// \return
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const millisecond &p_millisecond) {
    p_out << static_cast<uint16_t>(p_millisecond.m_value);
    return p_out;
  }

  ///
  /// \brief millisecond assignment from a \p amount_t<millisecond>, i.e., \p
  /// milliseconds \param p_amount amount of milliseconds used to create a \p
  /// millisecond
  ///
  inline millisecond &operator=(amount_t<millisecond> p_amount) {
    m_value = p_amount.value();
    return *this;
  }

  ///
  /// \brief operator >
  /// \param p_millisecond
  /// \return
  ///
  inline bool operator>(const millisecond &p_millisecond) const {
    return m_value > p_millisecond.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_millisecond
  /// \return
  ///
  inline bool operator<(const millisecond &p_millisecond) const {
    return m_value < p_millisecond.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_millisecond
  /// \return
  ///
  inline bool operator>=(const millisecond &p_millisecond) const {
    return m_value >= p_millisecond.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_millisecond
  /// \return
  ///
  inline bool operator<=(const millisecond &p_millisecond) const {
    return m_value <= p_millisecond.m_value;
  }

  ///
  /// \brief operator ==
  /// \param p_millisecond
  /// \return
  ///
  inline bool operator==(const millisecond &p_millisecond) const {
    return m_value == p_millisecond.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_millisecond
  /// \return
  ///
  inline bool operator!=(const millisecond &p_millisecond) const {
    return m_value != p_millisecond.m_value;
  }

  template<typename t_type>
  t_type value() const { return static_cast<t_type>(m_value); }


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
inline millisecond operator"" _millisecond(unsigned long long p_value) {
  return millisecond(p_value);
}

} // namespace time
} // namespace measures
} // namespace tenacitas

#endif // YEAR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_GENERIC_ID_H
#define TNCT_GENERIC_ID_H

#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace tnct::generic {

/// \brief creation of a unique identifier
///
/// \tparam is a means to create two diferent 'id' classes, lile 'id<0>' is not
/// the same as 'id<1'>.
template <typename t_what> struct id {

  /// \brief Identifier self generated
  id() {
    std::this_thread::sleep_for(1ms);
    m_value = static_cast<decltype(m_value)>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
  }

  id(const id &) = default;
  id(id &&) = default;
  id &operator=(const id &) = default;
  id &operator=(id &&) = default;

  ~id() = default;

  inline constexpr bool operator==(const id &p_id) const {
    return m_value == p_id.m_value;
  }

  inline constexpr bool operator!=(const id &p_id) const {
    return m_value != p_id.m_value;
  }

  inline constexpr bool operator>(const id &p_id) const {
    return m_value > p_id.m_value;
  }

  inline constexpr bool operator<(const id &p_id) const {
    return m_value < p_id.m_value;
  }

  inline constexpr bool operator>=(const id &p_id) const {
    return m_value >= p_id.m_value;
  }

  inline constexpr bool operator<=(const id &p_id) const {
    return m_value <= p_id.m_value;
  }

  friend std::ostream &operator<<(std::ostream &p_out, const id &p_id) {
    p_out << std::to_string(p_id.m_value);

    return p_out;
  }

private:
  size_t m_value{0};
};

} // namespace tnct::generic

#endif

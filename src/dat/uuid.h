/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_DAT_UUID_H
#define TENACITAS_LIB_DAT_UUID_H

#include <array>
#include <initializer_list>
#include <iostream>

namespace tenacitas::lib::dat {

struct uuid {
  uuid() = default;
  uuid(const uuid &) = default;
  uuid(uuid &) = default;

  uuid(std::array<uint8_t, 16> &&p_value) : m_value(std::move(p_value)) {}
  uuid(std::initializer_list<uint8_t> &&p_values) {
    uint8_t _counter{0};

    for (uint8_t _value : p_values) {
      if (_counter > 15) {
        return;
      }
      m_value[_counter++] = _value;
    }
  }

  uuid &operator=(const uuid &) = default;
  uuid &operator=(uuid &) = default;

  bool operator==(const uuid &p_uuid) const {
    return m_value == p_uuid.m_value;
  }
  bool operator!=(const uuid &p_uuid) const {
    return m_value != p_uuid.m_value;
  }
  bool operator<(const uuid &p_uuid) const { return m_value < p_uuid.m_value; }
  bool operator<=(const uuid &p_uuid) const {
    return m_value <= p_uuid.m_value;
  }
  bool operator>(const uuid &p_uuid) const { return m_value < p_uuid.m_value; }
  bool operator>=(const uuid &p_uuid) const {
    return m_value <= p_uuid.m_value;
  }

  friend std::ostream &operator<<(std::ostream &p_out, const uuid &p_uuid) {
    for (value ::value_type _v : p_uuid.m_value) {
      p_out << std::hex << std::uppercase << static_cast<uint16_t>(_v);
    }
    return p_out;
  }

private:
  using value = std::array<uint8_t, 16>;

private:
  value m_value;
};

} // namespace tenacitas::lib::dat

namespace tld = tenacitas::lib::dat;

#endif

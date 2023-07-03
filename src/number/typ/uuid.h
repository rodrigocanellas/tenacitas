#ifndef TENACITAS_LIB_NUMBER_TYP_UUID_H
#define TENACITAS_LIB_NUMBER_TYP_UUID_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <array>
#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <iostream>

/*
regex to identify a UUID like 3134dd5d-14fc-44b1-9f9b-d71da2b043db:

^([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})-([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})

regex to transform it into
{0x31,0x34,0xdd,0x5d,0x14,0xfc,0x44,0xb1,0x9f,0x9b,0xd7,0x1d,0xa2,0xdb}:

{0x$1,0x$2,0x$3,0x$4,0x$5,0x$6,0x$7,0x$8,0x$9,0x$10,0x$11,0x$12,0x$13,0x$14,0x$15,0x$16}

*/

namespace tenacitas::lib::number::typ {

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

} // namespace tenacitas::lib::number::typ

#endif // UUID_H

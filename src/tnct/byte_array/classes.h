/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_BYTE_ARRAY_CLASSES_H
#define TNCT_BYTE_ARRAY_CLASSES_H

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

namespace tnct::byte_array {
template <std::unsigned_integral TNum>
std::array<std::uint8_t, sizeof(TNum)> to_little(const TNum num);

template <>
std::array<std::uint8_t, sizeof(std::uint16_t)>
to_little(const std::uint16_t num) {
  return {static_cast<std::uint8_t>(num & 0xFF),
          static_cast<std::uint8_t>((num >> 8) & 0xFF)};
}

template <>
std::array<std::uint8_t, sizeof(std::uint32_t)>
to_little(const std::uint32_t num) {
  return {static_cast<std::uint8_t>(num & 0xFF),
          static_cast<std::uint8_t>((num >> 8) & 0xFF),
          static_cast<std::uint8_t>((num >> 16) & 0xFF),
          static_cast<std::uint8_t>((num >> 24) & 0xFF)};
}

template <>
std::array<std::uint8_t, sizeof(std::uint64_t)>
to_little(const std::uint64_t num) {
  return {static_cast<std::uint8_t>(num & 0xFF),
          static_cast<std::uint8_t>((num >> 8) & 0xFF),
          static_cast<std::uint8_t>((num >> 16) & 0xFF),
          static_cast<std::uint8_t>((num >> 24) & 0xFF),
          static_cast<std::uint8_t>((num >> 32) & 0xFF),
          static_cast<std::uint8_t>((num >> 40) & 0xFF),
          static_cast<std::uint8_t>((num >> 48) & 0xFF),
          static_cast<std::uint8_t>((num >> 56) & 0xFF)};
}

template <std::unsigned_integral TNum>
TNum from_little(const std::array<std::uint8_t, sizeof(TNum)> &);

template <>
std::uint16_t
from_little(const std::array<std::uint8_t, sizeof(std::uint16_t)> &array) {
  return static_cast<std::uint16_t>(array[0]) |
         (static_cast<std::uint16_t>(array[1]) << 8);
}

template <>
std::uint32_t
from_little(const std::array<std::uint8_t, sizeof(std::uint32_t)> &array) {
  return (static_cast<std::uint32_t>(array[3]) << 24) |
         (static_cast<std::uint32_t>(array[2]) << 16) |
         (static_cast<std::uint32_t>(array[1]) << 8) |
         (static_cast<std::uint32_t>(array[0]));
}

template <>
std::uint64_t
from_little(const std::array<std::uint8_t, sizeof(std::uint64_t)> &array) {
  return (static_cast<std::uint64_t>(array[7]) << 56) |
         (static_cast<std::uint64_t>(array[6]) << 48) |
         (static_cast<std::uint64_t>(array[5]) << 40) |
         (static_cast<std::uint64_t>(array[4]) << 32) |
         (static_cast<std::uint64_t>(array[3]) << 24) |
         (static_cast<std::uint64_t>(array[2]) << 16) |
         (static_cast<std::uint64_t>(array[1]) << 8) |
         static_cast<std::uint64_t>(array[0]);
}

template <typename TNum>
requires(std::integral<TNum> || std::floating_point<TNum>) TNum
    from_little(const std::uint8_t *);

template <> std::uint8_t from_little<std::uint8_t>(const std::uint8_t *array) {
  return static_cast<std::uint8_t>(array[0]);
}

template <>
std::uint16_t from_little<std::uint16_t>(const std::uint8_t *array) {
  return static_cast<std::uint16_t>(array[0]) |
         (static_cast<std::uint16_t>(array[1]) << 8);
}

template <>
std::uint32_t from_little<std::uint32_t>(const std::uint8_t *array) {
  return (static_cast<std::uint32_t>(array[3]) << 24) |
         (static_cast<std::uint32_t>(array[2]) << 16) |
         (static_cast<std::uint32_t>(array[1]) << 8) |
         (static_cast<std::uint32_t>(array[0]));
}

template <>
std::uint64_t from_little<std::uint64_t>(const std::uint8_t *array) {
  return (static_cast<std::uint64_t>(array[7]) << 56) |
         (static_cast<std::uint64_t>(array[6]) << 48) |
         (static_cast<std::uint64_t>(array[5]) << 40) |
         (static_cast<std::uint64_t>(array[4]) << 32) |
         (static_cast<std::uint64_t>(array[3]) << 24) |
         (static_cast<std::uint64_t>(array[2]) << 16) |
         (static_cast<std::uint64_t>(array[1]) << 8) |
         static_cast<std::uint64_t>(array[0]);
}

template <std::integral TNum>
std::string to_str(const std::array<std::uint8_t, sizeof(TNum)> &array) {
  std::stringstream stream;
  for (size_t i = 0; i < array.size(); ++i) {
    stream << "0x" << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(array[i]) << ' ';
  }
  return stream.str();
}

template <> std::int8_t from_little<std::int8_t>(const std::uint8_t *array) {
  return static_cast<int8_t>(static_cast<std::uint8_t>(array[0]));
}

template <> std::int16_t from_little<std::int16_t>(const std::uint8_t *array) {
  return static_cast<std::int16_t>(static_cast<std::uint16_t>(array[0]) |
                                   (static_cast<std::uint16_t>(array[1]) << 8));
}

template <> std::int32_t from_little<std::int32_t>(const std::uint8_t *array) {
  return static_cast<std::int32_t>(
      (static_cast<std::uint32_t>(array[3]) << 24) |
      (static_cast<std::uint32_t>(array[2]) << 16) |
      (static_cast<std::uint32_t>(array[1]) << 8) |
      (static_cast<std::uint32_t>(array[0])));
}

template <> std::int64_t from_little<std::int64_t>(const std::uint8_t *array) {
  return static_cast<std::int64_t>(
      (static_cast<std::uint64_t>(array[7]) << 56) |
      (static_cast<std::uint64_t>(array[6]) << 48) |
      (static_cast<std::uint64_t>(array[5]) << 40) |
      (static_cast<std::uint64_t>(array[4]) << 32) |
      (static_cast<std::uint64_t>(array[3]) << 24) |
      (static_cast<std::uint64_t>(array[2]) << 16) |
      (static_cast<std::uint64_t>(array[1]) << 8) |
      static_cast<std::uint64_t>(array[0]));
}

template <> double from_little<double>(const std::uint8_t *array) {
  std::uint64_t tmp{from_little<std::uint64_t>(array)};
  double value{std::numeric_limits<double>::max()};
  std::memcpy(&value, &tmp, sizeof(double));
  return value;
}

} // namespace tnct::byte_array

#endif

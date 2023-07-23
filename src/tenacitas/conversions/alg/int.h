#ifndef TENACITAS_LIB_CONVERSIONS_ALG_INT_H
#define TENACITAS_LIB_CONVERSIONS_ALG_INT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstring>
#include <optional>
#include <string>

#include <tenacitas/conversions/alg/internal/internal.h>
#include <tenacitas/conversions/typ/base.h>

namespace tenacitas::lib::conversions::alg {

template <typename t_to, typ::base t_base = typ::base::b10>
constexpr std::pair<std::unique_ptr<std::string>, t_to> cvt(const char *p_begin,
                                                            const char *p_end) {
  if constexpr (std::is_integral_v<t_to>) {
    if constexpr (std::is_floating_point_v<t_to>) {
      return {std::make_unique<std::string>(std::string{p_begin, p_end} +
                                            "' is not a integer number"),
              {}};
    }
    return internal::from_decimal<t_to, t_base>()(p_begin, p_end);
  }
  return {std::make_unique<std::string>("undefined conversion"), {}};
}

template <typename t_to, typ::base t_base = typ::base::b10>
constexpr std::pair<std::unique_ptr<std::string>, t_to>
cvt(const char *p_from) {
  return cvt<t_to, t_base>(p_from, p_from + strlen(p_from));
}

template <typename t_to, typename t_size, typ::base t_base = typ::base::b10>
constexpr std::pair<std::unique_ptr<std::string>, t_to> cvt(const char *p_from,
                                                            t_size p_size) {
  return cvt<t_to, t_base>(p_from, p_from + p_size);
}

template <typename t_to, typ::base t_base = typ::base::b10>
constexpr std::pair<std::unique_ptr<std::string>, t_to>
cvt(const std::string &p_from) {
  return cvt<t_to, t_base>(&p_from[0], &p_from[p_from.size()]);
}

template <typename t_to, typ::base t_base = typ::base::b10>
constexpr std::pair<std::unique_ptr<std::string>, t_to>
cvt(std::string &&p_from) {
  return cvt<t_to, t_base>(&p_from[0], &p_from[p_from.size()]);
}

template <typename t_to, typ::base t_base = typ::base::b10>
constexpr std::pair<std::unique_ptr<std::string>, t_to>
cvt(std::string_view p_from) {
  return cvt<t_to, t_base>(&p_from[0], &p_from[p_from.size()]);
}

} // namespace tenacitas::lib::conversions::alg

//// template <> inline uint8_t cvt(const char *p_from) {
////   try {
////     return static_cast<uint8_t>(atoi(p_from));
////   } catch (...) {
////     return {};
////   }
//// }

//// template <> inline int8_t cvt(const char *p_from) {
////   return static_cast<int8_t>(atoi(p_from));
//// }

//// template <> inline int16_t cvt(const char *p_from) {
////   return static_cast<uint16_t>(atoi(p_from));
//// }

//// template <> inline uint16_t cvt(const char *p_from) {
////   return static_cast<uint16_t>(atoi(p_from));
//// }

//// template <> inline int32_t cvt(const char *p_from) {
////   return static_cast<uint32_t>(atol(p_from));
//// }

//// template <> inline uint32_t cvt(const char *p_from) {
////   return static_cast<uint32_t>(atol(p_from));
//// }

//// template <> inline int64_t cvt(const char *p_from) {
////   return static_cast<uint64_t>(atoll(p_from));
//// }

//// template <> inline uint64_t cvt(const char *p_from) {
////   return static_cast<uint64_t>(atoll(p_from));
//// }

//// template <> inline std::p_string cvt(const char *p_from) {
////   return std::p_string{p_from};
//// }

//// template <> inline float cvt(const char *p_from) {
////   return static_cast<float>(cvt<int32_t>(p_from));
//// }

//// template <> inline double cvt(const char *p_from) {
////   return static_cast<double>(cvt<int32_t>(p_from));
//// }

////// from const std::p_string &

//// template <> inline uint8_t cvt(const std::p_string &p_from) {
////   return static_cast<uint8_t>(std::stoi(p_from));
//// }

//// template <> inline int8_t cvt(const std::p_string &p_from) {
////   return static_cast<int8_t>(std::stoi(p_from));
//// }

//// template <> inline int16_t cvt(const std::p_string &p_from) {
////   return static_cast<uint16_t>(std::stoi(p_from));
//// }

//// template <> inline uint16_t cvt(const std::p_string &p_from) {
////   return static_cast<uint16_t>(std::stoi(p_from));
//// }

//// template <> inline int32_t cvt(const std::p_string &p_from) {
////   return static_cast<uint32_t>(std::stol(p_from));
//// }

//// template <> inline uint32_t cvt(const std::p_string &p_from) {
////   return static_cast<uint32_t>(std::stol(p_from));
//// }

//// template <> inline int64_t cvt(const std::p_string &p_from) {
////   return static_cast<uint64_t>(std::stoll(p_from));
//// }

//// template <> inline uint64_t cvt(const std::p_string &p_from) {
////   return static_cast<uint64_t>(std::stoll(p_from));
//// }

//// template <> inline float cvt(const std::p_string &p_from) {
////   return std::stof(p_from);
//// }

//// template <> inline double cvt(const std::p_string &p_from) {
////   return std::stod(p_from);
//// }

//// template <> inline std::p_string cvt(const std::p_string &p_from) {
////   return std::p_string{p_from};
//// }

////// from std::p_string &&

//// template <> inline uint8_t cvt(std::p_string &&p_from) {
////   return static_cast<uint8_t>(std::stoi(p_from));
//// }

//// template <> inline int8_t cvt(std::p_string &&p_from) {
////   return static_cast<int8_t>(std::stoi(p_from));
//// }

//// template <> inline int16_t cvt(std::p_string &&p_from) {
////   return static_cast<uint16_t>(std::stoi(p_from));
//// }

//// template <> inline uint16_t cvt(std::p_string &&p_from) {
////   return static_cast<uint16_t>(std::stoi(p_from));
//// }

//// template <> inline int32_t cvt(std::p_string &&p_from) {
////   return static_cast<uint32_t>(std::stol(p_from));
//// }

//// template <> inline uint32_t cvt(std::p_string &&p_from) {
////   return static_cast<uint32_t>(std::stol(p_from));
//// }

//// template <> inline int64_t cvt(std::p_string &&p_from) {
////   return static_cast<uint64_t>(std::stoll(p_from));
//// }

//// template <> inline uint64_t cvt(std::p_string &&p_from) {
////   return static_cast<uint64_t>(std::stoll(p_from));
//// }

//// template <> inline float cvt(std::p_string &&p_from) { return
//// std::stof(p_from); }

//// template <> inline double cvt(std::p_string &&p_from) {
////   return std::stod(p_from);
//// }

//// template <> inline std::p_string cvt(std::p_string &&p_from) {
////   return std::p_string{p_from};
//// }

////// from std::p_string

//// template <> inline uint8_t cvt(std::p_string p_from) {
////   return static_cast<uint8_t>(std::stoi(p_from));
//// }

//// template <> inline int8_t cvt(std::p_string p_from) {
////   return static_cast<int8_t>(std::stoi(p_from));
//// }

//// template <> inline int16_t cvt(std::p_string p_from) {
////   return static_cast<uint16_t>(std::stoi(p_from));
//// }

//// template <> inline uint16_t cvt(std::p_string p_from) {
////   return static_cast<uint16_t>(std::stoi(p_from));
//// }

//// template <> inline int32_t cvt(std::p_string p_from) {
////   return static_cast<uint32_t>(std::stol(p_from));
//// }

//// template <> inline uint32_t cvt(std::p_string p_from) {
////   return static_cast<uint32_t>(std::stol(p_from));
//// }

//// template <> inline int64_t cvt(std::p_string p_from) {
////   return static_cast<uint64_t>(std::stoll(p_from));
//// }

//// template <> inline uint64_t cvt(std::p_string p_from) {
////   return static_cast<uint64_t>(std::stoll(p_from));
//// }

//// template <> inline float cvt(std::p_string p_from) { return
//// std::stof(p_from);
//// }

//// template <> inline double cvt(std::p_string p_from) { return
//// std::stod(p_from);
//// }

//// template <> inline std::p_string cvt(std::p_string p_from) {
////   return std::p_string{p_from};
//// }

////// from unsigned char *

//// template <> inline uint8_t cvt(const unsigned char *const p_from) {
////   return static_cast<uint8_t>(atoi(reinterpret_cast<const char
///*>(p_from))); / }

//// template <> inline int8_t cvt(const unsigned char *const p_from) {
////   return static_cast<int8_t>(atoi(reinterpret_cast<const char *>(p_from)));
//// }

//// template <> inline int16_t cvt(const unsigned char *const p_from) {
////   return static_cast<uint16_t>(atoi(reinterpret_cast<const char
///*>(p_from))); / }

//// template <> inline uint16_t cvt(const unsigned char *const p_from) {
////   return static_cast<uint16_t>(atoi(reinterpret_cast<const char
///*>(p_from))); / }

//// template <> inline int32_t cvt(const unsigned char *const p_from) {
////   return static_cast<uint32_t>(atol(reinterpret_cast<const char
///*>(p_from))); / }

//// template <> inline uint32_t cvt(const unsigned char *const p_from) {
////   return static_cast<uint32_t>(atol(reinterpret_cast<const char
///*>(p_from))); / }

//// template <> inline int64_t cvt(const unsigned char *const p_from) {
////   return static_cast<uint64_t>(atoll(reinterpret_cast<const char
////   *>(p_from)));
//// }

//// template <> inline uint64_t cvt(const unsigned char *const p_from) {
////   return static_cast<uint64_t>(atoll(reinterpret_cast<const char
////   *>(p_from)));
//// }

//// template <> inline float cvt(const unsigned char *const p_from) {
////   return static_cast<float>(atof(reinterpret_cast<const char *>(p_from)));
//// }

//// template <> inline double cvt(const unsigned char *const p_from) {
////   return static_cast<double>(atof(reinterpret_cast<const char *>(p_from)));
//// }

//// template <> inline std::p_string cvt(const unsigned char *const p_from) {
////   return std::p_string{reinterpret_cast<const char *>(p_from)};
//// }

////// from uint64_t

//// template <> inline std::p_string cvt(uint64_t p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline uint64_t cvt(uint64_t p_from) { return p_from; }

//// template <> inline int64_t cvt(uint64_t p_from) {
////   return static_cast<int64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(uint64_t p_from) {
////   return static_cast<uint32_t>(p_from);
//// }

//// template <> inline int32_t cvt(uint64_t p_from) {
////   return static_cast<int32_t>(p_from);
//// }

//// template <> inline uint16_t cvt(uint64_t p_from) {
////   return static_cast<uint16_t>(p_from);
//// }

//// template <> inline int16_t cvt(uint64_t p_from) {
////   return static_cast<int16_t>(p_from);
//// }

//// template <> inline uint8_t cvt(uint64_t p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(uint64_t p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(uint64_t p_from) {
////   return static_cast<float>(p_from);
//// }

//// template <> inline double cvt(uint64_t p_from) {
////   return static_cast<double>(p_from);
//// }

//// template <> inline std::array<std::byte, 8> cvt(uint64_t p_from) {
////   std::array<std::byte, 8> _res;

////  _res[7] = static_cast<std::byte>(p_from >> (8 * 0));
////  _res[6] = static_cast<std::byte>(p_from >> (8 * 1));
////  _res[5] = static_cast<std::byte>(p_from >> (8 * 2));
////  _res[4] = static_cast<std::byte>(p_from >> (8 * 3));
////  _res[3] = static_cast<std::byte>(p_from >> (8 * 4));
////  _res[2] = static_cast<std::byte>(p_from >> (8 * 5));
////  _res[1] = static_cast<std::byte>(p_from >> (8 * 6));
////  _res[0] = static_cast<std::byte>(p_from >> (8 * 7));

////  return _res;
////}

//// template <typename t_type = std::byte> void cvt(uint64_t p_from, t_type
//// *p_to) {
////   *p_to = static_cast<t_type>(p_from >> (8 * 7));
////   *(p_to + sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 6));
////   *(p_to + 2 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 5));
////   *(p_to + 3 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 4));
////   *(p_to + 4 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 3));
////   *(p_to + 5 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 2));
////   *(p_to + 6 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 1));
////   *(p_to + 7 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 0));
//// }

//// template <> inline uint64_t cvt(std::byte *p_from) {
////   uint64_t _res{static_cast<uint8_t>(p_from[0])};
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[2]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[3]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[4]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[5]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[6]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[7]);
////   return _res;
//// }

//// template <> inline uint64_t cvt(std::array<std::byte, 8> p_from) {
////   uint64_t _res{static_cast<uint8_t>(p_from[0])};
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[2]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[3]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[4]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[5]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[6]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[7]);
////   return _res;
//// }

//// template <> inline uint64_t cvt(const std::array<std::byte, 8> &p_from) {
////   uint64_t _res{static_cast<uint8_t>(p_from[0])};
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[2]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[3]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[4]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[5]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[6]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[7]);
////   return _res;
//// }

////// from int64_t
//// template <> inline std::p_string cvt(int64_t p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline int64_t cvt(int64_t p_from) { return p_from; }

//// template <> inline uint64_t cvt(int64_t p_from) {
////   return static_cast<uint64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(int64_t p_from) {
////   return static_cast<uint32_t>(p_from);
//// }

//// template <> inline int32_t cvt(int64_t p_from) {
////   return static_cast<int32_t>(p_from);
//// }

//// template <> inline uint16_t cvt(int64_t p_from) {
////   return static_cast<uint16_t>(p_from);
//// }

//// template <> inline int16_t cvt(int64_t p_from) {
////   return static_cast<int16_t>(p_from);
//// }

//// template <> inline uint8_t cvt(int64_t p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(int64_t p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(int64_t p_from) {
////   return static_cast<float>(p_from);
//// }

//// template <> inline double cvt(int64_t p_from) {
////   return static_cast<double>(p_from);
//// }

////// from uint32_t

//// template <> inline std::p_string cvt(uint32_t p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline uint64_t cvt(uint32_t p_from) {
////   return static_cast<uint64_t>(p_from);
//// }

//// template <> inline int64_t cvt(uint32_t p_from) {
////   return static_cast<int64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(uint32_t p_from) { return p_from; }

//// template <> inline int32_t cvt(uint32_t p_from) {
////   return static_cast<int32_t>(p_from);
//// }

//// template <> inline uint16_t cvt(uint32_t p_from) {
////   return static_cast<uint16_t>(p_from);
//// }

//// template <> inline int16_t cvt(uint32_t p_from) {
////   return static_cast<int16_t>(p_from);
//// }

//// template <> inline uint8_t cvt(uint32_t p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(uint32_t p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(uint32_t p_from) {
////   return static_cast<float>(p_from);
//// }

//// template <> inline double cvt(uint32_t p_from) {
////   return static_cast<double>(p_from);
//// }

//// template <typename t_type = std::byte> void cvt(uint32_t p_from, t_type
//// *p_to) {
////   *p_to = static_cast<t_type>(p_from >> (8 * 3));
////   *(p_to + sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 2));
////   *(p_to + 2 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 1));
////   *(p_to + 3 * sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 0));
//// }

//// template <> inline uint32_t cvt(std::byte *p_from) {
////   uint32_t _res{static_cast<uint8_t>(p_from[0])};
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[2]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[3]);
////   return _res;
//// }

//// template <> inline std::array<std::byte, 4> cvt(uint32_t p_from) {
////   std::array<std::byte, 4> _res;

////  _res[3] = static_cast<std::byte>(p_from >> (8 * 0));
////  _res[2] = static_cast<std::byte>(p_from >> (8 * 1));
////  _res[1] = static_cast<std::byte>(p_from >> (8 * 2));
////  _res[0] = static_cast<std::byte>(p_from >> (8 * 3));

////  return _res;
////}
//// template <> inline uint32_t cvt(std::array<std::byte, 4> p_from) {
////  uint32_t _res{static_cast<uint8_t>(p_from[0])};
////  _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////  _res = (_res << 8) | static_cast<uint8_t>(p_from[2]);
////  _res = (_res << 8) | static_cast<uint8_t>(p_from[3]);
////  return _res;
////}

//// template <> inline uint32_t cvt(const std::array<std::byte, 4> &p_from) {
////   uint32_t _res{static_cast<uint8_t>(p_from[0])};
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[2]);
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[3]);
////   return _res;
//// }

////// from int32_t

//// template <> inline std::p_string cvt(int32_t p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline uint64_t cvt(int32_t p_from) {
////   return static_cast<uint64_t>(p_from);
//// }

//// template <> inline int64_t cvt(int32_t p_from) {
////   return static_cast<int64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(int32_t p_from) {
////   return static_cast<uint32_t>(p_from);
//// }

//// template <> inline int32_t cvt(int32_t p_from) { return p_from; }

//// template <> inline uint16_t cvt(int32_t p_from) {
////   return static_cast<uint16_t>(p_from);
//// }

//// template <> inline int16_t cvt(int32_t p_from) {
////   return static_cast<int16_t>(p_from);
//// }

//// template <> inline uint8_t cvt(int32_t p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(int32_t p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(int32_t p_from) {
////   return static_cast<float>(p_from);
//// }

//// template <> inline double cvt(int32_t p_from) {
////   return static_cast<double>(p_from);
//// }

////// from uint16_t

//// template <> inline std::p_string cvt(uint16_t p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline uint64_t cvt(uint16_t p_from) {
////   return static_cast<uint64_t>(p_from);
//// }

//// template <> inline int64_t cvt(uint16_t p_from) {
////   return static_cast<int64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(uint16_t p_from) {
////   return static_cast<uint32_t>(p_from);
//// }

//// template <> inline int32_t cvt(uint16_t p_from) {
////   return static_cast<int32_t>(p_from);
//// }

//// template <> inline uint16_t cvt(uint16_t p_from) { return p_from; }

//// template <> inline int16_t cvt(uint16_t p_from) {
////   return static_cast<int16_t>(p_from);
//// }

//// template <> inline uint8_t cvt(uint16_t p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(uint16_t p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(uint16_t p_from) {
////   return static_cast<float>(p_from);
//// }

//// template <> inline double cvt(uint16_t p_from) {
////   return static_cast<double>(p_from);
//// }

//// template <typename t_type = std::byte> void cvt(uint16_t p_from, t_type
//// *p_to) {
////   *p_to = static_cast<t_type>(p_from >> (8 * 1));
////   *(p_to + sizeof(t_type)) = static_cast<t_type>(p_from >> (8 * 0));
//// }

//// template <> inline uint16_t cvt(const std::byte *p_from) {
////   uint16_t _res{static_cast<uint8_t>(p_from[0])};
////   _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////   return _res;
//// }

//// template <> inline std::array<std::byte, 2> cvt(uint16_t p_from) {
////   std::array<std::byte, 2> _res;

////  _res[1] = static_cast<std::byte>(p_from >> (8 * 0));
////  _res[0] = static_cast<std::byte>(p_from >> (8 * 1));

////  return _res;
////}
//// template <> inline uint16_t cvt(std::array<std::byte, 2> p_from) {
////  uint16_t _res{static_cast<uint8_t>(p_from[0])};
////  _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////  return _res;
////}

//// template <> inline uint16_t cvt(const std::array<std::byte, 2> &p_from) {

////  uint16_t _res{static_cast<uint8_t>(p_from[0])};
////  _res = (_res << 8) | static_cast<uint8_t>(p_from[1]);
////  return _res;
////}

////// from int16_t

//// template <> inline std::p_string cvt(int16_t p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline uint64_t cvt(int16_t p_from) {
////   return static_cast<uint64_t>(p_from);
//// }

//// template <> inline int64_t cvt(int16_t p_from) {
////   return static_cast<int64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(int16_t p_from) {
////   return static_cast<uint32_t>(p_from);
//// }

//// template <> inline int32_t cvt(int16_t p_from) {
////   return static_cast<int32_t>(p_from);
//// }

//// template <> inline uint16_t cvt(int16_t p_from) {
////   return static_cast<uint16_t>(p_from);
//// }

//// template <> inline int16_t cvt(int16_t p_from) { return p_from; }

//// template <> inline uint8_t cvt(int16_t p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(int16_t p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(int16_t p_from) {
////   return static_cast<float>(p_from);
//// }

//// template <> inline double cvt(int16_t p_from) {
////   return static_cast<double>(p_from);
//// }

////// from uint8_t

//// template <typename t_type = std::byte> void cvt(uint8_t p_from, t_type
///*p_to) / { /   *p_to = static_cast<t_type>(p_from); / }

////// from float

//// template <> inline std::p_string cvt(float p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline uint64_t cvt(float p_from) {
////   return static_cast<uint64_t>(p_from);
//// }

//// template <> inline int64_t cvt(float p_from) {
////   return static_cast<int64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(float p_from) {
////   return static_cast<uint32_t>(p_from);
//// }

//// template <> inline int32_t cvt(float p_from) {
////   return static_cast<int32_t>(p_from);
//// }

//// template <> inline uint16_t cvt(float p_from) {
////   return static_cast<uint16_t>(p_from);
//// }

//// template <> inline int16_t cvt(float p_from) {
////   return static_cast<int16_t>(p_from);
//// }

//// template <> inline uint8_t cvt(float p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(float p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(float p_from) { return p_from; }

//// template <> inline double cvt(float p_from) {
////   return static_cast<double>(p_from);
//// }

////// from double

//// template <> inline std::p_string cvt(double p_from) {
////   return std::to_p_string(p_from);
//// }

//// template <> inline uint64_t cvt(double p_from) {
////   return static_cast<uint64_t>(p_from);
//// }

//// template <> inline int64_t cvt(double p_from) {
////   return static_cast<int64_t>(p_from);
//// }

//// template <> inline uint32_t cvt(double p_from) {
////   return static_cast<uint32_t>(p_from);
//// }

//// template <> inline int32_t cvt(double p_from) {
////   return static_cast<int32_t>(p_from);
//// }

//// template <> inline uint16_t cvt(double p_from) {
////   return static_cast<uint16_t>(p_from);
//// }

//// template <> inline int16_t cvt(double p_from) {
////   return static_cast<int16_t>(p_from);
//// }

//// template <> inline uint8_t cvt(double p_from) {
////   return static_cast<uint8_t>(p_from);
//// }

//// template <> inline int8_t cvt(double p_from) {
////   return static_cast<int8_t>(p_from);
//// }

//// template <> inline float cvt(double p_from) {
////   return static_cast<float>(p_from);
//// }

//// template <> inline double cvt(double p_from) {
////   return static_cast<double>(p_from);
//// }

////// from void

//// inline std::p_string cvt(const void *const p_from, size_t p_size) {
////   if (p_from) {
////     const char *_begin{reinterpret_cast<const char *>(p_from)};
////     const char *_end{reinterpret_cast<const char *>(_begin + p_size)};
////     return std::p_string(_begin, _end);
////   }

////  return "";
////}

//// template <> inline int64_t cvt(const void *const p_from) {
////   int64_t _i{0};
////   memcpy(&_i, p_from, sizeof(int64_t));
////   return _i;
//// }

//// template <> inline uint64_t cvt(const void *const p_from) {
////   uint64_t _i{0};
////   if (p_from) {
////     memcpy(&_i, p_from, sizeof(uint64_t));
////   }
////   return _i;
//// }

//// template <> inline int32_t cvt(const void *const p_from) {
////   int32_t _i{0};
////   if (p_from) {
////     memcpy(&_i, p_from, sizeof(int32_t));
////   }
////   return _i;
//// }

//// template <> inline uint32_t cvt(const void *const p_from) {
////   uint32_t _i{0};
////   if (p_from) {
////     memcpy(&_i, p_from, sizeof(uint32_t));
////   }
////   return _i;
//// }

//// template <> inline int16_t cvt(const void *const p_from) {
////   int16_t _i{0};
////   if (p_from) {
////     memcpy(&_i, p_from, sizeof(int16_t));
////   }
////   return _i;
//// }

//// template <> inline uint16_t cvt(const void *const p_from) {
////   uint16_t _i{0};
////   if (p_from) {
////     memcpy(&_i, p_from, sizeof(uint16_t));
////   }
////   return _i;
//// }

//// template <> inline int8_t cvt(const void *const p_from) {
////   int8_t _i{0};
////   if (p_from) {
////     memcpy(&_i, p_from, sizeof(int8_t));
////   }
////   return _i;
//// }

//// template <> inline uint8_t cvt(const void *const p_from) {
////   uint8_t _i{0};
////   if (p_from) {
////     memcpy(&_i, p_from, sizeof(int8_t));
////   }
////   return _i;
//// }

//// template <> inline float cvt(const void *const p_from) {
////   float _f{0.0};
////   if (p_from) {
////     memcpy(&_f, p_from, sizeof(float));
////   }
////   return _f;
//// }

//// template <> inline double cvt(const void *const p_from) {
////   double _d{0.0};
////   if (p_from) {
////     memcpy(&_d, p_from, sizeof(double));
////   }
////   return _d;
//// }

//// template <typename t_type> void visit(t_type &&);

/////// \brief Traverses a tuple
//// template <size_t index = 0, typename t_visit, typename... t_fields>
//// void traverse(std::tuple<t_fields...> &p_tuple, t_visit &p_visit) {
////   p_visit(&(std::get<index>(p_tuple)));

////  if constexpr (index + 1 != sizeof...(t_fields)) {
////    traverse<index + 1>(p_tuple, p_visit);
////  }
////}

/////// \brief Converts a container into a tuple
///////
/////// \tparam index is the current element of the tuple being analysed
///////
/////// \tparam t_iterator is a iterator type for the container
///////
/////// \tparam t_fields are the types of the elements of the tuple
///////
/////// \param p_tuple is the tuple that will contain the converted values of
/// the
/////// container
///////
/////// \param p_ite is a iterator to the container that contains the values to
/// be
/////// converted
///////
/////// \attention It is necessary to implement a \p cvt function from the
/////// element type of the container to each element type of the tuple, like:
/////// \code
/////*
////#include <cstdint>
////#include <iop_stream>
////#include <iterator>
////#include <list>
////#include <memory>
////#include <sp_stream>
////#include <p_string>
////#include <tuple>
////#include <vector>

////#include <tenacitas/traits/alg/traits.h>

//// using namespace tenacitas::lib::src;

//// p_struct xpto {
////     xpto(const char *s) : p_str(s) {}
////     xpto() { std::cout << "xpto conp_structor\n"; }
////     xpto(const xpto &p_s) : p_str(p_s.p_str) {
////         std::cout << "xpto copy conp_structor\n";
////     }
////     xpto(xpto &&p_s) : p_str(move(p_s.p_str)) {
////         std::cout << "xpto move conp_structor\n";
////     }
////     xpto &operator=(const xpto &p_s) {
////         if (this != &p_s) {
////             p_str = p_s.p_str;
////             std::cout << "xpto copy \n";
////         }
////         return *this;
////     }
////     xpto &operator=(xpto &&p_s) {
////         if (this != &p_s) {
////             p_str = std::move(p_s.p_str);
////             std::cout << "xpto move \n";
////         }
////         return *this;
////     }
////     std::p_string p_str;
//// };

//// template <> uint32_t type_traits::cvt<uint32_t>(const xpto *const p_from) {
////     std::cout << "01\n";
////     return std::stoul(std::move(p_from->p_str));
//// }

//// template <> std::p_string type_traits::cvt<std::p_string>(const xpto *const
////                                                   p_from) { std::cout <<
////                                                   "02\n";
//// return p_from->p_str;
//// }

//// template <> float type_traits::cvt<float>(const xpto *const p_from) {
////     std::cout << "03\n";
////     return std::stof(std::move(p_from->p_str));
//// }

//// int main() {
////     {
////         std::cout << "\n\nmain0\n";
////         std::vector<xpto> _value{xpto{"1"}, xpto{"hello"}, xpto{"3.14"}};
////         std::tuple<uint32_t, std::p_string, float> _tuple;

////        std::vector<xpto>::const_iterator _begin{_value.begin()};
////        type_traits::cvt(_tuple, _begin);

////        std::apply([](auto &&... args) { ((std::cout << args << ' '), ...);
///}, /                   _tuple);

////        std::cout << '\n';
////    }
////    {
////        using vector = std::vector<std::shared_ptr<xpto>>;
////        auto _x0{std::make_shared<xpto>("1")};
////        auto _x1{std::make_shared<xpto>("hello")};
////        auto _x2{std::make_shared<xpto>("3.14")};
////        vector _value{_x0, _x1, _x2};
////        std::cout << "\n\nmain1\n";
////        std::tuple<uint32_t, std::p_string, float> _tuple;

////        vector::const_iterator _begin{_value.begin()};
////        type_traits::cvt(_tuple, _begin);

////        std::apply([](auto &&... args) { ((std::cout << args << ' '), ...);
///}, /                   _tuple); /    }

////    {
////        using vector = std::vector<std::unique_ptr<xpto>>;

////        vector _value;
////        _value.push_back(std::make_unique<xpto>("1"));
////        _value.push_back(std::make_unique<xpto>("hello"));
////        _value.push_back(std::make_unique<xpto>("3.14"));
////        std::cout << "\n\nmain2\n";
////        std::tuple<uint32_t, std::p_string, float> _tuple;

////        vector::const_iterator _begin{_value.begin()};
////        type_traits::cvt(_tuple, _begin);

////        std::apply([](auto &&... args) { ((std::cout << args << ' '), ...);
///}, /                   _tuple); /    }

////    {
////        using vector = std::vector<xpto *>;

////        vector _value;
////        _value.push_back(new xpto("1"));
////        _value.push_back(new xpto("hello"));
////        _value.push_back(new xpto("3.14"));
////        std::cout << "\n\nmain3\n";
////        std::tuple<uint32_t, std::p_string, float> _tuple;

////        vector::const_iterator _begin{_value.begin()};
////        type_traits::cvt(_tuple, _begin);

////        std::apply([](auto &&... args) { ((std::cout << args << ' '), ...);
///}, /                   _tuple);

////        for (vector::value_type &_v : _value) {
////            delete _v;
////        }
////    }

////    std::cout << '\n';
////    return 0;
////}

////*/
/////// \endcode
//// template <size_t index = 0, typename t_iterator, typename... t_fields>
//// void cvt(std::tuple<t_fields...> &p_tuple, t_iterator p_ite) {
////   using tuple_element_type =
////       typename std::tuple_element<index, std::tuple<t_fields...>>::type;

////  using iterator_value_type =
////      typename std::iterator_traits<t_iterator>::value_type;
////  if constexpr (lib::type::alg::is_smart_ptr_v<
////                    std::decay_t<iterator_value_type>>) {
////    // p_ite contains a smart (shared or unique) pointer

////    using pointer_type = typename iterator_value_type::element_type;
////    const pointer_type *_ptr = (*p_ite).get();
////    std::get<index>(p_tuple) = cvt<tuple_element_type>(*_ptr);
////    return;
////  }
////  if constexpr (std::is_pointer_v<std::decay_t<iterator_value_type>>) {
////    // p_ite contains a raw pointer

////    using pointer_type =
////        typename std::pointer_traits<iterator_value_type>::element_type;
////    const pointer_type *_ptr = *p_ite;
////    std::get<index>(p_tuple) = cvt<tuple_element_type>(*_ptr);
////    return;
////  }
////  {
////    // p_ite contains an object
////    std::get<index>(p_tuple) = cvt<tuple_element_type>(*p_ite);
////  }

////  if constexpr (index + 1 != sizeof...(t_fields)) {
////    cvt<index + 1>(p_tuple, ++p_ite);
////  }
////}

//// template <typename t_iterator, typename t_type>
//// void cvt(t_iterator p_ite, t_type &p_value) {
////   using iterator_value_type =
////       typename std::iterator_traits<t_iterator>::value_type;
////   if constexpr (lib::type::alg::is_smart_ptr_v<
////                     std::decay_t<iterator_value_type>>) {
////     // p_ite contains a smart (shared or unique) pointer

////    using pointer_type = typename iterator_value_type::element_type;
////    const pointer_type *_ptr = (*p_ite).get();
////    p_value = cvt<t_type>(*_ptr);
////    return;
////  }
////  if constexpr (std::is_pointer_v<iterator_value_type>) {
////    // p_ite contains a raw pointer
////    using pointer_type = typename std::pointer_traits<
////        std::decay_t<iterator_value_type>>::element_type;
////    const pointer_type *_ptr = *p_ite;
////    p_value = cvt<t_type>(*_ptr);
////    return;
////  }
////  {
////    // p_ite contains an object
////    // const iterator_value_type *_ptr{&(*p_ite)};
////    p_value = cvt<t_type>(*p_ite);
////  }
////}

//// template <typename t_iterator, typename t_type, typename... t_types>
//// void cvt(t_iterator p_ite, t_type &p_value, t_types &...p_values) {
////   cvt<t_iterator, t_type>(p_ite, p_value);
////   cvt<t_iterator, t_types...>(++p_ite, p_values...);
//// }

////} // namespace tenacitas::lib::conversions::alg
#endif // CONVERT_H

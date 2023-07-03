#ifndef TENACITAS_LIB_CONVERSIONS_ALG_INTERNAL_INTERNAL_H
#define TENACITAS_LIB_CONVERSIONS_ALG_INTERNAL_INTERNAL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <locale>
#include <map>
#include <memory>
#include <utility>

#include <tenacitas.lib/src/conversions/typ/base.h>
#include <tenacitas.lib/src/conversions/typ/result.h>
#include <tenacitas.lib/src/conversions/typ/separator.h>

namespace tenacitas::lib::conversions::alg::internal {

template <typename t_int> struct next_int;

template <> struct next_int<uint8_t> { using type = uint16_t; };

template <> struct next_int<uint16_t> { using type = uint32_t; };

template <> struct next_int<uint32_t> { using type = uint64_t; };

template <> struct next_int<uint64_t> { using type = uint64_t; };

template <> struct next_int<int8_t> { using type = int16_t; };

template <> struct next_int<int16_t> { using type = int32_t; };

template <> struct next_int<int32_t> { using type = int64_t; };

template <> struct next_int<int64_t> { using type = int64_t; };

template <typename t_int> std::string int_name() {

  char _c{typeid(t_int).name()[0]};
  switch (_c) {
  case 'h':
    return "uint8_t";
  case 'a':
    return "int8_t";
  case 't':
    return "uint16_t";
  case 's':
    return "int16_t";
  case 'j':
    return "uint32_t";
  case 'i':
    return "int32_t";
  case 'm':
    return "uint64_t";
  case 'l':
    return "int64_t";
  }
  return "-- undefined int type--";
}

template <typename t_int>
typ::result<t_int> overflows(const char *p_begin, const char *p_end) {
  return {std::make_unique<std::string>(std::string{p_begin, p_end} +
                                        " overflows the maximum value for " +
                                        int_name<t_int>()),
          {}};
}

template <typename t_int>
typ::result<t_int> underflows(const char *p_begin, const char *p_end) {
  return {std::make_unique<std::string>("-" + std::string{p_begin, p_end} +
                                        " underflows the minimum value for " +
                                        int_name<t_int>()),
          {}};
}

template <typename t_to, typ::base t_base> struct from_decimal;

template <typename t_to> struct from_decimal<t_to, typ::base::b16> {

  constexpr typ::result<t_to> operator()(const char *p_begin,
                                         const char *p_end) {

    static_assert(std::is_unsigned_v<t_to>,
                  "can not convert a hexadecimal string to a signed integer");

    using next_int = typename next_int<t_to>::type;

    const char *_p{p_begin};

    if ((*_p == '0') && ((*(_p + 1) == 'x') || (*(_p + 1) == 'X'))) {
      _p += 2;
    }

    if ((*_p == 'x') || (*_p == 'X')) {
      ++_p;
    }

    auto _is_valid = [](const char *p_begin, const char *p_end,
                        const char **p_c) -> std::unique_ptr<std::string> {
      if (std::isdigit(**p_c) && ((**p_c) - '0' < 8)) {
        return nullptr;
      }
      return std::make_unique<std::string>(std::string{**p_c} +
                                           " is not an hexadecimal digit in " +
                                           std::string{p_begin, p_end});
    };

    auto _multiplier = [&](const char *p_begin, const char *p_end,
                           t_to p_to) -> typ::result<t_to> {
      auto _mul{static_cast<next_int>(h2i(p_to)) * 16};
      if (_mul > std::numeric_limits<t_to>::max()) {
        return overflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_mul)};
    };

    auto _incrementer = [&](const char *p_begin, const char *p_end,
                            t_to p_value, char p_c) -> typ::result<t_to> {
      auto _sum{static_cast<next_int>(
          p_value + h2i(static_cast<char>(std::toupper(p_c))))};
      if (_sum > std::numeric_limits<t_to>::max()) {
        return overflows<t_to>(p_begin, p_end);
      }

      return {nullptr, static_cast<t_to>(_sum)};
    };

    return to_integer<t_to>(_p, p_end, _is_valid, _multiplier, _incrementer);
  }

private:
  uint8_t h2i(char p_c) {
    switch (p_c) {
    case 'A':
      return 10;
    case 'B':
      return 11;
    case 'C':
      return 12;
    case 'D':
      return 13;
    case 'E':
      return 14;
    case 'F':
      return 15;
    default:
      return (p_c - '0');
    }
  }
};

template <typename t_to> struct from_decimal<t_to, typ::base::b2> {

  constexpr typ::result<t_to> operator()(const char *p_begin,
                                         const char *p_end) {

    static_assert(std::is_unsigned_v<t_to>,
                  "can not convert a binary string to a signed integer");

    using next_int = typename next_int<t_to>::type;

    const char *_p{p_begin};

    if ((*_p == '0') && (*(_p + 1) == 'b')) {
      _p += 2;
    }

    auto _is_valid = [](const char *p_begin, const char *p_end,
                        const char **p_c) -> std::unique_ptr<std::string> {
      if (std::isdigit(**p_c) && ((**p_c == '1') || ((**p_c == '0')))) {
        return nullptr;
      }
      return std::make_unique<std::string>(std::string{**p_c} +
                                           " is not an binary digit in " +
                                           std::string{p_begin, p_end});
    };

    auto _multiplier = [&](const char *p_begin, const char *p_end,
                           t_to p_to) -> typ::result<t_to> {
      auto _mul{static_cast<next_int>(p_to) * 2};
      if (_mul > std::numeric_limits<t_to>::max()) {
        return overflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_mul)};
    };

    auto _incrementer = [&](const char *p_begin, const char *p_end,
                            t_to p_value, char p_c) -> typ::result<t_to> {
      auto _sum{static_cast<next_int>(p_value + (p_c == '1' ? 1 : 0))};
      if (_sum > std::numeric_limits<t_to>::max()) {
        return overflows<t_to>(p_begin, p_end);
      }

      return {nullptr, static_cast<t_to>(_sum)};
    };

    return to_integer<t_to>(_p, p_end, _is_valid, _multiplier, _incrementer);
  }
};

template <typename t_to> struct from_decimal<t_to, typ::base::b8> {

  constexpr typ::result<t_to> operator()(const char *p_begin,
                                         const char *p_end) {

    static_assert(std::is_unsigned_v<t_to>,
                  "can not convert a octal string to a signed integer");

    //    constexpr bool _unsigned{std::is_unsigned_v<t_to>};

    //    if constexpr (!_unsigned) {
    //      return {std::make_unique<std::string>(std::string{p_begin, p_end} +
    //                                            " can not be converted to " +
    //                                            int_name<t_to>()),
    //              {}};
    //    }

    using next_int = typename next_int<t_to>::type;

    const char *_p{p_begin};

    if (*_p == '0') {
      ++_p;
    }

    auto _is_valid = [](const char *p_begin, const char *p_end,
                        const char **p_c) -> std::unique_ptr<std::string> {
      if (std::isdigit(**p_c) && ((**p_c - '0') < 8)) {
        return nullptr;
      }
      return std::make_unique<std::string>(std::string{**p_c} +
                                           " is not an octal digit in " +
                                           std::string{p_begin, p_end});
    };

    auto _multiplier = [&](const char *p_begin, const char *p_end,
                           t_to p_to) -> typ::result<t_to> {
      auto _mul{static_cast<next_int>(p_to) * 8};
      if (_mul > std::numeric_limits<t_to>::max()) {
        return overflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_mul)};
    };

    auto _incrementer = [&](const char *p_begin, const char *p_end,
                            t_to p_value, char p_c) -> typ::result<t_to> {
      auto _sum{static_cast<next_int>(p_value + (p_c - '0'))};
      if (_sum > std::numeric_limits<t_to>::max()) {
        return overflows<t_to>(p_begin, p_end);
      }

      return {nullptr, static_cast<t_to>(_sum)};
    };

    return to_integer<t_to>(_p, p_end, _is_valid, _multiplier, _incrementer);
  }
};

template <typename t_to> struct from_decimal<t_to, typ::base::b10> {

  constexpr typ::result<t_to> operator()(const char *p_begin,
                                         const char *p_end) {

    bool _negative = false;

    constexpr bool _unsigned{std::is_unsigned_v<t_to>};

    const char *_p{p_begin};

    if (*_p == '+') {
      ++_p;
    } else if (*p_begin == '-') {
      ++_p;
      _negative = true;
    }

    if (_negative && _unsigned) {
      return {std::make_unique<std::string>(
                  std::string{p_begin, p_end} + " is a negative number, but " +
                  internal::int_name<t_to>() + " is a unsigned type"),
              {}};
    }

    if (_unsigned) {
      return unsigned_int(_p, p_end);
    }
    if (_negative && !_unsigned) {
      return signed_negative_int(_p, p_end);
    }
    return signed_positive_int(_p, p_end);
  }

private:
  struct is_valid {
    std::unique_ptr<std::string>
    operator()(const char *p_begin, const char *p_end, const char **p_c) {
      if ((**p_c != typ::thousands) && (!std::isdigit(**p_c))) {
        return std::make_unique<std::string>(
            std::string{**p_c} +
            " is not a thounsand separator or a digit in " +
            std::string{p_begin, p_end});
      }

      if (**p_c == typ::thousands) {
        if (!m_thousands_sep_found) { // first thounsand separator found
          if (m_total_counter_digits > 3) {
            // "9999,"
            return std::make_unique<std::string>(
                std::string{**p_c} + " is a thounsand separator, but " +
                std::to_string(static_cast<uint16_t>(m_total_counter_digits)) +
                " were already found in " + std::string{p_begin, p_end});
          }
          if (m_total_counter_digits == 0) {
            // ","
            return std::make_unique<std::string>(
                std::string{**p_c} +
                " is a thounsand separator, but it is the first character "
                "in " +
                std::string{p_begin, p_end});
          }
          m_thousands_sep_found = true;
          ++(*p_c);
        } else { // _thounsand_sep_found = true
          if (m_counter_digits == 3) {
            m_counter_digits = 0;

            ++(*p_c);
            if ((*p_c) == p_end) { // check for the end of the string
              return std::make_unique<std::string>(
                  std::string{**p_c} +
                  " is a thounsand separator, but it is the last character "
                  "in " +
                  std::string{p_begin, p_end});
            }
          }
        }
      } else {
        if (m_thousands_sep_found) {
          if (m_counter_digits == 3) {
            return std::make_unique<std::string>(
                std::string{**p_c} +
                " is the fourth digit since last thounsand separator " +
                std::string{p_begin, p_end} + " in " +
                std::string{p_begin, p_end});
          }
          ++m_counter_digits;

        } else { // _thounsand_sep_found = false
          ++m_total_counter_digits;
        }
      }
      return nullptr;
    }

  private:
    uint8_t m_total_counter_digits{0};
    uint8_t m_counter_digits{0};
    bool m_thousands_sep_found{false};
  };

private:
  constexpr std::pair<std::unique_ptr<std::string>, t_to>
  signed_positive_int(const char *p_begin, const char *p_end) {
    using next_int = typename next_int<t_to>::type;

    auto _multiplier =
        [&](const char *p_begin, const char *p_end,
            t_to p_to) -> std::pair<std::unique_ptr<std::string>, t_to> {
      auto _mul{static_cast<next_int>(p_to) * 10};
      if ((_mul == 0) || (_mul > std::numeric_limits<t_to>::max())) {
        return overflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_mul)};
    };

    auto _incrementer =
        [&](const char *p_begin, const char *p_end, t_to p_value,
            char p_c) -> std::pair<std::unique_ptr<std::string>, t_to> {
      if (p_c == '0') {
        return {nullptr, static_cast<t_to>(p_value)};
      }
      auto _sum{static_cast<next_int>(p_value + (p_c - '0'))};
      if ((_sum < 0) || (_sum > std::numeric_limits<t_to>::max())) {
        return overflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_sum)};
    };

    is_valid _is_valid;

    return to_integer<t_to>(
        p_begin, p_end,
        [&_is_valid](const char *p_begin, const char *p_end, const char **p_c) {
          return _is_valid(p_begin, p_end, p_c);
        },
        _multiplier, _incrementer);
  }

  constexpr std::pair<std::unique_ptr<std::string>, t_to>
  signed_negative_int(const char *p_begin, const char *p_end) {

    using next_int = typename next_int<t_to>::type;

    auto _multiplier =
        [&](const char *p_begin, const char *p_end,
            t_to p_to) -> std::pair<std::unique_ptr<std::string>, t_to> {
      auto _mul{static_cast<next_int>(p_to) * 10};
      if (_mul < std::numeric_limits<t_to>::min()) {
        return {std::make_unique<std::string>(
                    std::string{p_begin, p_end} +
                    " underflows the minimum value for " + int_name<t_to>()),
                {}};
      }
      return {nullptr, static_cast<t_to>(_mul)};
    };

    auto _incrementer =
        [&](const char *p_begin, const char *p_end, t_to p_value,
            char p_c) -> std::pair<std::unique_ptr<std::string>, t_to> {
      if (p_c == '0') {
        return {nullptr, static_cast<t_to>(p_value)};
      }
      auto _sum{static_cast<next_int>(p_value - (p_c - '0'))};
      if ((_sum > 0) || (_sum < std::numeric_limits<t_to>::min())) {
        return underflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_sum)};
    };

    is_valid _is_valid;

    return to_integer<t_to>(
        p_begin, p_end,
        [&_is_valid](const char *p_begin, const char *p_end, const char **p_c) {
          return _is_valid(p_begin, p_end, p_c);
        },
        _multiplier, _incrementer);
  }

  constexpr std::pair<std::unique_ptr<std::string>, t_to>
  unsigned_int(const char *p_begin, const char *p_end) {

    using next_int = typename next_int<t_to>::type;

    auto _multiplier =
        [&](const char *p_begin, const char *p_end,
            t_to p_to) -> std::pair<std::unique_ptr<std::string>, t_to> {
      auto _mul{static_cast<next_int>(p_to) * 10};
      if ((_mul == 0) || (_mul > std::numeric_limits<t_to>::max())) {
        return overflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_mul)};
    };

    auto _incrementer =
        [&](const char *p_begin, const char *p_end, t_to p_value,
            char p_c) -> std::pair<std::unique_ptr<std::string>, t_to> {
      if (p_c == '0') {
        return {nullptr, static_cast<t_to>(p_value)};
      }
      auto _sum{static_cast<next_int>(p_value + (p_c - '0'))};
      if ((_sum == 0) || (_sum > std::numeric_limits<t_to>::max())) {
        return overflows<t_to>(p_begin, p_end);
      }
      return {nullptr, static_cast<t_to>(_sum)};
    };

    is_valid _is_valid;

    return to_integer<t_to>(
        p_begin, p_end,
        [&_is_valid](const char *p_begin, const char *p_end, const char **p_c) {
          return _is_valid(p_begin, p_end, p_c);
        },
        _multiplier, _incrementer);
  }
};

template <typename t_to>
std::pair<std::unique_ptr<std::string>, t_to> to_integer(
    const char *p_begin, const char *p_end,

    std::function<std::unique_ptr<std::string>(const char *, const char *,
                                               const char **)>
        p_is_valid,

    std::function<typ::result<t_to>(const char *p_begin, const char *p_end,
                                    t_to p_value)>
        p_multiplier,

    std::function<typ::result<t_to>(const char *p_begin, const char *p_end,
                                    t_to p_result, char p_to_increment)>
        p_incrementer) {

  t_to _result = 0;

  const char *_ite{p_begin};

  while (_ite != p_end) {

    auto _ptr{p_is_valid(p_begin, p_end, &_ite)};
    if (_ptr) {
      return {std::move(_ptr), {}};
    }

    typ::result<t_to> _pair;

    if (_result != 0) {
      _pair = p_multiplier(p_begin, p_end, _result);

      if (_pair.first) {
        return {std::move(_pair.first), {}};
      }

      _result = _pair.second;
    }

    _pair = p_incrementer(p_begin, p_end, _result, *_ite);

    if (_pair.first) {
      return {std::move(_pair.first), {}};
    }

    _result = _pair.second;

    ++_ite;
  }
  return {nullptr, _result};
}

// template <typename t_to, char t_thousand_separator>
// constexpr std::pair<std::unique_ptr<std::string>, t_to>
// to_uint(const char *p_begin, const char *p_end) {
//   using next_int = typename next_int<sizeof(t_to), false>::type;

//  auto _over_under = [&](next_int p_value) -> bool {
//    return p_value > std::numeric_limits<t_to>::max();
//  };

//  auto _make_ptr = [&](const char *p_begin, const char *p_end) {
//    return std::make_unique<std::string>(
//        std::string{p_begin, p_end} + " overflows the maximum value for " +
//        int_name<t_to>());
//  };

//  auto _incrementer = [&](t_to p_value, uint8_t p_digit) {
//    return static_cast<next_int>(p_value + p_digit);
//  };

//  return to_int<t_to, t_thousand_separator, false>(p_begin, p_end,
//  _over_under,
//                                                   _make_ptr, _incrementer);
//}

// template <typename t_to, char t_thousand_separator>
// constexpr std::pair<std::unique_ptr<std::string>, t_to>
// to_sint(const char *p_begin, const char *p_end) {
//   using next_int = typename next_int<sizeof(t_to), true>::type;

//  auto _over_under = [&](next_int p_value) -> bool {
//    return p_value < std::numeric_limits<t_to>::min();
//  };

//  auto _make_ptr = [&](const char *p_begin, const char *p_end) {
//    return std::make_unique<std::string>(
//        std::string{p_begin, p_end} + " underflows the minimum value for " +
//        int_name<t_to>());
//  };

//  auto _incrementer = [&](t_to p_value, uint8_t p_digit) {
//    return static_cast<next_int>(p_value - p_digit);
//  };

//  return to_int<t_to, t_thousand_separator, true>(p_begin, p_end,
//  _over_under,
//                                                  _make_ptr, _incrementer);
//}

// template <typename t_to>
// std::pair<std::unique_ptr<std::string>, t_to>
// to_int_from_hexa(const char *p_begin, const char *p_end) {

//  using next_int = typename next_int<sizeof(t_to), false>::type;

//  auto _is_valid = [](const char *p_begin, const char *p_end,
//                      char p_c) -> std::unique_ptr<std::string> {
//    if (std::isdigit(p_c) || (std::isxdigit(p_c))) {
//      return nullptr;
//    }
//    return std::make_unique<std::string>(std::string{p_c} +
//                                         " is not an hexadecimal digit in "
//                                         + std::string{p_begin, p_end});
//  };

//  auto _multiplier =
//      [&](const char *p_begin, const char *p_end,
//          t_to p_to) -> std::pair<std::unique_ptr<std::string>, t_to> {
//    auto _mul{static_cast<next_int>(p_to) * 16};
//    if (_mul > std::numeric_limits<t_to>::max()) {
//      return {std::make_unique<std::string>(
//                  std::string{p_begin, p_end} +
//                  " overflows the maximum value for " +
//                  int_name<t_to>()),
//              {}};
//    }
//    return {nullptr, static_cast<t_to>(_mul)};
//  };

//  auto _incrementer =
//      [&](const char *p_begin, const char *p_end, t_to p_value,
//          char p_c) -> std::pair<std::unique_ptr<std::string>, t_to> {
//    char _c{static_cast<char>(std::toupper(p_c))};

//    uint8_t _u{0};
//    switch (_c) {
//    case 'A':
//      _u = 10;
//      break;
//    case 'B':
//      _u = 11;
//      break;
//    case 'C':
//      _u = 12;
//      break;
//    case 'D':
//      _u = 13;
//      break;
//    case 'E':
//      _u = 14;
//      break;
//    case 'F':
//      _u = 15;
//      break;
//    default:
//      _u = p_c - '0';
//    }

//    auto _sum{static_cast<next_int>(p_value + _u)};
//    if (_sum > std::numeric_limits<t_to>::max()) {
//      return {std::make_unique<std::string>(
//                  std::string{p_begin, p_end} +
//                  " overflows the maximum value for " +
//                  int_name<t_to>()),
//              {}};
//    }

//    return {nullptr, static_cast<t_to>(_sum)};
//  };

//  return to_int1<t_to, ',', false>(p_begin, p_end, _is_valid, _multiplier,
//                                   _incrementer);
//}

// template <typename t_to, char t_thousand_separator, bool t_negative>
// std::pair<std::unique_ptr<std::string>, t_to> to_int(
//     const char *p_begin, const char *p_end,
//     std::function<
//         bool(typename next_int<sizeof(t_to), t_negative>::type p_value)>
//         p_over_under,
//     std::function<std::unique_ptr<std::string>(const char *p_begin,
//                                                const char *p_end)>
//         p_make_ptr,

//    std::function<typename next_int<sizeof(t_to), t_negative>::type(
//        t_to p_increment, uint8_t p_digit)>
//        p_incrementer,

//    std::function<bool(char)> p_is_valid =
//        [](char p_c) { return std::isdigit(p_c); },

//    std::function<typename next_int<sizeof(t_to), t_negative>::type(t_to)>
//        p_multiplier =
//            [](t_to p_to) {
//              return static_cast<
//                  typename next_int<sizeof(t_to), t_negative>::type>(p_to *
//                  10);
//            }) {

//  t_to _result = 0;

//  const char *_ite{p_begin};

//  while (_ite != p_end) {
//    if (!p_is_valid(*_ite)) {
//      return {
//          std::make_unique<std::string>("there is a non digit character in "
//          +
//                                        std::string{p_begin, p_end}),
//          {}};
//    }

//    const auto _prod{p_multiplier(_result)};

//    if ((_result != 0) && ((_prod == 0) || (p_over_under(_prod)))) {
//      return {p_make_ptr(p_begin, p_end), {}};
//    }

//    _result = _prod;

//    const auto _sub{p_incrementer(_result, static_cast<uint8_t>(*_ite -
//    '0'))};

//    if ((_result != 0) && ((_sub == 0) || (p_over_under(_sub)))) {
//      return {p_make_ptr(p_begin, p_end), {}};
//    }

//    _result = _sub;

//    ++_ite;
//  }
//  return {nullptr, _result};
//}
} // namespace tenacitas::lib::conversions::alg::internal

#endif

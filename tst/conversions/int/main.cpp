#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>

#include <tenacitas/conversions/alg/cvt.h>
#include <tenacitas/program/alg/options.h>
#include <tenacitas/test/alg/tester.h>

using namespace tenacitas::lib;

namespace typ {
struct a {
  friend std::ostream &operator<<(std::ostream &p_out, const a &) {
    p_out << "a";
    return p_out;
  }
};
struct b {};
} // namespace typ

// struct testNoConversion {
//   static std::string desc() { return "test conversions::alg::cvt<a>(b)";
//   }

//  bool operator()(program::alg::options &) {
//    using namespace tenacitas::lib::conversions::tst;
//    b _b;

//    auto _maybe{conversions::alg::cvt<a>(_b)};

//    if (_maybe.first) {
//      std::cerr << "It did convert, as it should not, because '"
//                << *_maybe.first << '\n';
//      return true;
//    }

//    std::cerr << "It converted to " << _maybe.second << ", but it should not";
//    return false;
//  }
//};

template <typename t_int> struct test_unsigned_int_max {
  static std::string desc() {
    std::stringstream _stream;

    _stream << "conversions::alg::cvt<"
            << conversions::alg::internal::int_name<t_int>() << ">(\""
            << static_cast<uint64_t>(std::numeric_limits<t_int>::max())
            << "\"), which is std::numeric_limits<"
            << conversions::alg::internal::int_name<t_int>() << ">::max()";
    return _stream.str();
  }

  bool operator()(const program::alg::options &) {

    std::stringstream _stream;
    _stream << static_cast<uint64_t>(std::numeric_limits<t_int>::max());

    std::string _value{_stream.str()};

    auto _maybe{conversions::alg::cvt<t_int>(_value)};
    if (_maybe.first) {
      std::cerr << "It did not convert, but it should have, because '"
                << *_maybe.first << "'\n";
      return false;
    }

    if (_maybe.second != std::numeric_limits<t_int>::max()) {
      std::cerr << "It converted to " << static_cast<uint64_t>(_maybe.second)
                << ", but it should have converted to " << _value << "\n";
    }

    std::cerr << "It converted to " << static_cast<uint64_t>(_maybe.second)
              << ", as expected\n";
    return true;
  }
};

template <typename t_int> struct test_unsigned_int_max_overflow {
  static std::string desc() {

    std::stringstream _stream;

    if constexpr (std::is_same_v<t_int, uint64_t>) {
      _stream << "conversions::alg::cvt<"
              << conversions::alg::internal::int_name<t_int>() << ">(\""
              << "18446744073709551616"
              << "\"), which is std::numeric_limits<"
              << conversions::alg::internal::int_name<t_int>()
              << ">::max() + 1";

    } else {
      _stream << "conversions::alg::cvt<"
              << conversions::alg::internal::int_name<t_int>() << ">(\""
              << static_cast<uint64_t>(std::numeric_limits<t_int>::max()) + 1
              << "\"), which is std::numeric_limits<"
              << conversions::alg::internal::int_name<t_int>()
              << ">::max() + 1";
    }
    return _stream.str();
  }

  bool operator()(const program::alg::options &) {

    std::stringstream _stream;
    _stream << static_cast<uint64_t>(std::numeric_limits<t_int>::max());

    std::string _value{_stream.str()};

    int8_t _last(_value[_value.size() - 1] - '0');
    ++_last;
    _value[_value.size() - 1] = _last + '0';

    auto _maybe{conversions::alg::cvt<t_int>(_value)};
    if (!_maybe.first) {
      std::cerr << "It converted " << static_cast<uint64_t>(_maybe.second)
                << ", but it should not have\n";
      return false;
    }

    std::cerr << "It did not convert, as expected, because '" << *_maybe.first
              << "'\n";
    return true;
  }
};

template <typename t_int> struct test_signed_int_max {
  static std::string desc() {
    std::stringstream _stream;
    _stream << "conversions::alg::cvt<"
            << conversions::alg::internal::int_name<t_int>() << ">(\""
            << static_cast<int64_t>(std::numeric_limits<t_int>::max())
            << "\"), which is std::numeric_limits<"
            << conversions::alg::internal::int_name<t_int>() << ">::max()";
    return _stream.str();
  }

  bool operator()(const program::alg::options &) {

    std::stringstream _stream;
    _stream << static_cast<int64_t>(std::numeric_limits<t_int>::max());

    std::string _value{_stream.str()};

    auto _maybe{conversions::alg::cvt<t_int>(_value)};
    if (_maybe.first) {
      std::cerr << "It did not convert, but it should have, because '"
                << *_maybe.first << "'\n";
      return false;
    }

    if (_maybe.second != std::numeric_limits<t_int>::max()) {
      std::cerr << "It converted to " << static_cast<int64_t>(_maybe.second)
                << ", but it should have converted to " << _value << "\n";
    }

    std::cerr << "It converted to " << static_cast<int64_t>(_maybe.second)
              << ", as expected\n";
    return true;
  }
};

template <typename t_int> struct test_signed_int_max_overflow {
  static std::string desc() {
    std::stringstream _stream;
    if constexpr (std::is_same_v<t_int, int64_t>) {
      _stream << "conversions::alg::cvt<"
              << conversions::alg::internal::int_name<t_int>() << ">(\""
              << "9223372036854775808"
              << "\"), which is std::numeric_limits<"
              << conversions::alg::internal::int_name<t_int>()
              << ">::max() + 1";

    } else {
      _stream << "conversions::alg::cvt<"
              << conversions::alg::internal::int_name<t_int>() << ">(\""
              << static_cast<uint64_t>(std::numeric_limits<t_int>::max()) + 1
              << "\"), which is std::numeric_limits<"
              << conversions::alg::internal::int_name<t_int>()
              << ">::max() + 1";
    }
    return _stream.str();
  }

  bool operator()(const program::alg::options &) {

    std::stringstream _stream;
    _stream << static_cast<int64_t>(std::numeric_limits<t_int>::max());

    std::string _value{_stream.str()};

    int8_t _last(_value[_value.size() - 1] - '0');
    ++_last;
    _value[_value.size() - 1] = _last + '0';

    auto _maybe{conversions::alg::cvt<t_int>(_value)};
    if (!_maybe.first) {
      std::cerr << "It converted to " << static_cast<int64_t>(_maybe.second)
                << ", but it should not have\n";
      return false;
    }

    std::cerr << "It did not convert, as expected, because '" << *_maybe.first
              << "'\n";

    return true;
  }
};

template <typename t_int> struct test_signed_int_min {
  static std::string desc() {
    std::stringstream _stream;
    _stream << "conversions::alg::cvt<"
            << conversions::alg::internal::int_name<t_int>() << ">(\""
            << static_cast<int64_t>(std::numeric_limits<t_int>::min())
            << "\"), which is std::numeric_limits<"
            << conversions::alg::internal::int_name<t_int>() << ">::min() ";
    return _stream.str();
  }
  bool operator()(const program::alg::options &) {

    std::stringstream _stream;
    _stream << static_cast<int64_t>(std::numeric_limits<t_int>::min());

    std::string _value{_stream.str()};

    auto _maybe{conversions::alg::cvt<t_int>(_value)};
    if (_maybe.first) {
      std::cerr << "It did not convert, but it should have, because '"
                << *_maybe.first << "'\n";
      return false;
    }

    if (_maybe.second != std::numeric_limits<t_int>::min()) {
      std::cerr << "It should have converted to " << _value
                << ", but it converted to "
                << static_cast<int64_t>(_maybe.second) << '\n';
      return false;
    }

    std::cerr << "It converted to " << static_cast<int64_t>(_maybe.second)
              << ", as it should \n";
    return true;
  }
};

template <typename t_int> struct test_signed_int_min_underflow {
  static std::string desc() {
    std::stringstream _stream;
    if constexpr (std::is_same_v<t_int, int64_t>) {
      _stream << "conversions::alg::cvt<"
              << conversions::alg::internal::int_name<t_int>() << ">(\""
              << "-9223372036854775809"
              << "\"), which is std::numeric_limits<"
              << conversions::alg::internal::int_name<t_int>()
              << ">::min() - 1";
    } else {
      _stream << "conversions::alg::cvt<"
              << conversions::alg::internal::int_name<t_int>() << ">(\""
              << static_cast<int64_t>(std::numeric_limits<t_int>::min()) - 1
              << "\"), which is std::numeric_limits<"
              << conversions::alg::internal::int_name<t_int>()
              << ">::min() - 1";
    }
    return _stream.str();
  }

  bool operator()(const program::alg::options &) {

    std::stringstream _stream;
    _stream << static_cast<int64_t>(std::numeric_limits<t_int>::min());

    std::string _value{_stream.str()};

    int8_t _last(_value[_value.size() - 1] - '0');
    ++_last;
    _value[_value.size() - 1] = _last + '0';

    auto _maybe{conversions::alg::cvt<t_int>(_value)};
    if (!_maybe.first) {
      std::cerr << "It converted to " << static_cast<int64_t>(_maybe.second)
                << ", but it should not have\n";
      return false;
    }

    std::cerr << "It did not convert, as expected, because '" << *_maybe.first
              << "'\n";

    return true;
  }
};

template <size_t t_test_number, typename t_to, conversions::typ::base t_base,
          t_to t_converted>
struct test_converted {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "conversions::alg::cvt<"
            << conversions::alg::internal::int_name<t_to>() << ">(\"" << str
            << "\"), base " << t_base;

    return _stream.str();
  }

  bool operator()(const program::alg::options &) {
    auto _result{conversions::alg::cvt<t_to, t_base>(str)};

    if (_result.first) {
      std::cerr << "It did not convert, as it should have, because '"
                << *_result.first << "'\n";

      return false;
    }

    if (_result.second != t_converted) {
      std::cerr
          << "It should have converted to '"
          << (t_base == conversions::typ::base::b16
                  ? std::hex
                  : (t_base == conversions::typ::base::b8 ? std::oct
                                                          : std::dec))
          << static_cast<
                 typename conversions::alg::internal::next_int<t_to>::type>(
                 t_converted)
          << "', but it converted to " << static_cast<uint16_t>(_result.second)
          << '\n';
      return false;
    }

    std::cerr << "It converted to '"
              << (t_base == conversions::typ::base::b16
                      ? std::hex
                      : (t_base == conversions::typ::base::b8 ? std::oct
                                                              : std::dec))
              << static_cast<
                     typename conversions::alg::internal::next_int<t_to>::type>(
                     t_converted)
              << "' as it should \n";
    return true;
  }

  static std::string str;
};
template <size_t t_test_number, typename t_to, conversions::typ::base t_base,
          t_to t_converted>
std::string test_converted<t_test_number, t_to, t_base, t_converted>::str{
    "--not-set--"};

template <size_t t_test_number, typename t_to, conversions::typ::base t_base>
struct test_not_converted {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "conversions::alg::cvt<"
            << conversions::alg::internal::int_name<t_to>() << ">(\"" << str
            << "\"), base " << t_base;

    return _stream.str();
  }

  bool operator()(const program::alg::options &) {
    auto _result{conversions::alg::cvt<t_to, t_base>(str)};

    if (_result.first) {
      std::cerr << "It dit not convert, as expected, because '"
                << *_result.first << "'\n";

      return true;
    }

    std::cerr << "It converted to '"
              << (t_base == conversions::typ::base::b16
                      ? std::hex
                      : (t_base == conversions::typ::base::b8 ? std::oct
                                                              : std::dec))
              << static_cast<
                     typename conversions::alg::internal::next_int<t_to>::type>(
                     _result.second)
              << "' but it should not\n";
    return false;
  }

  static std::string str;
};
template <size_t t_test_number, typename t_to, conversions::typ::base t_base>
std::string test_not_converted<t_test_number, t_to, t_base>::str{"--not-set--"};

using test0000 = test_unsigned_int_max<uint8_t>;
using test0001 = test_unsigned_int_max_overflow<uint8_t>;
using test0002 = test_signed_int_max<int8_t>;
using test0003 = test_signed_int_max_overflow<int8_t>;
using test0004 = test_signed_int_min<int8_t>;
using test0005 = test_signed_int_min_underflow<int8_t>;

using test0006 = test_unsigned_int_max<uint16_t>;
using test0007 = test_unsigned_int_max_overflow<uint16_t>;
using test0008 = test_signed_int_max<int16_t>;
using test0009 = test_signed_int_max_overflow<int16_t>;
using test0010 = test_signed_int_min<int16_t>;
using test0011 = test_signed_int_min_underflow<int16_t>;

using test0012 = test_unsigned_int_max<uint32_t>;
using test0013 = test_unsigned_int_max_overflow<uint32_t>;
using test0014 = test_signed_int_max<int32_t>;
using test0015 = test_signed_int_max_overflow<int32_t>;
using test0016 = test_signed_int_min<int32_t>;
using test0017 = test_signed_int_min_underflow<int32_t>;

using test0018 = test_unsigned_int_max<uint64_t>;
using test0019 = test_unsigned_int_max_overflow<uint64_t>;
using test0020 = test_signed_int_max<int64_t>;
using test0021 = test_signed_int_max_overflow<int64_t>;
using test0022 = test_signed_int_min<int64_t>;
using test0023 = test_signed_int_min_underflow<int64_t>;

using test0024 = test_converted<24, uint8_t, conversions::typ::base::b16, 0xE>;
template <> std::string test0024::str = "0xE";

using test0025 = test_not_converted<25, uint8_t, conversions::typ::base::b16>;
template <> std::string test0025::str = "0x1C";

using test0026 = test_not_converted<26, uint8_t, conversions::typ::base::b10>;
template <> std::string test0026::str = "-8";

using test0027 = test_converted<27, uint8_t, conversions::typ::base::b2, 0b11>;
template <> std::string test0027::str = "11";

using test0028 =
    test_converted<28, uint16_t, conversions::typ::base::b2, 0b01100110>;
template <> std::string test0028::str = "01100110";

using test0029 =
    test_converted<29, uint16_t, conversions::typ::base::b2, 0b01100111>;
template <> std::string test0029::str = "0b01100111";

using test0030 = test_converted<30, uint8_t, conversions::typ::base::b8, 037>;
template <> std::string test0030::str = "37";

using test0031 = test_converted<31, uint8_t, conversions::typ::base::b10, 122>;
template <> std::string test0031::str = "0000122";

using test0032 = test_not_converted<32, uint8_t, conversions::typ::base::b10>;
template <> std::string test0032::str = "2.4";

using test0033 = test_not_converted<33, uint8_t, conversions::typ::base::b10>;
template <> std::string test0033::str = "1c2";

using test0034 = test_not_converted<34, int8_t, conversions::typ::base::b10>;
template <> std::string test0034::str = "-129";

using test0035 = test_converted<35, uint8_t, conversions::typ::base::b10, 127>;
template <> std::string test0035::str = "127";

using test0036 = test_not_converted<36, uint32_t, conversions::typ::base::b2>;
template <> std::string test0036::str = "-1101";

// causes a compile time error
using test0037 = test_not_converted<37, int32_t, conversions::typ::base::b16>;
template <> std::string test0037::str = "9A";

// causes a compile time error
using test0038 = test_not_converted<38, int32_t, conversions::typ::base::b2>;
template <> std::string test0038::str = "101";

// causes a compile time error
using test0039 = test_not_converted<39, int32_t, conversions::typ::base::b8>;
template <> std::string test0039::str = "73";

int main(int argc, char **argv) {
  using namespace tenacitas;
  lib::test::alg::tester _test(argc, argv);

  // run_test(_test, testNoConversion);
  run_test(_test, test0000);
  run_test(_test, test0001);
  run_test(_test, test0002);
  run_test(_test, test0003);
  run_test(_test, test0004);
  run_test(_test, test0005);
  run_test(_test, test0006);
  run_test(_test, test0007);
  run_test(_test, test0008);
  run_test(_test, test0009);
  run_test(_test, test0010);
  run_test(_test, test0011);
  run_test(_test, test0012);
  run_test(_test, test0013);
  run_test(_test, test0014);
  run_test(_test, test0015);
  run_test(_test, test0016);
  run_test(_test, test0017);
  run_test(_test, test0018);
  run_test(_test, test0019);
  run_test(_test, test0020);
  run_test(_test, test0021);
  run_test(_test, test0022);
  run_test(_test, test0023);
  run_test(_test, test0024);
  run_test(_test, test0025);
  run_test(_test, test0026);
  run_test(_test, test0027);
  run_test(_test, test0028);
  run_test(_test, test0029);
  run_test(_test, test0030);
  run_test(_test, test0031);
  run_test(_test, test0032);
  run_test(_test, test0033);
  run_test(_test, test0034);
  run_test(_test, test0035);
  run_test(_test, test0036);

  // the test below causes a compile time error
  //  run_test(_test, test0037);

  // the test below causes a compile time error
  //  run_test(_test, test0038);

  // the test below causes a compile time error
  //  run_test(_test, test0039);
}

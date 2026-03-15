#ifndef TNCT_MATH_TST_MUL_H
#define TNCT_MATH_TST_MUL_H

#include <cstdint>
#include <limits>
#include <optional>
#include <string>

#include "tnct/math/bus/internal/mul.h"
#include "tnct/math/bus/mul.h"
#include "tnct/program/bus/options.h"

namespace tnct::math::test
{

/*----------------------------------------------------------
  mul_same (signed)
----------------------------------------------------------*/

struct test_mul_000
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(std::int8_t{3}, std::int8_t{4});
    return r && (*r == std::int8_t{12});
  }

  static std::string desc()
  {
    return "internal::mul_same(3,4) == 12";
  }
};

struct test_mul_001
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(std::int8_t{-3}, std::int8_t{4});
    return r && (*r == std::int8_t{-12});
  }

  static std::string desc()
  {
    return "internal::mul_same(-3,4) == -12";
  }
};

struct test_mul_002
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(std::int8_t{-3}, std::int8_t{-4});
    return r && (*r == std::int8_t{12});
  }

  static std::string desc()
  {
    return "internal::mul_same(-3,-4) == 12";
  }
};

struct test_mul_003
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(
        std::numeric_limits<std::int8_t>::max(), std::int8_t{2});
    return !r;
  }

  static std::string desc()
  {
    return "internal::mul_same(int8 max,2) overflow";
  }
};

/*----------------------------------------------------------
  mul_same (unsigned)
----------------------------------------------------------*/

struct test_mul_004
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(std::uint8_t{3}, std::uint8_t{4});
    return r && (*r == std::uint8_t{12});
  }

  static std::string desc()
  {
    return "internal::mul_same(3u,4u) == 12";
  }
};

struct test_mul_005
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(std::uint8_t{200}, std::uint8_t{2});
    return !r;
  }

  static std::string desc()
  {
    return "internal::mul_same(uint8 overflow)";
  }
};

/*----------------------------------------------------------
  mul_same_operands
----------------------------------------------------------*/

struct test_mul_006
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same_operands<std::int16_t>(
        std::int8_t{5}, std::int8_t{6});

    return r && (*r == std::int16_t{30});
  }

  static std::string desc()
  {
    return "mul_same_operands<int16>(5,6)==30";
  }
};

struct test_mul_007
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same_operands<std::int8_t>(
        std::int16_t{100}, std::int16_t{2});

    return !r;
  }

  static std::string desc()
  {
    return "mul_same_operands<int8> overflow";
  }
};

/*----------------------------------------------------------
  different operands (signed result)
----------------------------------------------------------*/

struct test_mul_008
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::mul_different_operands_signed_result<std::int16_t>(
            std::int8_t{5}, std::uint16_t{4});

    return r && (*r == std::int16_t{20});
  }

  static std::string desc()
  {
    return "5 * 4u == 20";
  }
};

struct test_mul_009
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::mul_different_operands_signed_result<std::int16_t>(
            std::int8_t{-5}, std::uint16_t{4});

    return r && (*r == std::int16_t{-20});
  }

  static std::string desc()
  {
    return "-5 * 4u == -20";
  }
};

struct test_mul_010
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::mul_different_operands_signed_result<std::int16_t>(
            std::uint16_t{4}, std::int8_t{-5});

    return r && (*r == std::int16_t{-20});
  }

  static std::string desc()
  {
    return "4u * -5 == -20";
  }
};

/*----------------------------------------------------------
  different operands (unsigned result)
----------------------------------------------------------*/

struct test_mul_011
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::mul_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{5}, std::uint16_t{4});

    return r && (*r == std::uint16_t{20});
  }

  static std::string desc()
  {
    return "5 * 4u == 20 (unsigned result)";
  }
};

struct test_mul_012
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::mul_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{-5}, std::uint16_t{4});

    return !r;
  }

  static std::string desc()
  {
    return "-5 * 4u cannot be unsigned";
  }
};

/*----------------------------------------------------------
  public API
----------------------------------------------------------*/

struct test_mul_013
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::mul<std::int16_t>(std::int8_t{6}, std::int8_t{7});
    return r && (*r == std::int16_t{42});
  }

  static std::string desc()
  {
    return "bus::mul<int16>(6,7)==42";
  }
};

struct test_mul_014
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::mul<std::uint16_t>(std::uint8_t{6}, std::uint8_t{7});
    return r && (*r == std::uint16_t{42});
  }

  static std::string desc()
  {
    return "bus::mul<uint16>(6u,7u)==42";
  }
};

/*----------------------------------------------------------
  floating point
----------------------------------------------------------*/

struct test_mul_015
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::mul<float>(2.0, 3.0);
    return r && (*r > 5.99f) && (*r < 6.01f);
  }

  static std::string desc()
  {
    return "mul<float>(2,3)==6";
  }
};

struct test_mul_016
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::mul<float>(std::numeric_limits<float>::max(), 2);

    return !r;
  }

  static std::string desc()
  {
    return "mul<float> overflow -> nullopt";
  }
};

struct test_mul_017
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(
        std::numeric_limits<std::int8_t>::min(), std::int8_t{-1});
    return !r;
  }

  static std::string desc()
  {
    return "internal::mul_same(int8 min, -1) overflows";
  }
};

struct test_mul_018
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::mul_same(
        std::int8_t{-1}, std::numeric_limits<std::int8_t>::min());
    return !r;
  }

  static std::string desc()
  {
    return "internal::mul_same(-1, int8 min) overflows";
  }
};

struct test_mul_019
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::mul_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{-5}, std::uint16_t{0});
    return r && (*r == std::uint16_t{0});
  }

  static std::string desc()
  {
    return "(-5) * 0u == 0 for unsigned result";
  }
};

struct test_mul_020
{
  bool operator()(const program::bus::options &)
  {
    const double nan = std::numeric_limits<double>::quiet_NaN();

    const auto r = bus::mul<float>(nan, 2.0);

    return !r;
  }

  static std::string desc()
  {
    return "mul<float>(NaN,2) -> nullopt";
  }
};

struct test_mul_021
{
  bool operator()(const program::bus::options &)
  {
    const double big = std::numeric_limits<double>::max();

    const auto r = bus::mul<float>(big, 1.0);

    return !r;
  }

  static std::string desc()
  {
    return "mul<float>(double max,1) -> nullopt after float conversion";
  }
};
} // namespace tnct::math::test

#endif

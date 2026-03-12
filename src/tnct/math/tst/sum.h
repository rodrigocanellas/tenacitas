#ifndef TNCT_MATH_TST_SUM_H
#define TNCT_MATH_TST_SUM_H

#include <cstdint>
#include <limits>
#include <optional>
#include <string>

#include "tnct/math/bus/internal/sum.h"
#include "tnct/math/bus/revert.h"
#include "tnct/math/bus/sum.h"
#include "tnct/program/bus/options.h"

using namespace tnct;

namespace tnct::math::test
{

struct test_000
{
  bool operator()(const program::bus::options &)
  {
    return bus::revert(std::int8_t{-1}) == std::uint8_t{1};
  }

  static std::string desc()
  {
    return "revert<int8_t>(-1) == 1";
  }
};

struct test_001
{
  bool operator()(const program::bus::options &)
  {
    return bus::revert(std::int8_t{-5}) == std::uint8_t{5};
  }

  static std::string desc()
  {
    return "revert<int8_t>(-5) == 5";
  }
};

struct test_002
{
  bool operator()(const program::bus::options &)
  {
    return bus::revert(std::int8_t{0}) == std::uint8_t{0};
  }

  static std::string desc()
  {
    return "revert<int8_t>(0) == 0";
  }
};

struct test_003
{
  bool operator()(const program::bus::options &)
  {
    return bus::revert(std::numeric_limits<std::int8_t>::min())
           == std::uint8_t{128};
  }

  static std::string desc()
  {
    return "revert<int8_t>(min) == 128";
  }
};

struct test_004
{
  bool operator()(const program::bus::options &)
  {
    return bus::revert(std::numeric_limits<std::int16_t>::min())
           == std::uint16_t{32768};
  }

  static std::string desc()
  {
    return "revert<int16_t>(min) == 32768";
  }
};

struct test_005
{
  bool operator()(const program::bus::options &)
  {
    return bus::internal::fits<std::int8_t>(std::int16_t{127});
  }

  static std::string desc()
  {
    return "fits<int8_t>(int16_t{127})";
  }
};

struct test_006
{
  bool operator()(const program::bus::options &)
  {
    return !bus::internal::fits<std::int8_t>(std::int16_t{128});
  }

  static std::string desc()
  {
    return "!fits<int8_t>(int16_t{128})";
  }
};

struct test_007
{
  bool operator()(const program::bus::options &)
  {
    return !bus::internal::fits<std::int8_t>(std::int16_t{-129});
  }

  static std::string desc()
  {
    return "!fits<int8_t>(int16_t{-129})";
  }
};

struct test_008
{
  bool operator()(const program::bus::options &)
  {
    return bus::internal::fits<std::uint8_t>(std::uint16_t{255});
  }

  static std::string desc()
  {
    return "fits<uint8_t>(uint16_t{255})";
  }
};

struct test_009
{
  bool operator()(const program::bus::options &)
  {
    return !bus::internal::fits<std::uint8_t>(std::uint16_t{256});
  }

  static std::string desc()
  {
    return "!fits<uint8_t>(uint16_t{256})";
  }
};

struct test_010
{
  bool operator()(const program::bus::options &)
  {
    return bus::internal::fits<std::uint8_t>(std::int16_t{42});
  }

  static std::string desc()
  {
    return "fits<uint8_t>(int16_t{42})";
  }
};

struct test_011
{
  bool operator()(const program::bus::options &)
  {
    return !bus::internal::fits<std::uint8_t>(std::int16_t{-1});
  }

  static std::string desc()
  {
    return "!fits<uint8_t>(int16_t{-1})";
  }
};

struct test_012
{
  bool operator()(const program::bus::options &)
  {
    return !bus::internal::fits<std::uint8_t>(std::int16_t{256});
  }

  static std::string desc()
  {
    return "!fits<uint8_t>(int16_t{256})";
  }
};

struct test_013
{
  bool operator()(const program::bus::options &)
  {
    return bus::internal::fits<std::int8_t>(std::uint16_t{127});
  }

  static std::string desc()
  {
    return "fits<int8_t>(uint16_t{127})";
  }
};

struct test_014
{
  bool operator()(const program::bus::options &)
  {
    return !bus::internal::fits<std::int8_t>(std::uint16_t{128});
  }

  static std::string desc()
  {
    return "!fits<int8_t>(uint16_t{128})";
  }
};

struct test_015
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same(std::int8_t{10}, std::int8_t{20});
    return r && (*r == std::int8_t{30});
  }

  static std::string desc()
  {
    return "internal::sum_same(int8_t{10}, int8_t{20}) == 30";
  }
};

struct test_016
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same(
        std::numeric_limits<std::int8_t>::max(), std::int8_t{1});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sum_same(int8_t max, 1) overflows";
  }
};

struct test_017
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same(
        std::numeric_limits<std::int8_t>::min(), std::int8_t{-1});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sum_same(int8_t min, -1) underflows";
  }
};

struct test_018
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same(std::uint8_t{10}, std::uint8_t{20});
    return r && (*r == std::uint8_t{30});
  }

  static std::string desc()
  {
    return "internal::sum_same(uint8_t{10}, uint8_t{20}) == 30";
  }
};

struct test_019
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same(
        std::numeric_limits<std::uint8_t>::max(), std::uint8_t{1});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sum_same(uint8_t max, 1) overflows";
  }
};

struct test_020
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same_operands<std::int16_t>(
        std::int8_t{100}, std::int8_t{20});
    return r && (*r == std::int16_t{120});
  }

  static std::string desc()
  {
    return "internal::sum_same_operands<int16_t>(int8_t{100}, int8_t{20}) == "
           "120";
  }
};

struct test_021
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same_operands<std::int8_t>(
        std::int16_t{100}, std::int16_t{40});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sum_same_operands<int8_t>(int16_t{100}, int16_t{40}) "
           "fails";
  }
};

struct test_022
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same_operands<std::uint16_t>(
        std::uint8_t{200}, std::uint8_t{55});
    return r && (*r == std::uint16_t{255});
  }

  static std::string desc()
  {
    return "internal::sum_same_operands<uint16_t>(uint8_t{200}, uint8_t{55}) "
           "== 255";
  }
};

struct test_023
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sum_same_operands<std::uint8_t>(
        std::uint16_t{200}, std::uint16_t{100});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sum_same_operands<uint8_t>(uint16_t{200}, uint16_t{100}) "
           "fails";
  }
};

struct test_024
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_signed_result<std::int16_t>(
            std::int8_t{4}, std::uint16_t{9});
    return r && (*r == std::int16_t{13});
  }

  static std::string desc()
  {
    return "signed result: 4 + 9u == 13";
  }
};

struct test_025
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_signed_result<std::int16_t>(
            std::int8_t{-4}, std::uint16_t{9});
    return r && (*r == std::int16_t{5});
  }

  static std::string desc()
  {
    return "signed result: -4 + 9u == 5";
  }
};

struct test_026
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_signed_result<std::int16_t>(
            std::int8_t{-9}, std::uint16_t{9});
    return r && (*r == std::int16_t{0});
  }

  static std::string desc()
  {
    return "signed result: -9 + 9u == 0";
  }
};

struct test_027
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_signed_result<std::int16_t>(
            std::int8_t{-9}, std::uint16_t{4});
    return r && (*r == std::int16_t{-5});
  }

  static std::string desc()
  {
    return "signed result: -9 + 4u == -5";
  }
};

struct test_028
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_signed_result<std::int8_t>(
            std::int16_t{100}, std::uint16_t{40});
    return !r;
  }

  static std::string desc()
  {
    return "signed result too small for 100 + 40u";
  }
};

struct test_029
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{4}, std::uint16_t{9});
    return r && (*r == std::uint16_t{13});
  }

  static std::string desc()
  {
    return "unsigned result: 4 + 9u == 13";
  }
};

struct test_030
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{-4}, std::uint16_t{9});
    return r && (*r == std::uint16_t{5});
  }

  static std::string desc()
  {
    return "unsigned result: -4 + 9u == 5";
  }
};

struct test_031
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{-9}, std::uint16_t{9});
    return r && (*r == std::uint16_t{0});
  }

  static std::string desc()
  {
    return "unsigned result: -9 + 9u == 0";
  }
};

struct test_032
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sum_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{-9}, std::uint16_t{4});
    return !r;
  }

  static std::string desc()
  {
    return "unsigned result: -9 + 4u is impossible";
  }
};

struct test_033
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<std::int8_t>(std::int8_t{10}, std::int8_t{20});
    return r && (*r == std::int8_t{30});
  }

  static std::string desc()
  {
    return "public sum<int8_t>(10, 20) == 30";
  }
};

struct test_034
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<std::uint8_t>(std::uint8_t{10}, std::uint8_t{20});
    return r && (*r == std::uint8_t{30});
  }

  static std::string desc()
  {
    return "public sum<uint8_t>(10u, 20u) == 30";
  }
};

struct test_035
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<std::int16_t>(std::int8_t{-4}, std::uint16_t{9});
    return r && (*r == std::int16_t{5});
  }

  static std::string desc()
  {
    return "public sum<int16_t>(-4, 9u) == 5";
  }
};

struct test_036
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<std::uint16_t>(std::int8_t{-4}, std::uint16_t{9});
    return r && (*r == std::uint16_t{5});
  }

  static std::string desc()
  {
    return "public sum<uint16_t>(-4, 9u) == 5";
  }
};

struct test_037
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<std::uint16_t>(std::int8_t{-9}, std::uint16_t{4});
    return !r;
  }

  static std::string desc()
  {
    return "public sum<uint16_t>(-9, 4u) fails";
  }
};

struct test_038
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<float>(1.5, 2.25);
    return r && (*r > 3.74f) && (*r < 3.76f);
  }

  static std::string desc()
  {
    return "public sum<float>(1.5, 2.25) ~= 3.75";
  }
};

struct test_039
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<float>(std::numeric_limits<double>::max(), 1.0);
    return !r;
  }

  static std::string desc()
  {
    return "public sum<float>(double max, 1.0) fails on conversion overflow";
  }
};

struct test_040
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::sum<float>(std::numeric_limits<double>::quiet_NaN(), 1.0);
    return !r;
  }

  static std::string desc()
  {
    return "public sum<float>(NaN, 1.0) fails";
  }
};

struct test_041
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sum<float>(std::numeric_limits<float>::max(),
                                   std::numeric_limits<float>::max());
    return !r;
  }

  static std::string desc()
  {
    return "public sum<float>(float max, float max) fails on operation "
           "overflow";
  }
};

struct test_042
{
  bool operator()(const program::bus::options &)
  {
    return bus::internal::fits<std::int16_t>(std::int8_t{120});
  }

  static std::string desc()
  {
    return "fits<int16_t>(int8_t{120})";
  }
};

struct test_043
{

  bool operator()(const program::bus::options &)
  {
    static_assert(math::bus::sum<std::int8_t>(10, 20).value() == 30);
    return true;
  }

  static std::string desc()
  {
    return "tnct::math::bus::sum<std::int8_t>(10, 20).value() == 30 in compile "
           "time";
  }
};

} // namespace tnct::math::test

#endif

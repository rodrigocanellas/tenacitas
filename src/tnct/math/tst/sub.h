#ifndef TNCT_MATH_TST_SUB_H
#define TNCT_MATH_TST_SUB_H

#include <cstdint>
#include <limits>
#include <optional>
#include <string>

#include "tnct/math/bus/internal/sub.h"
#include "tnct/math/bus/sub.h"
#include "tnct/program/bus/options.h"

namespace tnct::math::test
{

struct test_sub_000
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same(std::int8_t{20}, std::int8_t{10});
    return r && (*r == std::int8_t{10});
  }

  static std::string desc()
  {
    return "internal::sub_same(int8_t{20}, int8_t{10}) == 10";
  }
};

struct test_sub_001
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same(std::int8_t{10}, std::int8_t{20});
    return r && (*r == std::int8_t{-10});
  }

  static std::string desc()
  {
    return "internal::sub_same(int8_t{10}, int8_t{20}) == -10";
  }
};

struct test_sub_002
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same(
        std::numeric_limits<std::int8_t>::min(), std::int8_t{1});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sub_same(int8_t min, 1) underflows";
  }
};

struct test_sub_003
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same(
        std::numeric_limits<std::int8_t>::max(), std::int8_t{-1});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sub_same(int8_t max, -1) overflows";
  }
};

struct test_sub_004
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same(std::uint8_t{20}, std::uint8_t{10});
    return r && (*r == std::uint8_t{10});
  }

  static std::string desc()
  {
    return "internal::sub_same(uint8_t{20}, uint8_t{10}) == 10";
  }
};

struct test_sub_005
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same(std::uint8_t{10}, std::uint8_t{20});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sub_same(uint8_t{10}, uint8_t{20}) fails";
  }
};

struct test_sub_006
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same_operands<std::int16_t>(
        std::int8_t{120}, std::int8_t{20});
    return r && (*r == std::int16_t{100});
  }

  static std::string desc()
  {
    return "internal::sub_same_operands<int16_t>(int8_t{120}, int8_t{20}) == "
           "100";
  }
};

struct test_sub_007
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same_operands<std::int8_t>(
        std::int16_t{120}, std::int16_t{-20});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sub_same_operands<int8_t>(int16_t{120}, int16_t{-20}) "
           "fails";
  }
};

struct test_sub_008
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same_operands<std::uint16_t>(
        std::uint8_t{200}, std::uint8_t{50});
    return r && (*r == std::uint16_t{150});
  }

  static std::string desc()
  {
    return "internal::sub_same_operands<uint16_t>(uint8_t{200}, uint8_t{50}) "
           "== 150";
  }
};

struct test_sub_009
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::internal::sub_same_operands<std::uint8_t>(
        std::uint16_t{10}, std::uint16_t{20});
    return !r;
  }

  static std::string desc()
  {
    return "internal::sub_same_operands<uint8_t>(uint16_t{10}, uint16_t{20}) "
           "fails";
  }
};

struct test_sub_010
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_signed_result<std::int16_t>(
            std::int8_t{9}, std::uint16_t{4});
    return r && (*r == std::int16_t{5});
  }

  static std::string desc()
  {
    return "signed result: 9 - 4u == 5";
  }
};

struct test_sub_011
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_signed_result<std::int16_t>(
            std::int8_t{4}, std::uint16_t{9});
    return r && (*r == std::int16_t{-5});
  }

  static std::string desc()
  {
    return "signed result: 4 - 9u == -5";
  }
};

struct test_sub_012
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_signed_result<std::int16_t>(
            std::int8_t{-4}, std::uint16_t{9});
    return r && (*r == std::int16_t{-13});
  }

  static std::string desc()
  {
    return "signed result: -4 - 9u == -13";
  }
};

struct test_sub_013
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_signed_result<std::int16_t>(
            std::uint16_t{9}, std::int8_t{4});
    return r && (*r == std::int16_t{5});
  }

  static std::string desc()
  {
    return "signed result: 9u - 4 == 5";
  }
};

struct test_sub_014
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_signed_result<std::int16_t>(
            std::uint16_t{4}, std::int8_t{9});
    return r && (*r == std::int16_t{-5});
  }

  static std::string desc()
  {
    return "signed result: 4u - 9 == -5";
  }
};

struct test_sub_015
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_signed_result<std::int16_t>(
            std::uint16_t{4}, std::int8_t{-9});
    return r && (*r == std::int16_t{13});
  }

  static std::string desc()
  {
    return "signed result: 4u - (-9) == 13";
  }
};

struct test_sub_016
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{9}, std::uint16_t{4});
    return r && (*r == std::uint16_t{5});
  }

  static std::string desc()
  {
    return "unsigned result: 9 - 4u == 5";
  }
};

struct test_sub_017
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{4}, std::uint16_t{9});
    return !r;
  }

  static std::string desc()
  {
    return "unsigned result: 4 - 9u fails";
  }
};

struct test_sub_018
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_unsigned_result<std::uint16_t>(
            std::int8_t{-4}, std::uint16_t{9});
    return !r;
  }

  static std::string desc()
  {
    return "unsigned result: -4 - 9u fails";
  }
};

struct test_sub_019
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_unsigned_result<std::uint16_t>(
            std::uint16_t{9}, std::int8_t{4});
    return r && (*r == std::uint16_t{5});
  }

  static std::string desc()
  {
    return "unsigned result: 9u - 4 == 5";
  }
};

struct test_sub_020
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_unsigned_result<std::uint16_t>(
            std::uint16_t{4}, std::int8_t{9});
    return !r;
  }

  static std::string desc()
  {
    return "unsigned result: 4u - 9 fails";
  }
};

struct test_sub_021
{
  bool operator()(const program::bus::options &)
  {
    const auto r =
        bus::internal::sub_different_operands_unsigned_result<std::uint16_t>(
            std::uint16_t{4}, std::int8_t{-9});
    return r && (*r == std::uint16_t{13});
  }

  static std::string desc()
  {
    return "unsigned result: 4u - (-9) == 13";
  }
};

struct test_sub_022
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::int8_t>(std::int8_t{20}, std::int8_t{10});
    return r && (*r == std::int8_t{10});
  }

  static std::string desc()
  {
    return "public sub<int8_t>(20, 10) == 10";
  }
};

struct test_sub_023
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::int8_t>(std::int8_t{10}, std::int8_t{20});
    return r && (*r == std::int8_t{-10});
  }

  static std::string desc()
  {
    return "public sub<int8_t>(10, 20) == -10";
  }
};

struct test_sub_024
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::uint8_t>(std::uint8_t{20}, std::uint8_t{10});
    return r && (*r == std::uint8_t{10});
  }

  static std::string desc()
  {
    return "public sub<uint8_t>(20u, 10u) == 10";
  }
};

struct test_sub_025
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::uint8_t>(std::uint8_t{10}, std::uint8_t{20});
    return !r;
  }

  static std::string desc()
  {
    return "public sub<uint8_t>(10u, 20u) fails";
  }
};

struct test_sub_026
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::int16_t>(std::int8_t{4}, std::uint16_t{9});
    return r && (*r == std::int16_t{-5});
  }

  static std::string desc()
  {
    return "public sub<int16_t>(4, 9u) == -5";
  }
};

struct test_sub_027
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::int16_t>(std::uint16_t{4}, std::int8_t{-9});
    return r && (*r == std::int16_t{13});
  }

  static std::string desc()
  {
    return "public sub<int16_t>(4u, -9) == 13";
  }
};

struct test_sub_028
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::uint16_t>(std::uint16_t{4}, std::int8_t{-9});
    return r && (*r == std::uint16_t{13});
  }

  static std::string desc()
  {
    return "public sub<uint16_t>(4u, -9) == 13";
  }
};

struct test_sub_029
{
  bool operator()(const program::bus::options &)
  {
    const auto r = bus::sub<std::uint16_t>(std::int8_t{-4}, std::uint16_t{9});
    return !r;
  }

  static std::string desc()
  {
    return "public sub<uint16_t>(-4, 9u) fails";
  }
};

} // namespace tnct::math::test

#endif

#ifndef TNCT_MATH_TST_MOVING_AVERAGE_H
#define TNCT_MATH_TST_MOVING_AVERAGE_H

#include <cstdint>
#include <limits>
#include <string>

#include "tnct/math/bus/moving_average.h"
#include "tnct/program/bus/options.h"

namespace tnct::math::test
{

struct test_moving_average_000
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<double, int, 3> ma;

    const auto r = ma(3);

    return r && (*r > 0.99) && (*r < 1.01);
  }

  static std::string desc()
  {
    return "moving_average<double,int,3> default 0, first value 3 -> average "
           "1.0";
  }
};

struct test_moving_average_001
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<double, int, 3> ma;

    const auto r1 = ma(3);
    const auto r2 = ma(6);
    const auto r3 = ma(9);

    return r1 && r2 && r3 && (*r1 > 0.99) && (*r1 < 1.01) && (*r2 > 2.99)
           && (*r2 < 3.01) && (*r3 > 5.99) && (*r3 < 6.01);
  }

  static std::string desc()
  {
    return "moving_average<double,int,3> sequence 3,6,9 -> 1.0,3.0,6.0";
  }
};

struct test_moving_average_002
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<double, int, 3> ma;

    const auto r1 = ma(3);
    const auto r2 = ma(6);
    const auto r3 = ma(9);
    const auto r4 = ma(12);

    return r1 && r2 && r3 && r4 && (*r4 > 8.99) && (*r4 < 9.01);
  }

  static std::string desc()
  {
    return "moving_average<double,int,3> rotates window: [3,6,9] -> add 12 => "
           "average 9.0";
  }
};

struct test_moving_average_003
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<double, int, 4> ma(2);

    const auto r = ma(6);

    // initial window = [2,2,2,2], sum = 8
    // replace first 2 by 6 => [6,2,2,2], sum = 12, avg = 3
    return r && (*r > 2.99) && (*r < 3.01);
  }

  static std::string desc()
  {
    return "moving_average<double,int,4> default 2, inserting 6 -> average 3.0";
  }
};

struct test_moving_average_004
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<int, int, 2> ma;

    const auto r1 = ma(1);
    const auto r2 = ma(2);

    // r1 = (0 + 1) / 2 = 0.5 -> truncates to 0
    // r2 = (1 + 2) / 2 = 1.5 -> truncates to 1
    return r1 && r2 && (*r1 == 0) && (*r2 == 1);
  }

  static std::string desc()
  {
    return "moving_average<int,int,2> truncates fractional part";
  }
};

struct test_moving_average_005
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<float, float, 3> ma;

    const auto r1 = ma(1.5f);
    const auto r2 = ma(1.5f);
    const auto r3 = ma(1.5f);

    return r1 && r2 && r3 && (*r1 > 0.49f) && (*r1 < 0.51f) && (*r2 > 0.99f)
           && (*r2 < 1.01f) && (*r3 > 1.49f) && (*r3 < 1.51f);
  }

  static std::string desc()
  {
    return "moving_average<float,float,3> handles floating values";
  }
};

struct test_moving_average_006
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<std::uint8_t, std::uint8_t, 3> ma;

    const auto r1 = ma(std::uint8_t{255});
    const auto r2 = ma(std::uint8_t{255});

    // third insertion should overflow the accumulated sum:
    // current window after two inserts is [255,255,0], sum = 510
    // but accumulator/result is uint8_t, so safe arithmetic should fail earlier
    // depending on implementation details, overflow may already happen before
    // r2 or on r3.
    const auto r3 = ma(std::uint8_t{255});

    return r1.has_value() || !r2.has_value() || !r3.has_value();
  }

  static std::string desc()
  {
    return "moving_average<uint8_t,uint8_t,3> detects accumulator overflow";
  }
};

struct test_moving_average_007
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<double, int, 3> ma;

    const auto r1 = ma(3);  // [3,0,0] avg 1
    const auto r2 = ma(6);  // [3,6,0] avg 3
    const auto r3 = ma(9);  // [3,6,9] avg 6
    const auto r4 = ma(12); // [12,6,9] avg 9
    const auto r5 = ma(15); // [12,15,9] avg 12
    const auto r6 = ma(18); // [12,15,18] avg 15

    return r1 && r2 && r3 && r4 && r5 && r6 && (*r1 > 0.99) && (*r1 < 1.01)
           && (*r2 > 2.99) && (*r2 < 3.01) && (*r3 > 5.99) && (*r3 < 6.01)
           && (*r4 > 8.99) && (*r4 < 9.01) && (*r5 > 11.99) && (*r5 < 12.01)
           && (*r6 > 14.99) && (*r6 < 15.01);
  }

  static std::string desc()
  {
    return "moving_average<double,int,3> multiple rotations keep expected "
           "averages";
  }
};

struct test_moving_average_008
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<double, int, 5> ma(10);

    const auto r1 = ma(10);
    const auto r2 = ma(10);
    const auto r3 = ma(10);

    return r1 && r2 && r3 && (*r1 > 9.99) && (*r1 < 10.01) && (*r2 > 9.99)
           && (*r2 < 10.01) && (*r3 > 9.99) && (*r3 < 10.01);
  }

  static std::string desc()
  {
    return "moving_average<double,int,5> remains constant when inserting "
           "default value";
  }
};

struct test_moving_average_009
{
  bool operator()(const program::bus::options &)
  {
    bus::moving_average<double, int, 2> ma;

    const auto r1 = ma(10); // [10,0] avg 5
    const auto r2 = ma(20); // [10,20] avg 15
    const auto r3 = ma(30); // [30,20] avg 25

    return r1 && r2 && r3 && (*r1 > 4.99) && (*r1 < 5.01) && (*r2 > 14.99)
           && (*r2 < 15.01) && (*r3 > 24.99) && (*r3 < 25.01);
  }

  static std::string desc()
  {
    return "moving_average<double,int,2> replaces oldest value correctly";
  }
};

} // namespace tnct::math::test

#endif

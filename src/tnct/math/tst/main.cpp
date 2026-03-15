/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/math/tst/moving_average.h"
#include "tnct/math/tst/mul.h"
#include "tnct/math/tst/sub.h"
#include "tnct/math/tst/sum.h"
#include "tnct/tester/bus/test.h"

int main(int argc, char **argv)
{
  using namespace tnct::math::test;

  tester::bus::test<> _test(argc, argv);

  run_test(_test, test_sum_000);
  run_test(_test, test_sum_001);
  run_test(_test, test_sum_002);
  run_test(_test, test_sum_003);
  run_test(_test, test_sum_004);
  run_test(_test, test_sum_005);
  run_test(_test, test_sum_006);
  run_test(_test, test_sum_007);
  run_test(_test, test_sum_008);
  run_test(_test, test_sum_009);
  run_test(_test, test_sum_010);
  run_test(_test, test_sum_011);
  run_test(_test, test_sum_012);
  run_test(_test, test_sum_013);
  run_test(_test, test_sum_014);
  run_test(_test, test_sum_015);
  run_test(_test, test_sum_016);
  run_test(_test, test_sum_017);
  run_test(_test, test_sum_018);
  run_test(_test, test_sum_019);
  run_test(_test, test_sum_020);
  run_test(_test, test_sum_021);
  run_test(_test, test_sum_022);
  run_test(_test, test_sum_023);
  run_test(_test, test_sum_024);
  run_test(_test, test_sum_025);
  run_test(_test, test_sum_026);
  run_test(_test, test_sum_027);
  run_test(_test, test_sum_028);
  run_test(_test, test_sum_029);
  run_test(_test, test_sum_030);
  run_test(_test, test_sum_031);
  run_test(_test, test_sum_032);
  run_test(_test, test_sum_033);
  run_test(_test, test_sum_034);
  run_test(_test, test_sum_035);
  run_test(_test, test_sum_036);
  run_test(_test, test_sum_037);
  run_test(_test, test_sum_038);
  run_test(_test, test_sum_039);
  run_test(_test, test_sum_040);
  run_test(_test, test_sum_041);
  run_test(_test, test_sum_042);
  run_test(_test, test_sum_043);

  run_test(_test, test_sub_000);
  run_test(_test, test_sub_001);
  run_test(_test, test_sub_002);
  run_test(_test, test_sub_003);
  run_test(_test, test_sub_004);
  run_test(_test, test_sub_005);
  run_test(_test, test_sub_006);
  run_test(_test, test_sub_007);
  run_test(_test, test_sub_008);
  run_test(_test, test_sub_009);
  run_test(_test, test_sub_010);
  run_test(_test, test_sub_011);
  run_test(_test, test_sub_012);
  run_test(_test, test_sub_013);
  run_test(_test, test_sub_014);
  run_test(_test, test_sub_015);
  run_test(_test, test_sub_016);
  run_test(_test, test_sub_017);
  run_test(_test, test_sub_018);
  run_test(_test, test_sub_019);
  run_test(_test, test_sub_020);
  run_test(_test, test_sub_021);
  run_test(_test, test_sub_022);
  run_test(_test, test_sub_023);
  run_test(_test, test_sub_024);
  run_test(_test, test_sub_025);
  run_test(_test, test_sub_026);
  run_test(_test, test_sub_027);
  run_test(_test, test_sub_028);
  run_test(_test, test_sub_029);

  run_test(_test, test_mul_000);
  run_test(_test, test_mul_001);
  run_test(_test, test_mul_002);
  run_test(_test, test_mul_003);
  run_test(_test, test_mul_004);
  run_test(_test, test_mul_005);
  run_test(_test, test_mul_006);
  run_test(_test, test_mul_007);
  run_test(_test, test_mul_008);
  run_test(_test, test_mul_009);
  run_test(_test, test_mul_010);
  run_test(_test, test_mul_011);
  run_test(_test, test_mul_012);
  run_test(_test, test_mul_013);
  run_test(_test, test_mul_014);
  run_test(_test, test_mul_015);
  run_test(_test, test_mul_016);
  run_test(_test, test_mul_017);
  run_test(_test, test_mul_018);
  run_test(_test, test_mul_019);
  run_test(_test, test_mul_020);
  run_test(_test, test_mul_021);

  run_test(_test, test_moving_average_000);
  run_test(_test, test_moving_average_001);
  run_test(_test, test_moving_average_002);
  run_test(_test, test_moving_average_003);
  run_test(_test, test_moving_average_004);
  run_test(_test, test_moving_average_005);
  run_test(_test, test_moving_average_006);
  run_test(_test, test_moving_average_007);
  run_test(_test, test_moving_average_008);
  run_test(_test, test_moving_average_009);
}

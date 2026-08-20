/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/tester/bus/test.h"

#include "tnct/container/tst/chunked_container_test.h"
#include "tnct/container/tst/circular_queue_test.h"
#include "tnct/container/tst/cpt_test.h"
#include "tnct/container/tst/matrix_test.h"
#include "tnct/container/tst/multi_index_cpt_test.h"
#include "tnct/container/tst/multi_index_test.h"
#include "tnct/container/tst/multiply_matrix_row_test.h"
#include "tnct/container/tst/multiply_matrix_test.h"

using namespace tnct;

int main(int argc, char **argv) {

  tester::bus::test _tester(argc, argv);
  run_test(_tester, container::tst::circular_queue_001);
  run_test(_tester, container::tst::circular_queue_003);
  // run_test(_tester, container::tst::circular_queue_test);

  run_test(_tester, container::tst::matrix_000);
  run_test(_tester, container::tst::matrix_001);
  run_test(_tester, container::tst::matrix_002);
  run_test(_tester, container::tst::matrix_003);
  run_test(_tester, container::tst::matrix_004);

  run_test(_tester, container::tst::cpt_test_000);
  run_test(_tester, container::tst::cpt_test_001);
  run_test(_tester, container::tst::cpt_test_002);

  run_test(_tester, container::tst::multiply_matrix_000);
  run_test(_tester, container::tst::multiply_matrix_001);

  run_test(_tester, container::tst::multiply_matrix_row_000);

  // run_test(_tester, container::tst::multi_index_001);
  // run_test(_tester, container::tst::multi_index_002);
  // run_test(_tester, container::tst::multi_index_003);
  // run_test(_tester, container::tst::multi_index_004);
  // run_test(_tester, container::tst::multi_index_005);
  // run_test(_tester, container::tst::multi_index_006);
  // run_test(_tester, container::tst::multi_index_007);
  // run_test(_tester, container::tst::multi_index_008);
  // run_test(_tester, container::tst::multi_index_009);
  // run_test(_tester, container::tst::multi_index_010);
  // run_test(_tester, container::tst::multi_index_011);
  // run_test(_tester, container::tst::multi_index_012);
  // run_test(_tester, container::tst::multi_index_013);
  // run_test(_tester, container::tst::multi_index_014);
  // run_test(_tester, container::tst::multi_index_015);

  run_test(_tester, container::tst::chunked_container_001);
  run_test(_tester, container::tst::chunked_container_002);
  run_test(_tester, container::tst::chunked_container_003);
  run_test(_tester, container::tst::chunked_container_004);
  run_test(_tester, container::tst::chunked_container_005);
  run_test(_tester, container::tst::chunked_container_006);
  run_test(_tester, container::tst::chunked_container_007);
  run_test(_tester, container::tst::chunked_container_008);
  run_test(_tester, container::tst::chunked_container_009);
  run_test(_tester, container::tst::chunked_container_010);
  run_test(_tester, container::tst::chunked_container_011);
  run_test(_tester, container::tst::chunked_container_012);
  run_test(_tester, container::tst::chunked_container_013);
  run_test(_tester, container::tst::chunked_container_014);
  run_test(_tester, container::tst::chunked_container_015);
  run_test(_tester, container::tst::chunked_container_016);
  run_test(_tester, container::tst::chunked_container_017);
  run_test(_tester, container::tst::chunked_container_018);
  run_test(_tester, container::tst::chunked_container_019);
  run_test(_tester, container::tst::chunked_container_020);
  run_test(_tester, container::tst::chunked_container_021);
  run_test(_tester, container::tst::chunked_container_022);
  run_test(_tester, container::tst::chunked_container_023);
  run_test(_tester, container::tst::chunked_container_024);
  // run_test(_tester, container::tst::chunked_container_025);
  run_test(_tester, container::tst::chunked_container_026);
  run_test(_tester, container::tst::chunked_container_027);
  run_test(_tester, container::tst::chunked_container_028);

  // run_test(_tester, container::tst::multi_index_cpt_000);
  // run_test(_tester, container::tst::multi_index_cpt_001);
  // run_test(_tester, container::tst::multi_index_cpt_002);
  // run_test(_tester, container::tst::multi_index_cpt_003);
  // run_test(_tester, container::tst::multi_index_cpt_004);
  // run_test(_tester, container::tst::multi_index_cpt_005);
  // run_test(_tester, container::tst::multi_index_cpt_006);
  // run_test(_tester, container::tst::multi_index_cpt_007);
  // run_test(_tester, container::tst::multi_index_cpt_008);
  // run_test(_tester, container::tst::multi_index_cpt_009);
  // run_test(_tester, container::tst::multi_index_cpt_010);
  // run_test(_tester, container::tst::multi_index_cpt_011);
  // run_test(_tester, container::tst::multi_index_cpt_012);
  // run_test(_tester, container::tst::multi_index_cpt_013);
  // run_test(_tester, container::tst::multi_index_cpt_014);
  // run_test(_tester, container::tst::multi_index_cpt_015);
}

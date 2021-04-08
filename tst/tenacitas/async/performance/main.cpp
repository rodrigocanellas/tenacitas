/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concurrent/performance/thread_pool_tester.h>
#include <concurrent/thread_pool.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

struct thread_pool_000 {
  bool operator()() {
    thread_pool_tester<2, 1, 300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 1, 300 work sleep"; }
};

struct thread_pool_001 {
  bool operator()() {
    thread_pool_tester<2, 1, 2800>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 1, 2800 work sleep"; }
};

struct thread_pool_002 {
  bool operator()() {
    thread_pool_tester<2, 1, 5300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 1, 5300 work sleep"; }
};

struct thread_pool_003 {
  bool operator()() {
    thread_pool_tester<2, 10, 300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 10, 300 work sleep"; }
};

struct thread_pool_004 {
  bool operator()() {
    thread_pool_tester<2, 10, 2800>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 10, 2800 work sleep"; }
};

struct thread_pool_005 {
  bool operator()() {
    thread_pool_tester<2, 10, 5300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 10, 5300 work sleep"; }
};

struct thread_pool_006 {
  bool operator()() {
    thread_pool_tester<2, 100, 300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 100, 300 work sleep"; }
};

struct thread_pool_007 {
  bool operator()() {
    thread_pool_tester<2, 100, 2800>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 100, 2800 work sleep"; }
};

struct thread_pool_008 {
  bool operator()() {
    thread_pool_tester<2, 100, 5300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 100, 5300 work sleep"; }
};

struct thread_pool_009 {
  bool operator()() {
    thread_pool_tester<2, 1000, 300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 1000, 300 work sleep"; }
};

struct thread_pool_010 {
  bool operator()() {
    thread_pool_tester<2, 1000, 2800>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 1000, 2800 work sleep"; }
};

struct thread_pool_011 {
  bool operator()() {
    thread_pool_tester<2, 1000, 5300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 1000, 5300 work sleep"; }
};

struct thread_pool_012 {
  bool operator()() {
    thread_pool_tester<2, 10000, 300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 10000, 300 work sleep"; }
};

struct thread_pool_013 {
  bool operator()() {
    thread_pool_tester<2, 10000, 2800>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 10000, 2800 work sleep"; }
};

struct thread_pool_014 {
  bool operator()() {
    thread_pool_tester<2, 10000, 5300>()();
    return true;
  }

  static std::string desc() { return "2 consumers, 10000, 5300 work sleep"; }
};

struct thread_pool_015 {
  bool operator()() {
    thread_pool_tester<7, 1, 300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 1, 300 work sleep"; }
};

struct thread_pool_016 {
  bool operator()() {
    thread_pool_tester<7, 1, 2800>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 1, 2800 work sleep"; }
};

struct thread_pool_017 {
  bool operator()() {
    thread_pool_tester<7, 1, 5300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 1, 5300 work sleep"; }
};

struct thread_pool_018 {
  bool operator()() {
    thread_pool_tester<7, 10, 300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 10, 300 work sleep"; }
};

struct thread_pool_019 {
  bool operator()() {
    thread_pool_tester<7, 10, 2800>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 10, 2800 work sleep"; }
};

struct thread_pool_020 {
  bool operator()() {
    thread_pool_tester<7, 10, 5300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 10, 5300 work sleep"; }
};

struct thread_pool_021 {
  bool operator()() {
    thread_pool_tester<7, 100, 300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 100, 300 work sleep"; }
};

struct thread_pool_022 {
  bool operator()() {
    thread_pool_tester<7, 100, 2800>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 100, 2800 work sleep"; }
};

struct thread_pool_023 {
  bool operator()() {
    thread_pool_tester<7, 100, 5300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 100, 5300 work sleep"; }
};

struct thread_pool_024 {
  bool operator()() {
    thread_pool_tester<7, 1000, 300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 1000, 300 work sleep"; }
};

struct thread_pool_025 {
  bool operator()() {
    thread_pool_tester<7, 1000, 2800>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 1000, 2800 work sleep"; }
};

struct thread_pool_026 {
  bool operator()() {
    thread_pool_tester<7, 1000, 5300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 1000, 5300 work sleep"; }
};

struct thread_pool_027 {
  bool operator()() {
    thread_pool_tester<7, 10000, 300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 10000, 300 work sleep"; }
};

struct thread_pool_028 {
  bool operator()() {
    thread_pool_tester<7, 10000, 2800>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 10000, 2800 work sleep"; }
};

struct thread_pool_029 {
  bool operator()() {
    thread_pool_tester<7, 10000, 5300>()();
    return true;
  }

  static std::string desc() { return "7 consumers, 10000, 5300 work sleep"; }
};

struct thread_pool_030 {
  bool operator()() {
    thread_pool_tester<12, 1, 300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 1, 300 work sleep"; }
};

struct thread_pool_031 {
  bool operator()() {
    thread_pool_tester<12, 1, 2800>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 1, 2800 work sleep"; }
};

struct thread_pool_032 {
  bool operator()() {
    thread_pool_tester<12, 1, 5300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 1, 5300 work sleep"; }
};

struct thread_pool_033 {
  bool operator()() {
    thread_pool_tester<12, 10, 300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 10, 300 work sleep"; }
};

struct thread_pool_034 {
  bool operator()() {
    thread_pool_tester<12, 10, 2800>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 10, 2800 work sleep"; }
};

struct thread_pool_035 {
  bool operator()() {
    thread_pool_tester<12, 10, 5300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 10, 5300 work sleep"; }
};

struct thread_pool_036 {
  bool operator()() {
    thread_pool_tester<12, 100, 300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 100, 300 work sleep"; }
};

struct thread_pool_037 {
  bool operator()() {
    thread_pool_tester<12, 100, 2800>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 100, 2800 work sleep"; }
};
struct thread_pool_038 {
  bool operator()() {
    thread_pool_tester<12, 100, 5300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 100, 5300 work sleep"; }
};

struct thread_pool_039 {
  bool operator()() {
    thread_pool_tester<12, 1000, 300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 1000, 300 work sleep"; }
};

struct thread_pool_040 {
  bool operator()() {
    thread_pool_tester<12, 1000, 2800>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 1000, 2800 work sleep"; }
};

struct thread_pool_041 {
  bool operator()() {
    thread_pool_tester<12, 1000, 5300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 1000, 5300 work sleep"; }
};

struct thread_pool_042 {
  bool operator()() {
    thread_pool_tester<12, 10000, 300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 10000, 300 work sleep"; }
};

struct thread_pool_043 {
  bool operator()() {
    thread_pool_tester<12, 10000, 2800>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 10000, 2800 work sleep"; }
};

struct thread_pool_044 {
  bool operator()() {
    thread_pool_tester<12, 10000, 5300>()();
    return true;
  }

  static std::string desc() { return "12 consumers, 10000, 5300 work sleep"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _tester(argc, argv);

  run_test(_tester, thread_pool_000);
  run_test(_tester, thread_pool_001);
  run_test(_tester, thread_pool_002);
  run_test(_tester, thread_pool_003);
  run_test(_tester, thread_pool_004);
  run_test(_tester, thread_pool_005);
  run_test(_tester, thread_pool_006);
  run_test(_tester, thread_pool_007);
  run_test(_tester, thread_pool_008);
  run_test(_tester, thread_pool_009);
  run_test(_tester, thread_pool_010);
  run_test(_tester, thread_pool_011);
  run_test(_tester, thread_pool_012);
  run_test(_tester, thread_pool_013);
  run_test(_tester, thread_pool_015);
  run_test(_tester, thread_pool_016);
  run_test(_tester, thread_pool_017);
  run_test(_tester, thread_pool_018);
  run_test(_tester, thread_pool_019);
  run_test(_tester, thread_pool_020);
  run_test(_tester, thread_pool_021);
  run_test(_tester, thread_pool_022);
  run_test(_tester, thread_pool_023);
  run_test(_tester, thread_pool_024);
  run_test(_tester, thread_pool_025);
  run_test(_tester, thread_pool_026);
  run_test(_tester, thread_pool_027);
  run_test(_tester, thread_pool_028);
  run_test(_tester, thread_pool_029);
  run_test(_tester, thread_pool_030);
  run_test(_tester, thread_pool_031);
  run_test(_tester, thread_pool_032);
  run_test(_tester, thread_pool_033);
  run_test(_tester, thread_pool_034);
  run_test(_tester, thread_pool_035);
  run_test(_tester, thread_pool_036);
  run_test(_tester, thread_pool_037);
  run_test(_tester, thread_pool_038);
  run_test(_tester, thread_pool_039);
  run_test(_tester, thread_pool_040);
  run_test(_tester, thread_pool_041);
  run_test(_tester, thread_pool_042);
  run_test(_tester, thread_pool_043);
  run_test(_tester, thread_pool_044);
}

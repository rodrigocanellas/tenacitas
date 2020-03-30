#include <concurrent/bus/thread_pool.h>
#include <concurrent/tst/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>
#include <tester/bus/test.h>

struct thread_pool_008 {
  bool operator()() {
    thread_pool_tester<2, 100, 5300>()();
    return true;
  }

 static std::string desc() { return "2 consumers, 100, 5300 work sleep";   }

  static std::string name() { return "thread_pool_008"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<thread_pool_008>(argc, argv);
}

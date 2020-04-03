#include <concurrent/_bus/thread_pool.h>
#include <concurrent/_tst/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

struct thread_pool_044 {
  bool operator()() {
    thread_pool_tester<12, 10000, 5300>()();
    return true;
  }

 static std::string desc() { return "12 consumers, 10000, 5300 work sleep";   }

  static std::string name() { return "thread_pool_044"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<thread_pool_044>(argc, argv);
}

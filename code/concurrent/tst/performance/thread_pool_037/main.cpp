#include <concurrent/bus/thread_pool.h>
#include <concurrent/tst/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>
#include <tester/bus/test.h>

struct thread_pool_037 {
  bool operator()() {
    thread_pool_tester<12, 100, 2800>()();
    return true;
  }

 static std::string desc() { return "12 consumers, 100, 2800 work sleep";   }

  static std::string name() { return "thread_pool_037"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::bus::test::run<thread_pool_037>(argc, argv);
}

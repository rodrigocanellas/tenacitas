#include <concurrent/thread_pool.h>
#include <concurrent/_tst/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

struct thread_pool_010 {
  bool operator()() {
    thread_pool_tester<2, 1000, 2800>()();
    return true;
  }

 static std::string desc() { return "2 consumers, 1000, 2800 work sleep";   }

  static std::string name() { return "thread_pool_010"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<thread_pool_010>(argc, argv);
}

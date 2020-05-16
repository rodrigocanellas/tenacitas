#include <concurrent/thread_pool.h>
#include <concurrent/_tst/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

struct thread_pool_021 {
  bool operator()() {
    thread_pool_tester<7, 100, 300>()();
    return true;
  }

 static std::string desc() { return "7 consumers, 100, 300 work sleep";   }

  static std::string name() { return "thread_pool_021"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<thread_pool_021>(argc, argv);
}

#include <concurrent/_bus/thread_pool.h>
#include <concurrent/_tst/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

struct thread_pool_007 {
  bool operator()() {
    thread_pool_tester<2, 100, 2800>()();
    return true;
  }

 static std::string desc() { return "2 consumers, 100, 2800 work sleep";   }

  static std::string name() { return "thread_pool_007"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<thread_pool_007>(argc, argv);
}

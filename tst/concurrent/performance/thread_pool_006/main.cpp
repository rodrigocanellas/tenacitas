#include <concurrent/_tst/performance/thread_pool_tester.h>
#include <concurrent/thread_pool.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<thread_pool_006>(argc, argv);
}

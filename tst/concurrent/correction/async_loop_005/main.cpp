
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/internal/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct work1 {
  concurrent::_bus::work_status operator()(uint32_t &&p_data) {

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    concurrent_log_debug(logger::cerr::log, p_data);
    return concurrent::_bus::work_status::dont_stop;
  }
};

struct provide {
  std::pair<bool, uint32_t> operator()() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    if (++m_data > 830) {
      return std::make_pair(false, 0);
    }
    return std::make_pair(true, m_data);
  }
  uint32_t m_data = 0;
};

typedef concurrent::_bus::async_loop_t<uint32_t, logger::cerr::log> async_loop;

struct async_loop_005 {
  bool operator()() {
    using namespace tenacitas;
    try {
      async_loop sl1(work1(), std::chrono::milliseconds(1000),
                     []() { return concurrent::_bus::work_status::dont_stop; },
                     provide());

      // uncomment the line below for a compiler error "error:
      // ‘tenacitas::concurrent::_bus::_bus::async_loop<t_timeout,
      // t_data>::async_loop(tenacitas::concurrent::_bus::_bus::async_loop<t_timeout,
      // t_data>::work_t&&, t_timeout&&,
      // tenacitas::concurrent::_bus::_bus::async_loop<t_timeout,
      // t_data>::control_t&&,
      // tenacitas::concurrent::_bus::_bus::async_loop<t_timeout,
      // t_data>::provide_t&&) [with t_timeout =
      // std::chrono::duration<long int, std::ratio<1, 1000> >; t_data =
      // unsigned int;
      // tenacitas::concurrent::_bus::_bus::async_loop<t_timeout,
      // t_data>::work_t = std::function<bool(unsigned int&&)>;
      // tenacitas::concurrent::_bus::_bus::async_loop<t_timeout,
      // t_data>::control_t = std::function<bool()>;
      // tenacitas::concurrent::_bus::_bus::async_loop<t_timeout,
      // t_data>::provide_t = std::function<std::pair<bool, unsigned
      // int>()>]’ is private within this context"
      //            async_loop_t sl0(work1(),
      //                             std::chrono::milliseconds(1000),
      //                             []() { return true; },
      //                             provide());

      // uncomment the line below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::async_loop(const
      // tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&) [with t_work_ptr = std::shared_ptr<work1>;
      // t_time = std::chrono::duration<long int, std::ratio<1, 1000> >;
      // t_provide_ptr = std::shared_ptr<provide>]’"
      //            async_loop_t sl2(sl1);

      // *****************************************************************

      // uncomment the line below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::async_loop() [with t_work_ptr =
      // std::shared_ptr<work1>; t_time = std::chrono::duration<long int,
      // std::ratio<1, 1000> >; t_provide_ptr =
      // std::shared_ptr<provide>]’"
      //            async_loop_t sl3;

      // *****************************************************************

      // uncomment the two lines below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&
      // tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::operator=(const
      // tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&) [with t_work_ptr = std::shared_ptr<work1>;
      // t_time = std::chrono::duration<long int, std::ratio<1, 1000> >;
      // t_provide_ptr = std::shared_ptr<provide>]’"
      //            async_loop_t sl3(std::make_shared<work1>(),
      //                             std::chrono::milliseconds(1000),
      //                             []() { return true; },
      //                             std::make_shared<provide>(provide()));
      //            sl3 = sl1;

      // *****************************************************************

      // uncomment the line below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::async_loop(tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr,
      // t_time, t_provide_ptr>&&) [with t_work_ptr =
      // std::shared_ptr<work1>; t_time = std::chrono::duration<long int,
      // std::ratio<1, 1000> >; t_provide_ptr =
      // std::shared_ptr<provide>]’"
      //            async_loop_t sl4(std::move(sl1));

      // *****************************************************************

      // uncomment the two lines below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&
      // tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::operator=(tenacitas::concurrent::_bus::_bus::async_loop<t_work_ptr,
      // t_time, t_provide_ptr>&&) [with t_work_ptr =
      // std::shared_ptr<work1>; t_time = std::chrono::duration<long int,
      // std::ratio<1, 1000> >; t_provide_ptr =
      // std::shared_ptr<provide>]’"
      //            async_loop_t sl5(std::make_shared<work1>(),
      //                             std::chrono::milliseconds(1000),
      //                             []() { return true; },
      //                             std::make_shared<provide>(provide()));
      //            sl5 = std::move(sl1);

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string name() { return "async_loop_005"; }
  static std::string desc() {
    return "Testing compile failing for private constructor, default "
           "constructor, copy constructor, move constructor, copy assignment "
           "and move assignment of 'async_loop'";
  }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<async_loop_005>(argc, argv);
}
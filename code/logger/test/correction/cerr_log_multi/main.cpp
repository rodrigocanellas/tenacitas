
#include <iostream>

#include <business/concurrent/traits.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/logger/cerr.h>
#include <business/logger/cout.h>
#include <business/logger/log.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

typedef concurrent::sleeping_loop_t<void, logger::log> sleeping_loop;

#define log_test(p_params...)                                                  \
  logger::log::test("cout_log_multi", __LINE__, p_params)

#define log_debug(p_params...)                                                 \
  logger::log::debug(                                   \
  "cout_log_multi", __LINE__, p_params)

#define log_info(p_params...)                                                  \
  logger::log::info("cout_log_multi", __LINE__, p_params)

#define log_warn(p_params...)                                                  \
  logger::log::warn("cout_log_multi", __LINE__, p_params)

#define log_error(p_params...)                                                 \
  logger::log::error(                                   \
  "cout_log_multi", __LINE__, p_params)

#define log_fatal(p_params...)                                                 \
  logger::log::error(                                   \
  "cout_log_multi", __LINE__, p_params)

class cout_log_multi
{

public:
  bool operator()()
  {
    using namespace logger;
    try {
      configure_cout_log();
      log::set_debug();

      sleeping_loop _loop1(std::chrono::milliseconds(1000),
                           []() -> concurrent::result{
                             for (uint32_t _i = 0; _i < 1000; ++_i) {
                               log_debug("ola! ", 33);
                               log_debug("como vai? ", _i);
                               log_info("vou bem!! ", _i);
                               log_info("e vc? ", _i);
                               log_warn("ótimo! novo emprego! ", _i);
                               log_warn("que bom! ", _i);
                             }
                             return concurrent::result::dont_stop;
                           },
                           std::chrono::milliseconds(1000));

      sleeping_loop _loop2(std::chrono::milliseconds(500),
                           []() -> concurrent::result{
                             for (uint32_t _i = 0; _i < 1500; ++_i) {
                               log_debug("aaa! ", 33);
                               log_debug("bbb? ", _i);
                               log_info("ccc!! ", _i);
                               log_info("ddd ", _i);
                               log_warn("eee! ", _i);
                               log_warn("fff! ", _i);
                             }
                             return concurrent::result::dont_stop;
                           },
                           std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
            std::chrono::milliseconds(100),
            []() -> concurrent::result {
              for (uint32_t _i = 0; _i < 3000; ++_i) {
                log_debug("abcdefghijklmnopqrstivwxyz! ", 33);
                log_debug("abcdefghijklmnopqrstivwxyz? ", _i);
                log_info("abcdefghijklmnopqrstivwxyz!! ", _i);
                log_info("abcdefghijklmnopqrstivwxyz ", _i);
                log_warn("abcdefghijklmnopqrstivwxyz! ", _i);
                log_warn("abcdefghijklmnopqrstivwxyz! ", _i);
              }
              return concurrent::result::dont_stop;
            },
            std::chrono::milliseconds(1000));

      _loop1.run();
      _loop2.run();
      _loop3.run();

      log_debug("---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      log_debug("---- waking up");

      return true;
    } catch (std::exception& _ex) {
      log::fatal("ERRO log::log_multi: '", _ex.what(), "'");
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(cout_log_multi, argc, argv, "Multiple threads logging to 'cerr'");
}

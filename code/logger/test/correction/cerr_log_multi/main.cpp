
#include <iostream>

#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <logger/business/cout.h>
#include <tester/business/run.h>

typedef tenacitas::concurrent::business::sleeping_loop<void> sleeping_loop_t;

class cout_log_multi
{

  public:
    bool operator()()
    {
        try {
            tenacitas::logger::business::configure_cout_log();
            cout_set_debug();

            sleeping_loop_t _loop1(std::chrono::milliseconds(1000),
                                   []() {
                                       for (uint32_t _i = 0; _i < 1000; ++_i) {
                                           cout_debug("ola! ", 33);
                                           cout_debug("como vai? ", _i);
                                           cout_info("vou bem!! ", _i);
                                           cout_info("e vc? ", _i);
                                           cout_warn("ótimo! novo emprego! ",
                                                     _i);
                                           cout_warn("que bom! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            sleeping_loop_t _loop2(std::chrono::milliseconds(500),
                                   []() {
                                       for (uint32_t _i = 0; _i < 1500; ++_i) {
                                           cout_debug("aaa! ", 33);
                                           cout_debug("bbb? ", _i);
                                           cout_info("ccc!! ", _i);
                                           cout_info("ddd ", _i);
                                           cout_warn("eee! ", _i);
                                           cout_warn("fff! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            sleeping_loop_t _loop3(
              std::chrono::milliseconds(100),
              []() {
                  for (uint32_t _i = 0; _i < 3000; ++_i) {
                      cout_debug("abcdefghijklmnopqrstivwxyz! ", 33);
                      cout_debug("abcdefghijklmnopqrstivwxyz? ", _i);
                      cout_info("abcdefghijklmnopqrstivwxyz!! ", _i);
                      cout_info("abcdefghijklmnopqrstivwxyz ", _i);
                      cout_warn("abcdefghijklmnopqrstivwxyz! ", _i);
                      cout_warn("abcdefghijklmnopqrstivwxyz! ", _i);
                  }
                  return true;
              },
              std::chrono::milliseconds(1000));

            _loop1.run();
            _loop2.run();
            _loop3.run();

            cerr_debug("---- sleeping");
            std::this_thread::sleep_for(std::chrono::seconds(50));
            cerr_debug("---- waking up");

            return true;
        } catch (std::exception& _ex) {
            cerr_fatal("ERRO cout_log_multi: '", _ex.what(), "'");
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(cout_log_multi, argc, argv, "Multiple threads logging to 'cerr'");
}

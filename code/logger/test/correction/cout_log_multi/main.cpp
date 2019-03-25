/// \example
#include <iostream>

#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

typedef tenacitas::concurrent::business::sleeping_loop<void> sleeping_loop_t;

class cerr_log_multi
{

  public:
    cerr_log_multi() = default;

    bool operator()()
    {
        try {
            tenacitas::logger::business::configure_cerr_log();
            cerr_set_debug();

            sleeping_loop_t _loop1(std::chrono::milliseconds(1000),
                                   []() {
                                       for (uint32_t _i = 0; _i < 1000; ++_i) {
                                           cerr_debug("ola! ", 33);
                                           cerr_debug("como vai? ", _i);
                                           cerr_info("vou bem!! ", _i);
                                           cerr_info("e vc? ", _i);
                                           cerr_warn("ótimo! novo emprego! ",
                                                     _i);
                                           cerr_warn("que bom! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            sleeping_loop_t _loop2(std::chrono::milliseconds(500),
                                   []() {
                                       for (uint32_t _i = 0; _i < 1500; ++_i) {
                                           cerr_debug("aaa! ", 33);
                                           cerr_debug("bbb? ", _i);
                                           cerr_info("ccc!! ", _i);
                                           cerr_info("ddd ", _i);
                                           cerr_warn("eee! ", _i);
                                           cerr_warn("fff! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            sleeping_loop_t _loop3(
              std::chrono::milliseconds(100),
              []() {
                  for (uint32_t _i = 0; _i < 3000; ++_i) {
                      cerr_debug("abcdefghijklmnopqrstivwxyz! ", 33);
                      cerr_debug("abcdefghijklmnopqrstivwxyz? ", _i);
                      cerr_info("abcdefghijklmnopqrstivwxyz!! ", _i);
                      cerr_info("abcdefghijklmnopqrstivwxyz ", _i);
                      cerr_warn("abcdefghijklmnopqrstivwxyz! ", _i);
                      cerr_warn("abcdefghijklmnopqrstivwxyz! ", _i);
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
            std::cerr << "ERRO cerr_log_multi: '" << _ex.what() << "'"
                      << std::endl;
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(cerr_log_multi, argc, argv, "Multiple threads logging to 'cout'");
}

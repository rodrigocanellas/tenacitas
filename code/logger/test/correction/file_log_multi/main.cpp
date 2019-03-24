
#include <iostream>

#include <concurrent/bus/sleeping_loop.h>
#include <logger/bus/file.h>
#include <tester/bus/run.h>

typedef tenacitas::concurrent::bus::sleeping_loop<void> sleeping_loop_t;

class file_log_multi
{

  public:
    file_log_multi() = default;

    bool operator()()
    {
        try {
            tenacitas::logger::bus::configure_file_log(
              "./", "file_log_multi", 1 * 1024 * 1024, std::chrono::minutes(3));
            file_set_debug();

            sleeping_loop_t _loop1(std::chrono::milliseconds(1000),
                                   []() {
                                       for (uint32_t _i = 0; _i < 1000; ++_i) {
                                           file_debug("ola! ", 33);
                                           file_debug("como vai? ", _i);
                                           file_info("vou bem!! ", _i);
                                           file_info("e vc? ", _i);
                                           file_warn("ótimo! novo emprego! ",
                                                     _i);
                                           file_warn("que bom! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            sleeping_loop_t _loop2(std::chrono::milliseconds(500),
                                   []() {
                                       for (uint32_t _i = 0; _i < 1500; ++_i) {
                                           file_debug("aaa! ", 33);
                                           file_debug("bbb? ", _i);
                                           file_info("ccc!! ", _i);
                                           file_info("ddd ", _i);
                                           file_warn("eee! ", _i);
                                           file_warn("fff! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            sleeping_loop_t _loop3(
              std::chrono::milliseconds(100),
              []() {
                  for (uint32_t _i = 0; _i < 3000; ++_i) {
                      file_debug("abcdefghijklmnopqrstivwxyz! ", 33);
                      file_debug("abcdefghijklmnopqrstivwxyz? ", _i);
                      file_info("abcdefghijklmnopqrstivwxyz!! ", _i);
                      file_info("abcdefghijklmnopqrstivwxyz ", _i);
                      file_warn("abcdefghijklmnopqrstivwxyz! ", _i);
                      file_warn("abcdefghijklmnopqrstivwxyz! ", _i);
                  }
                  return true;
              },
              std::chrono::milliseconds(1000));

            _loop1.run();
            _loop2.run();
            _loop3.run();

            cerr_test("---- sleeping");
            std::this_thread::sleep_for(std::chrono::seconds(50));
            cerr_test("---- waking up");

            return true;
        } catch (std::exception& _ex) {
            cerr_fatal("ERRO file_log_multi: '", _ex.what(), "'");
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(file_log_multi, argc, argv, "Multiple threads logging to 'file'");
}

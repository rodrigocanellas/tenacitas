#include <iostream>
#include <sstream>
#include <string>

#include <concurrent/sleeping_loop.h>
#include <logger/cout/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct cout_log_creation {
  bool operator()() {

    try {

      logger::cout::log::set_debug();

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "'cout' log creation"; }
};

struct cout_log_how_to {
  bool operator()() {
    try {

      logger::cout::log::set_debug();

      logger::cout::log::debug("cout_how_to", __LINE__, "hello! ", 309);
      logger::cout::log::debug("cout_how_to", __LINE__, "how are you doing? ",
                               3.14);
      logger::cout::log::info("cout_how_to", __LINE__, "fine!! ", 'W');
      logger::cout::log::info("cout_how_to", __LINE__, "and you?");
      logger::cout::log::warn("cout_how_to", __LINE__,
                              "great! got a new job!! ", 6987.58f);
      logger::cout::log::warn("cout_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cout' log usage"; }
};

struct cout_log_single {

  bool operator()() {
    try {
      typedef concurrent::sleeping_loop_t<void, logger::cout::log>
          sleeping_loop;
      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            logger::cout::log::debug("cout_log_single", __LINE__,
                                     "================= work! ", time(nullptr));
            for (uint32_t _i = 0; _i < 5; ++_i) {
              logger::cout::log::debug("cout_log_single", __LINE__, "ola! ",
                                       _i);
              logger::cout::log::debug("cout_log_single", __LINE__,
                                       "como vai? ", _i);
              logger::cout::log::info("cout_log_single", __LINE__, "vou bem!! ",
                                      _i);
              logger::cout::log::info("cout_log_single", __LINE__, "e vc? ",
                                      _i);
              logger::cout::log::warn("cout_log_single", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::cout::log::warn("cout_log_single", __LINE__, "que bom! ",
                                      _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      _loop1.start();

      logger::cout::log::debug("cout_log_single", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::minutes(1));
      logger::cout::log::debug("cout_log_single", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO logger::cout::log::logger::cout::log_single: '"
                << _ex.what() << "'" << std::endl;
    }
    return false;
  }

  static std::string desc() {
    return "Single thread logger::cout::logging to 'cout'";
  }
};

class cout_log_multi {

public:
  cout_log_multi() = default;

  bool operator()() {
    try {

      typedef concurrent::sleeping_loop_t<void, logger::cout::log>
          sleeping_loop;

      logger::cout::log::set_debug();

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            for (uint32_t _i = 0; _i < 1000; ++_i) {
              logger::cout::log::debug("cerr_log_multi", __LINE__, "ola! ", 33);
              logger::cout::log::debug("cerr_log_multi", __LINE__, "como vai? ",
                                       _i);
              logger::cout::log::info("cerr_log_multi", __LINE__, "vou bem!! ",
                                      _i);
              logger::cout::log::info("cerr_log_multi", __LINE__, "e vc? ", _i);
              logger::cout::log::warn("cerr_log_multi", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::cout::log::warn("cerr_log_multi", __LINE__, "que bom! ",
                                      _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop2(
          std::chrono::milliseconds(500),
          []() {
            for (uint32_t _i = 0; _i < 1500; ++_i) {
              logger::cout::log::debug("cerr_log_multi", __LINE__, "aaa! ", 33);
              logger::cout::log::debug("cerr_log_multi", __LINE__, "bbb? ", _i);
              logger::cout::log::info("cerr_log_multi", __LINE__, "ccc!! ", _i);
              logger::cout::log::info("cerr_log_multi", __LINE__, "ddd ", _i);
              logger::cout::log::warn("cerr_log_multi", __LINE__, "eee! ", _i);
              logger::cout::log::warn("cerr_log_multi", __LINE__, "fff! ", _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
          std::chrono::milliseconds(100),
          []() {
            for (uint32_t _i = 0; _i < 3000; ++_i) {
              logger::cout::log::debug("cerr_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz! ", 33);
              logger::cout::log::debug("cerr_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz? ", _i);
              logger::cout::log::info("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz!! ", _i);
              logger::cout::log::info("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz ", _i);
              logger::cout::log::warn("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
              logger::cout::log::warn("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      _loop1.start();
      _loop2.start();
      _loop3.start();

      logger::cout::log::debug("cerr_log_multi", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      logger::cout::log::debug("cerr_log_multi", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO logger::cout::log::log_multi: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Multiple threads logging to 'cout'"; }
};

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);
  run_test(_tester, cout_log_creation);
  run_test(_tester, cout_log_how_to);
  run_test(_tester, cout_log_single);
  run_test(_tester, cout_log_multi);
}

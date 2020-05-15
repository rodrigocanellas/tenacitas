#include <iostream>
#include <sstream>
#include <string>

#include <concurrent/sleeping_loop.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

class cerr_log_creation {

public:
  bool operator()() {
    using namespace tenacitas;
    try {

      logger::cerr::log::log::set_debug();

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "'cerr' log creation"; }

  static std::string name() { return "cerr_log_creation"; }

private:
  std::string m_base_cout_name;
};

struct cerr_log_how_to {
  bool operator()() {

    try {

      logger::cerr::log::set_debug();

      logger::cerr::log::debug("cerr_log_how_to", __LINE__, "hello! ", 309);
      logger::cerr::log::debug("cerr_log_how_to", __LINE__,
                               "how are you doing? ", 3.14);
      logger::cerr::log::info("cerr_log_how_to", __LINE__, "fine!! ", 'W');
      logger::cerr::log::info("cerr_log_how_to", __LINE__, "and you?");
      logger::cerr::log::warn("cerr_log_how_to", __LINE__,
                              "great! got a new job!! ", 6987.58f);
      logger::cerr::log::warn("cerr_log_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cerr' log usage"; }
  static std::string name() { return "cerr_log_how_to"; }
};

struct cerr_log_single {
  bool operator()() {
    try {

      typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log>
          sleeping_loop;
      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            logger::cerr::log::debug("cerr_log_single", __LINE__,
                                     "================= work! ", time(nullptr));
            for (uint32_t _i = 0; _i < 5; ++_i) {
              logger::cerr::log::debug("cerr_log_single", __LINE__, "ola! ",
                                       _i);
              logger::cerr::log::debug("cerr_log_single", __LINE__,
                                       "como vai? ", _i);
              logger::cerr::log::info("cerr_log_single", __LINE__, "vou bem!! ",
                                      _i);
              logger::cerr::log::info("cerr_log_single", __LINE__, "e vc? ",
                                      _i);
              logger::cerr::log::warn("cerr_log_single", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::cerr::log::warn("cerr_log_single", __LINE__, "que bom! ",
                                      _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      _loop1.run();

      logger::cerr::log::debug("cerr_log_single", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::minutes(1));
      logger::cerr::log::debug("cerr_log_single", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      logger::cerr::log::fatal("ERRO logger::cerr::log::log_single: '",
                               _ex.what(), "'");
    }
    return false;
  }

  static std::string desc() { return "Single thread logging to 'cerr'"; }
  static std::string name() { return "cerr_log_single"; }
};

class cerr_log_multi {

public:
  bool operator()() {
    typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log>
        sleeping_loop;
    try {
      logger::cerr::log::set_debug();

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            for (uint32_t _i = 0; _i < 1000; ++_i) {
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "ola! ", 33);
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "como vai? ",
                                       _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "vou bem!! ",
                                      _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "e vc? ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__, "que bom! ",
                                      _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop2(
          std::chrono::milliseconds(500),
          []() {
            for (uint32_t _i = 0; _i < 1500; ++_i) {
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "aaa! ", 33);
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "bbb? ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "ccc!! ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "ddd ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__, "eee! ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__, "fff! ", _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
          std::chrono::milliseconds(100),
          []() {
            for (uint32_t _i = 0; _i < 3000; ++_i) {
              logger::cerr::log::debug("cerr_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz! ", 33);
              logger::cerr::log::debug("cerr_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz? ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz!! ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      _loop1.run();
      _loop2.run();
      _loop3.run();

      logger::cerr::log::debug("cerr_log_multi", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      logger::cerr::log::debug("cerr_log_multi", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO logger::cerr::log::log_multi: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Multiple threads logging to 'cerr'"; }
  static std::string name() { return "cerr_log_multi"; }
};

int main(int argc, char **argv) {

  tester::test _test(argc, argv);
  _test.run<cerr_log_how_to>();
  _test.run<cerr_log_creation>();
  _test.run<cerr_log_single>();
  _test.run<cerr_log_multi>();
}

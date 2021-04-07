#include <iostream>
#include <sstream>
#include <string>

#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;

struct cerr_log_how_to {
  bool operator()() {

    try {

      m_log.debug(__FILE__, __LINE__, "hello! ", 309);
      m_log.debug(__FILE__, __LINE__, "how are you doing? ", 3.14);
      m_log.info(__FILE__, __LINE__, "fine!! ", 'W');
      m_log.info(__FILE__, __LINE__, "and you?");
      m_log.warn(__FILE__, __LINE__, "great! got a new job!! ", 6987.58f);
      m_log.warn(__FILE__, __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cerr' log usage"; }
  logger::cerr<> m_log{"cerr_log_how_to"};
};

// struct cerr_log_single {
//  bool operator()() {
//    try {

//      typedef async::sleeping_loop_t<void, logger::cerr> sleeping_loop;
//      sleeping_loop _loop1(
//          std::chrono::milliseconds(1000),
//          [this]() {
//            m_log.debug(this__func_, __FILE__, __LINE__,
//            "================= work! ", time(nullptr)); for (uint32_t _i = 0;
//            _i < 5; ++_i) {
//              m_log.debug(this__func_, __FILE__, __LINE__, "ola! ",
//              _i); m_log.debug(this__func_, __FILE__, __LINE__, "como
//              vai? ", _i); m_log.info(this__func_, __FILE__, __LINE__,
//              "vou bem!! ", _i); m_log.info(this__func_, __FILE__,
//              __LINE__, "e vc? ", _i); m_log.warn(this__func_,
//              __FILE__, __LINE__, "ótimo! novo emprego! ", _i);
//              m_log.warn(this__func_, __FILE__, __LINE__, "que bom! ",
//              _i);
//            }
//            return true;
//          },
//          std::chrono::milliseconds(1000));

//      _loop1.start();

//      m_log.debug(this__func_, __FILE__, __LINE__, "---- sleeping");
//      std::this_thread::sleep_for(std::chrono::minutes(1));
//      m_log.debug(this__func_, __FILE__, __LINE__, "---- waking up");

//      return true;
//    } catch (std::exception &_ex) {
//      m_log.fatal(this__func_, __FILE__, __LINE__, "ERRO
//      m_log.log_single: '", _ex.what(),
//      "'");
//    }
//    return false;
//  }

//  static std::string desc() { return "Single thread logging to 'cerr'"; }

// private:
//  logger::cerr m_log{"cerr_log_single"};
//};

// class cerr_log_multi {

// public:
//  bool operator()() {
//    typedef async::sleeping_loop_t<void, logger::cerr> sleeping_loop;
//    try {
//      sleeping_loop _loop1(
//          std::chrono::milliseconds(1000),
//          [this]() {
//            for (uint32_t _i = 0; _i < 1000; ++_i) {
//              m_log.debug(this__func_, __FILE__, __LINE__, "ola! ",
//              33); m_log.debug(this__func_, __FILE__, __LINE__, "como
//              vai? ", _i); m_log.info(this__func_, __FILE__, __LINE__,
//              "vou bem!! ", _i); m_log.info(this__func_, __FILE__,
//              __LINE__, "e vc? ", _i); m_log.warn(this__func_,
//              __FILE__, __LINE__, "ótimo! novo emprego! ", _i);
//              m_log.warn(this__func_, __FILE__, __LINE__, "que bom! ",
//              _i);
//            }
//            return true;
//          },
//          std::chrono::milliseconds(1000));

//      sleeping_loop _loop2(
//          std::chrono::milliseconds(500),
//          [this]() {
//            for (uint32_t _i = 0; _i < 1500; ++_i) {
//              m_log.debug(this__func_, __FILE__, __LINE__, "aaa! ",
//              33); m_log.debug(this__func_, __FILE__, __LINE__, "bbb?
//              ", _i); m_log.info(this__func_, __FILE__, __LINE__,
//              "ccc!! ", _i); m_log.info(this__func_, __FILE__,
//              __LINE__, "ddd ", _i); m_log.warn(this__func_, __FILE__,
//              __LINE__, "eee! ", _i); m_log.warn(this__func_, __FILE__,
//              __LINE__, "fff! ", _i);
//            }
//            return true;
//          },
//          std::chrono::milliseconds(1000));

//      sleeping_loop _loop3(
//          std::chrono::milliseconds(100),
//          [this]() {
//            for (uint32_t _i = 0; _i < 3000; ++_i) {
//              m_log.debug(this__func_, __FILE__, __LINE__,
//              "abcdefghijklmnopqrstivwxyz! ", 33);
//              m_log.debug(this__func_,
//              __FILE__, __LINE__, "abcdefghijklmnopqrstivwxyz? ", _i);
//              m_log.info(this__func_, __FILE__, __LINE__,
//              "abcdefghijklmnopqrstivwxyz!! ", _i);
//              m_log.info(this__func_,
//              __FILE__, __LINE__, "abcdefghijklmnopqrstivwxyz ", _i);
//              m_log.warn(this__func_, __FILE__, __LINE__,
//              "abcdefghijklmnopqrstivwxyz! ", _i);
//              m_log.warn(this__func_,
//              __FILE__, __LINE__, "abcdefghijklmnopqrstivwxyz! ", _i);
//            }
//            return true;
//          },
//          std::chrono::milliseconds(1000));

//      _loop1.start();
//      _loop2.start();
//      _loop3.start();

//      m_log.debug(this__func_, __FILE__, __LINE__, "---- sleeping");
//      std::this_thread::sleep_for(std::chrono::seconds(50));
//      m_log.debug(this__func_, __FILE__, __LINE__, "---- waking up");

//      return true;
//    } catch (std::exception &_ex) {
//      std::cerr << "ERRO m_log.log_multi: '" << _ex.what() << "'" <<
//      std::endl;
//    }
//    return false;
//  }

//  static std::string desc() { return "Multiple threads logging to 'cerr'"; }

// private:
//  logger::cerr m_log{"cerr_log_multi"};
//};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test<> _tester(argc, argv);
  run_test(_tester, cerr_log_how_to);
  //  run_test(_tester, cerr_log_single);
  //  run_test(_tester, cerr_log_multi);
}

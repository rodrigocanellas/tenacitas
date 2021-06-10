#include <iostream>
#include <sstream>
#include <string>

#include <concurrent/sleeping_loop.h>
#include <concurrent/traits.h>
#include <logger/file/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct file_log_how_to {
  bool operator()() {

    try {
      logger::file::log::configure(".", "FileLogHowTo", 10 * 1024,
                                   std::chrono::minutes(15));

      m_log.debug(__LINE__, "hello! ", 309);
      m_log.debug(__LINE__, "how are you doing? ", 3.14);
      m_log.info(__LINE__, "fine!! ", 'W');
      m_log.info(__LINE__, "and you?");
      m_log.warn(__LINE__, "great! got a new job!! ", 6987.58f);
      m_log.warn(__LINE__, "nice!! ", 10);
      std::this_thread::sleep_for(std::chrono::microseconds(982248));
      m_log.warn(__LINE__, "a few microsecs later");
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERROR: '" << _ex.what() << "'" << std::endl;
    }
    return false;
  }
  static std::string desc() { return "Simple 'file_log' usage"; }

private:
  logger::file::log m_log{"file_log_how_to"};
};

class file_log_creation {

public:
  bool operator()() {

    try {
      logger::file::log::configure(".", "FileLogCreation", 100,
                                   std::chrono::minutes(10));

      return true;
    } catch (std::exception &_ex) {
      m_log.fatal(__LINE__, "ERROR: '", _ex.what(), "'");
    }
    return false;
  }
  static std::string desc() { return "'file_log' creation"; }

private:
  logger::file::log m_log{"file_log_creation"};
};

class file_log_single {

public:
  typedef concurrent::sleeping_loop_t<void, tenacitas::logger::file::log>
      sleeping_loop;

  bool operator()() {
    try {
      using namespace tenacitas;

      logger::file::log::configure(".", "FileLogSingle", 10 * 1024,
                                   std::chrono::minutes(5));

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          [this]() {
            m_log.debug(__LINE__, "================= work! ", time(nullptr));
            for (uint32_t _i = 0; _i < 5; ++_i) {
              m_log.debug(__LINE__, "ola! ", _i);
              m_log.debug(__LINE__, "como vai? ", _i);
              m_log.info(__LINE__, "vou bem!! ", _i);
              m_log.info(__LINE__, "e vc? ", _i);
              m_log.warn(__LINE__, "ótimo! novo emprego! ", _i);
              m_log.warn(__LINE__, "que bom! ", _i);
            }
            return true;
          },
          std::chrono::milliseconds(1000));

      _loop1.start();

      m_log.debug(__LINE__, "---- sleeping 3 minutes");
      std::this_thread::sleep_for(std::chrono::minutes(5));
      m_log.debug(__LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      m_log.fatal(__LINE__, "ERROR: '", _ex.what(), "'");
    }
    return false;
  }

  static std::string desc() { return "Single thread logging to 'file'"; }
  static std::string name() { return "file_log_single"; }

private:
  logger::file::log m_log{"file_log_single"};
};

class file_log_multi {

public:
  typedef concurrent::sleeping_loop_t<void, logger::file::log> sleeping_loop;

  bool operator()() {
    try {
      logger::file::log::configure("./", "FileLogMulti", 1 * 1024 * 1024,
                                   std::chrono::minutes(3));

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          [this]() {
            for (uint32_t _i = 0; _i < 1000; ++_i) {
              m_log.debug(__LINE__, "ola! ", 33);
              m_log.debug(__LINE__, "como vai? ", _i);
              m_log.info(__LINE__, "vou bem!! ", _i);
              m_log.info(__LINE__, "e vc? ", _i);
              m_log.warn(__LINE__, "ótimo! novo emprego! ", _i);
              m_log.warn(__LINE__, "que bom! ", _i);
            }
            return true;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop2(
          std::chrono::milliseconds(500),
          [this]() {
            for (uint32_t _i = 0; _i < 1500; ++_i) {
              m_log.debug(__LINE__, "aaa! ", 33);
              m_log.debug(__LINE__, "bbb? ", _i);
              m_log.info(__LINE__, "ccc!! ", _i);
              m_log.info(__LINE__, "ddd ", _i);
              m_log.warn(__LINE__, "eee! ", _i);
              m_log.warn(__LINE__, "fff! ", _i);
            }
            return true;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
          std::chrono::milliseconds(100),
          [this]() {
            for (uint32_t _i = 0; _i < 3000; ++_i) {
              m_log.debug(__LINE__, "abcdefghijklmnopqrstivwxyz! ", 33);
              m_log.debug(__LINE__, "abcdefghijklmnopqrstivwxyz? ", _i);
              m_log.info(__LINE__, "abcdefghijklmnopqrstivwxyz!! ", _i);
              m_log.info(__LINE__, "abcdefghijklmnopqrstivwxyz ", _i);
              m_log.warn(__LINE__, "abcdefghijklmnopqrstivwxyz! ", _i);
              m_log.warn(__LINE__, "abcdefghijklmnopqrstivwxyz! ", _i);
            }
            return true;
          },
          std::chrono::milliseconds(1000));

      _loop1.start();
      _loop2.start();
      _loop3.start();

      m_log.debug(__LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      m_log.debug(__LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      m_log.fatal(__LINE__, "ERROR: '", _ex.what(), "'");
    }
    return false;
  }
  static std::string desc() { return "Multiple threads logging to 'file'"; }
  static std::string name() { return "file_log_multi"; }

private:
  logger::file::log m_log{"file_log_multi"};
};

int main(int argc, char **argv) {
  logger::file::log::set_debug();
  logger::cerr::log::set_debug();
  tester::test _tester(argc, argv);
  run_test(_tester, file_log_how_to);
  run_test(_tester, file_log_creation);
  run_test(_tester, file_log_single);
  run_test(_tester, file_log_multi);
}

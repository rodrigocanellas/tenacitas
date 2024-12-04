/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <string>
// #include <tuple>
// #include <utility>

#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/log/cout.h"
#include "tenacitas.lib/log/file.h"
#include "tenacitas.lib/program/options.h"
#include "tenacitas.lib/tester/test.h"
#include "tenacitas.lib/tuple/tuple_output.h"

using namespace std::chrono_literals;

using namespace tenacitas::lib;

struct basic_log {
  static std::string desc() { return "basic log"; }

  bool operator()(const program::options &) {
    log::cout _logger;

    _logger.set_tra();

    _logger.tst("str");

    return true;
  }
};

struct cerr_log {

  bool operator()(const program::options &) {

    try {
      log::cerr _logger;
      _logger.set_tra();

      _logger.tst(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple('z', 3.13, "hi"), ',',
                              std::make_pair(-938, "h"), ',', 3min, ',', 8s,
                              ',', 20ms, ',', 300us, ',', -9, ',', 28));

      _logger.tra(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.deb(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.inf(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.war(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.err(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.fat(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cerr' log usage"; }
};

struct cout_log {
  bool operator()(const program::options &) {
    try {
      log::cout _logger;
      _logger.set_tra();
      _logger.tst(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.tra(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.deb(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.inf(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.war(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.err(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.fat(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cout' log usage"; }
};

struct file_log {
  bool operator()(const program::options &) {
    try {
      log::file _logger(log::file_logger_writer("./file_log_how_to"));
      _logger.set_tra();
      _logger.tst(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.tra(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.deb(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.inf(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.war(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.err(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      _logger.fat(format::fmt("hello!", ',', 309, ',', 'W', ',',
                              std::make_tuple<char, float>('z', 3.13), ',',
                              std::make_pair<int, std::string>(-938, "h"), ',',
                              3min, ',', 8s, ',', 20ms, ',', 300us, ',', -9,
                              ',', 28));
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'file' log usage"; }
};

struct file_log_multiple_files {
  bool operator()(const program::options &) {
    try {
      log::file _logger(
          log::file_logger_writer("./file_log_more_than_one_file", 10 * 1024));
      _logger.set_tra();
      for (uint16_t _i = 0; _i < 100; ++_i) {
        _logger.tst(format::fmt("hello!", ',', 309, ',', 'W', ',',
                                std::make_tuple<char, float>('z', 3.13), ',',
                                std::make_pair<int, std::string>(-938, "h"),
                                ',', 3min, ',', 8s, ',', 20ms, ',', 300us, ',',
                                -9, ',', 28, ',', _i));
        _logger.tra(format::fmt("hello!", ',', 309, ',', 'W', ',',
                                std::make_tuple<char, float>('z', 3.13), ',',
                                std::make_pair<int, std::string>(-938, "h"),
                                ',', 3min, ',', 8s, ',', 20ms, ',', 300us, ',',
                                -9, ',', 28, ',', _i));
        _logger.deb(format::fmt("hello!", ',', 309, ',', 'W', ',',
                                std::make_tuple<char, float>('z', 3.13), ',',
                                std::make_pair<int, std::string>(-938, "h"),
                                ',', 3min, ',', 8s, ',', 20ms, ',', 300us, ',',
                                -9, ',', 28, ',', _i));
        _logger.inf(format::fmt("hello!", ',', 309, ',', 'W', ',',
                                std::make_tuple<char, float>('z', 3.13), ',',
                                std::make_pair<int, std::string>(-938, "h"),
                                ',', 3min, ',', 8s, ',', 20ms, ',', 300us, ',',
                                -9, ',', 28, ',', _i));
        _logger.war(format::fmt("hello!", ',', 309, ',', 'W', ',',
                                std::make_tuple<char, float>('z', 3.13), ',',
                                std::make_pair<int, std::string>(-938, "h"),
                                ',', 3min, ',', 8s, ',', 20ms, ',', 300us, ',',
                                -9, ',', 28, ',', _i));
        _logger.err(format::fmt("hello!", ',', 309, ',', 'W', ',',
                                std::make_tuple<char, float>('z', 3.13), ',',
                                std::make_pair<int, std::string>(-938, "h"),
                                ',', 3min, ',', 8s, ',', 20ms, ',', 300us, ',',
                                -9, ',', 28, ',', _i));
        _logger.fat(format::fmt("hello!", ',', 309, ',', 'W', ',',
                                std::make_tuple<char, float>('z', 3.13), ',',
                                std::make_pair<int, std::string>(-938, "h"),
                                ',', 3min, ',', 8s, ',', 20ms, ',', 300us, ',',
                                -9, ',', 28, ',', _i));
      }
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "File log to more than one file"; }
};

struct cout_log_no_fmt {
  bool operator()(const program::options &) {
    try {
      log::cout _logger;
      _logger.set_tra();
      _logger.tst("a simple string");
      _logger.tra("a simple string");
      _logger.deb("a simple string");
      _logger.inf("a simple string");
      _logger.war("a simple string");
      _logger.err("a simple string");
      _logger.fat("a simple string");

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() {
    return "Simple 'cout' log usage with a simple string as content";
  }
};

struct log_000 {
  static std::string desc() { return "Change and restore the log level"; }

  bool operator()(const program::options &) {
    log::cerr _cerr;

    _cerr.set_inf();

    _cerr.inf("info level");
    _cerr.deb("debug will not be printed");

    _cerr.save_level();

    _cerr.set_deb();

    _cerr.deb("now debug will be printed");
    _cerr.inf("as weel as info");
    _cerr.restore_level();
    _cerr.deb("now debug will not be printed again");
    _cerr.inf("info printed");
    return true;
  }
};

int main(int argc, char **argv) {

  tester::test _tester(argc, argv);

  run_test(_tester, cerr_log);
  run_test(_tester, cout_log);
  run_test(_tester, file_log);
  run_test(_tester, file_log_multiple_files);
  run_test(_tester, cout_log_no_fmt);
  run_test(_tester, log_000);
  run_test(_tester, basic_log);
}

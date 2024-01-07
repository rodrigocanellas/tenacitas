/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <string>
#include <tuple>

#include <tnct/lib/alg/log.h>
#include <tnct/lib/alg/program_options.h>
#include <tnct/lib/alg/tester.h>

struct cerr_log_how_to {
  bool operator()(const tla::program_options &) {
    try {
      TNCT_LOG_DEB("hello! ", 309);
      TNCT_LOG_DEB("how are you doing? ", 3.14);
      TNCT_LOG_INF("fine!! ", 'W');
      TNCT_LOG_INF("and you?");
      TNCT_LOG_WAR("great! got a new job!! ", 6987.58f);
      TNCT_LOG_WAR("nice!! ", 10);
      std::tuple<uint16_t, std::string, float> _tuple{43, "yes!!", -0.32};
      TNCT_LOG_ERR(_tuple);
      std::pair<char, double> _pair{'W', 321.098};
      TNCT_LOG_WAR(_pair);
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cerr' log usage"; }
};

struct clog_how_to {
  bool operator()(const tla::program_options &) {
    try {

      TNCT_LOG_DEB("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                   3.1415);
      TNCT_LOG_INF("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                   3.1415);
      TNCT_LOG_WAR("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                   3.1415);
      TNCT_LOG_ERR("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                   3.1415);
      TNCT_LOG_FAT("teste de clog para debug ", -345, ' ', 19023, ' ', 'W', ' ',
                   3.1415);

      return true;

    } catch (std::exception &_ex) {
      std::cout << "ERRO clog_how_to: '" << _ex.what() << "'" << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'clog' log usage"; }
};

struct cout_log_how_to {
  bool operator()(const tla::program_options &) {
    try {
      TNCT_LOG_DEB("how are you doing? ", 3.14);
      TNCT_LOG_INF("fine!! ", 'W');
      TNCT_LOG_INF("and you?");
      TNCT_LOG_WAR("great! got a new job!! ", 6987.58f);
      TNCT_LOG_WAR("nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cout' log usage"; }
};

struct cout_log_print_tuple {
  static std::string desc() { return "Prints a tuple"; }

  bool operator()(const tla::program_options &) {
    try {
      std::tuple<int16_t, float> _tuple{-3, 3.14};

      TNCT_LOG_DEB("how are you doing? ", _tuple);
      return true;
    } catch (std::exception &_ex) {
      std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
};

struct file_log {
  static std::string desc() { return "Testing logging into a file."; }

  bool operator()(const tla::program_options &) {
    try {
      tla::set_log_file_writer("test_logger", 150);
      TNCT_LOG_DEB("how are you doing? ", 3.14);
      TNCT_LOG_INF("fine!! ", 'W');
      TNCT_LOG_INF("and you?");
      TNCT_LOG_WAR("great! got a new job!! ", 6987.58f);
      TNCT_LOG_WAR("nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cout << "ERRO file_log: '" << _ex.what() << "'" << std::endl;
    }
    return false;
  }
};

int main(int argc, char **argv) {

  tla::set_writer_cerr();
  tla::set_debug_level();
  tla::tester _tester(argc, argv);

  run_test(_tester, cout_log_how_to);
  run_test(_tester, cout_log_print_tuple);
  run_test(_tester, clog_how_to);
  run_test(_tester, cerr_log_how_to);
  run_test(_tester, file_log);
}

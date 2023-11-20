/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include <tenacitas.h>

using namespace tenacitas::lib;

struct from_memory {
  bool operator()(const tncta::program_options &) {

    tncta::translator_in_memory _translator(
        {{0, "word 0"}, {1, "word 1"}, {9, "word 9"}, {6, "word 6"}});

    TNCT_LOG_TST("translations = ", _translator);

    return (_translator.size() == 4);
  }

  static std::string desc() { return "In memory translator with 4 words"; }
};

struct from_existing_file {
  bool operator()(const tncta::program_options &p_options) {
    std::optional<std::string> _maybe{p_options.get_single_param("file_name")};
    if (!_maybe.has_value()) {
      TNCT_LOG_ERR("File name not provided");
      return false;
    }
    std::string _file_name{std::move(_maybe.value())};

    tncta::translator_from_file _translator;

    if (!_translator.load(_file_name)) {
      return false;
    }

    TNCT_LOG_TST("translations = ", _translator);

    return true;
  }

  static std::string desc() { return "Loads a dictonary from a valid file"; }
};

struct from_non_existing_file {
  bool operator()(const tncta::program_options &) {
    tncta::translator_from_file _translator;

    return !_translator.load("file.dict");
  }

  static std::string desc() {
    return "Trying to load a dictionary from a non-existing file";
  }
};

struct translate_non_existing_word {
  bool operator()(const tncta::program_options &) {
    tncta::translator_in_memory _translator(
        {{0, "word 0"}, {1, "word 1"}, {9, "word 9"}, {6, "word 6"}});

    TNCT_LOG_TST("translations = ", _translator);

    std::string _word = _translator.get(8);

    TNCT_LOG_TST("word = '", _word, "'");

    return _word.empty();
  }

  static std::string desc() {
    return "Trying to translate a non existing word";
  }
};

struct translate_an_existing_word {
  bool operator()(const tncta::program_options &) {
    tncta::translator_in_memory _translator(
        {{0, "word 0"}, {1, "word 1"}, {9, "word 9"}, {6, "word 6"}});

    TNCT_LOG_TST("translations = ", _translator);

    std::string _word = _translator.get(9);

    TNCT_LOG_TST("word = '", _word, "'");

    return _word == "word 9";
  }

  static std::string desc() { return "Trying to translate an existing word"; }
};

int main(int argc, char **argv) {

  tncta::set_writer_cerr();
  tncta::set_debug_level();
  tncta::tester<> _test(argc, argv);

  run_test(_test, from_memory);
  run_test(_test, from_existing_file);
  run_test(_test, from_non_existing_file);
  run_test(_test, translate_non_existing_word);
  run_test(_test, translate_an_existing_word);
}

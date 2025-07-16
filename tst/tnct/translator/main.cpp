/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <optional>
#include <string>

#include "tnct/format/fmt.h"
#include "tnct/tester/test.h"
#include "tnct/log/cerr.h"
#include "tnct/program/options.h"
#include "tnct/translator/translator_from_file.h"
#include "tnct/translator/translator_in_memory.h"

using namespace tnct;

using logger = log::cerr;

struct from_memory {
  bool operator()(const program::options &) {
    logger _log;
    translator::translator_in_memory _translator(
        {{0, "word 0"}, {1, "word 1"}, {9, "word 9"}, {6, "word 6"}});

    _log.tst(format::fmt("translations = ", _translator));

    return (_translator.size() == 4);
  }

  static std::string desc() { return "In memory translator with 4 words"; }
};

struct from_existing_file {
  bool operator()(const program::options &p_options) {
    logger _log;
    std::optional<std::string> _maybe{p_options.get_single_param("file-name")};
    if (!_maybe.has_value()) {
      _log.err("File name not provided");
      return false;
    }
    std::string _file_name{std::move(_maybe.value())};

    translator::translator_from_file<logger> _translator(_log);

    if (!_translator.load(_file_name)) {
      return false;
    }

    _log.tst(format::fmt("translations = ", _translator));

    return true;
  }

  static std::string desc() {
    return "Loads a dictonary from a valid file. It is necessary to pass the "
           "parameter '--file-name' to the test with a valid dictionary.";
  }
};

struct from_non_existing_file {
  bool operator()(const program::options &) {
    logger _log;
    translator::translator_from_file<logger> _translator(_log);

    return !_translator.load("file.dict");
  }

  static std::string desc() {
    return "Trying to load a dictionary from a non-existing file";
  }
};

struct translate_non_existing_word {
  bool operator()(const program::options &) {
    logger _log;
    translator::translator_in_memory _translator(
        {{0, "word 0"}, {1, "word 1"}, {9, "word 9"}, {6, "word 6"}});

    _log.tst(format::fmt("translations = ", _translator));

    std::string _word = _translator.get(8);

    _log.tst(format::fmt("word = '", _word, "'"));

    return _word.empty();
  }

  static std::string desc() {
    return "Trying to translate a non existing word";
  }
};

struct translate_an_existing_word {
  bool operator()(const program::options &) {
    logger _log;
    translator::translator_in_memory _translator(
        {{0, "word 0"}, {1, "word 1"}, {9, "word 9"}, {6, "word 6"}});

    _log.tst(format::fmt("translations = ", _translator));

    std::string _word = _translator.get(9);

    _log.tst(format::fmt("word = '", _word, "'"));

    return _word == "word 9";
  }

  static std::string desc() { return "Trying to translate an existing word"; }
};

int main(int argc, char **argv) {

  tester::test<> _test(argc, argv);

  run_test(_test, from_memory);
  run_test(_test, from_existing_file);
  run_test(_test, from_non_existing_file);
  run_test(_test, translate_non_existing_word);
  run_test(_test, translate_an_existing_word);
}

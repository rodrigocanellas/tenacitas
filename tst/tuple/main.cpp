/// \example traverse_tuple

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <iostream>
#include <string>
#include <tuple>

#include <tnct/lib/alg/log.h>
#include <tnct/lib/alg/program_options.h>
#include <tnct/lib/alg/tester.h>
#include <tnct/lib/alg/traverse_tuple.h>

struct tuple_000 {
  static std::string desc() { return "Traverses a tuple"; }

  bool operator()(const tla::program_options &) {
    using container = std::tuple<char, int, float, std::string>;
    using function_char = std::function<void(char &)>;
    using function_int = std::function<void(int &)>;
    using function_float = std::function<void(float &)>;
    using function_string = std::function<void(std::string &)>;

    using functions = std::tuple<function_char, function_int, function_float,
                                 function_string>;

    auto _function_char = [](char &p_char) { TNCT_LOG_TST("char: ", p_char); };
    auto _function_int = [](int &p_int) { TNCT_LOG_TST("int: ", p_int); };

    auto _function_float = [](float &p_float) {
      TNCT_LOG_TST("float: ", p_float);
    };
    auto _function_string = [](std::string &p_str) {
      TNCT_LOG_TST("string: ", p_str);
    };

    container _container{'h', -8, 3.14, "hi!!"};
    functions _functions{_function_char, _function_int, _function_float,
                         _function_string};

    tla::traverse_tuple(std::move(_functions), _container);

    return true;
  }
};

struct tuple_001 {
  static std::string desc() { return "Traverses a tuple"; }

  bool operator()(const tla::program_options &) {

    auto _functions{std::make_tuple(
        [](char &p_char) { TNCT_LOG_TST("char: ", p_char); },
        [](int &p_int) { TNCT_LOG_TST("int: ", p_int); },
        [](float &p_float) { TNCT_LOG_TST("float: ", p_float); },
        [](std::string &p_str) { TNCT_LOG_TST("string: ", p_str); })};

    auto _data = std::make_tuple('k', static_cast<int>(-19),
                                 static_cast<float>(8.014), std::string("bye"));

    tla::traverse_tuple(_functions, _data);

    return true;
  }
};

struct tuple_002 {
  static std::string desc() { return "Traverses a tuple"; }

  bool operator()(const tla::program_options &) {

    auto _function{[](auto &p_value) { TNCT_LOG_TST("value: ", p_value); }};

    auto _data = std::make_tuple('k', static_cast<int>(-19),
                                 static_cast<float>(8.014), std::string("bye"));

    tla::traverse_tuple(_function, _data);

    return true;
  }
};

int main(int argc, char **argv) {

  try {
    tla::tester _test(argc, argv);
    run_test(_test, tuple_000);
    run_test(_test, tuple_001);
    run_test(_test, tuple_002);

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}

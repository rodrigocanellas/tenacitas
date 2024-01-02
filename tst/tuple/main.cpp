/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <functional>
#include <iostream>
#include <string>
#include <tuple>

#include <tenacitas.h>

using namespace tenacitas::lib;

struct tuple_000 {
  static std::string desc() { return "Traverses a tuple"; }

  bool operator()(const tnctl::program_options_a &) {
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

    tnctl::traverse_a(std::move(_functions), _container);

    return true;
  }
};

struct tuple_001 {
  static std::string desc() { return "Traverses a tuple"; }

  bool operator()(const tnctl::program_options_a &) {

    auto _functions{std::make_tuple(
        [](char &p_char) { TNCT_LOG_TST("char: ", p_char); },
        [](int &p_int) { TNCT_LOG_TST("int: ", p_int); },
        [](float &p_float) { TNCT_LOG_TST("float: ", p_float); },
        [](std::string &p_str) { TNCT_LOG_TST("string: ", p_str); })};

    auto _data = std::make_tuple('k', static_cast<int>(-19),
                                 static_cast<float>(8.014), std::string("bye"));

    tnctl::traverse_a(_functions, _data);

    return true;
  }
};

struct tuple_002 {
  static std::string desc() { return "Traverses a tuple"; }

  bool operator()(const tnctl::program_options_a &) {

    auto _function{[](auto &p_value) { TNCT_LOG_TST("value: ", p_value); }};

    auto _data = std::make_tuple('k', static_cast<int>(-19),
                                 static_cast<float>(8.014), std::string("bye"));

    tnctl::traverse_a(_function, _data);

    return true;
  }
};

int main(int argc, char **argv) {
  using namespace tenacitas;
  try {
    tnctl::tester_a _test(argc, argv);
    run_test(_test, tuple_000);
    run_test(_test, tuple_001);
    run_test(_test, tuple_002);

  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}

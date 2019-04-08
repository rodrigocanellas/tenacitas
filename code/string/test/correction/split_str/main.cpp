#include <iostream>
#include <sstream>
#include <string>

#include <logger/business/cerr.h>
#include <string/business/split_str.h>
#include <tester/business/run.h>

using namespace tenacitas::string::business;

struct test_string_no_extra_spaces
{
    bool operator()()
    {
        int16_t _i = 0;
        bool _ret = true;

        slipt_str("; == < >=", [&_i, &_ret](std::string&& p_str) -> void {
            switch (_i) {
                case 0:
                    if (p_str != ";") {
                        cerr_error("str #",
                                   _i,
                                   " should be ';', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;
                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 1:
                    if (p_str != "==") {
                        cerr_error("str #",
                                   _i,
                                   " should be '==', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 2:
                    if (p_str != "<") {
                        cerr_error("str #",
                                   _i,
                                   " should be '<', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 3:
                    if (p_str != ">=") {
                        cerr_error("str #",
                                   _i,
                                   " should be '>=', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                default:
                    _ret = false;
                    cerr_error("invalid counter ", _i);
            }
        });
        return _ret;
    }
};

struct test_string_extra_spaces
{
    bool operator()()
    {
        int16_t _i = 0;
        bool _ret = true;

        slipt_str("; == < >=   ", [&_i, &_ret](std::string&& p_str) -> void {
            switch (_i) {
                case 0:
                    if (p_str != ";") {
                        cerr_error("str #",
                                   _i,
                                   " should be ';', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;
                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 1:
                    if (p_str != "==") {
                        cerr_error("str #",
                                   _i,
                                   " should be '==', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 2:
                    if (p_str != "<") {
                        cerr_error("str #",
                                   _i,
                                   " should be '<', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 3:
                    if (p_str != ">=") {
                        cerr_error("str #",
                                   _i,
                                   " should be '>=', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                default:
                    cerr_error("invalid counter ", _i);
                    ++_i;
            }
        });
        return _ret;
    }
};

struct test_string_extra_spaces_before_last
{
    bool operator()()
    {
        int16_t _i = 0;
        bool _ret = true;

        slipt_str("; == < >=   !=", [&_i, &_ret](std::string&& p_str) -> void {
            switch (_i) {
                case 0:
                    if (p_str != ";") {
                        cerr_error("str #",
                                   _i,
                                   " should be ';', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;
                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 1:
                    if (p_str != "==") {
                        cerr_error("str #",
                                   _i,
                                   " should be '==', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 2:
                    if (p_str != "<") {
                        cerr_error("str #",
                                   _i,
                                   " should be '<', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 3:
                    if (p_str != ">=") {
                        cerr_error("str #",
                                   _i,
                                   " should be '>=', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                case 4:
                    if (p_str != "!=") {
                        cerr_error("str #",
                                   _i,
                                   " should be '!=', but it is '",
                                   p_str,
                                   "'");
                        _ret = false;

                    } else {
                        cerr_test("str #", _i, " = '", p_str, "'");
                    }
                    ++_i;
                    break;
                default:
                    cerr_error("invalid counter ", _i);
                    ++_i;
            }
        });
        return _ret;
    }
};

struct test_string_extra_spaces_in_the_middle
{
    bool operator()()
    {
        int16_t _i = 0;
        bool _ret = true;

        slipt_str(
          "; ==     < >=   !=", [&_i, &_ret](std::string&& p_str) -> void {
              switch (_i) {
                  case 0:
                      if (p_str != ";") {
                          cerr_error("str #",
                                     _i,
                                     " should be ';', but it is '",
                                     p_str,
                                     "'");
                          _ret = false;
                      } else {
                          cerr_test("str #", _i, " = '", p_str, "'");
                      }
                      ++_i;
                      break;
                  case 1:
                      if (p_str != "==") {
                          cerr_error("str #",
                                     _i,
                                     " should be '==', but it is '",
                                     p_str,
                                     "'");
                          _ret = false;

                      } else {
                          cerr_test("str #", _i, " = '", p_str, "'");
                      }
                      ++_i;
                      break;
                  case 2:
                      if (p_str != "<") {
                          cerr_error("str #",
                                     _i,
                                     " should be '<', but it is '",
                                     p_str,
                                     "'");
                          _ret = false;

                      } else {
                          cerr_test("str #", _i, " = '", p_str, "'");
                      }
                      ++_i;
                      break;
                  case 3:
                      if (p_str != ">=") {
                          cerr_error("str #",
                                     _i,
                                     " should be '>=', but it is '",
                                     p_str,
                                     "'");
                          _ret = false;

                      } else {
                          cerr_test("str #", _i, " = '", p_str, "'");
                      }
                      ++_i;
                      break;
                  case 4:
                      if (p_str != "!=") {
                          cerr_error("str #",
                                     _i,
                                     " should be '!=', but it is '",
                                     p_str,
                                     "'");
                          _ret = false;

                      } else {
                          cerr_test("str #", _i, " = '", p_str, "'");
                      }
                      ++_i;
                      break;
                  default:
                      cerr_error("invalid counter ", _i);
                      ++_i;
              }
          });
        return _ret;
    }
};

int
main(int argc, char** argv)
{
    run_test(test_string_no_extra_spaces,
             argc,
             argv,
             "'test_string_no_extra_spaces' for \"; == < >=\"");

    run_test(test_string_extra_spaces,
             argc,
             argv,
             "'test_string_no_extra_spaces' for \"; == < >=   \"");

    run_test(test_string_extra_spaces_before_last,
             argc,
             argv,
             "'test_string_extra_spaces_before_last' for \"; == < >=   !=\"");

    run_test(
      test_string_extra_spaces_in_the_middle,
      argc,
      argv,
      "'test_string_extra_spaces_in_the_middle' for \"; ==     < >=   !=\"");
}

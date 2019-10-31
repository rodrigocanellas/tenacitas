#ifndef TENACITAS_BUSINESS_TESTE_RUN_H
#define TENACITAS_BUSINESS_TESTE_RUN_H

#include <iostream>
#include <sstream>
#include <string>

#include <business/logger/log.h>
#include <business/tester/internal/test.h>

namespace tenacitas {
namespace business {
namespace tester {

inline static std::string
syntax(char** argv)
{
    std::stringstream _stream;
    _stream
      << "Syntax: \n"
      << argv[0] << " exec  --> executes the test, with log in 'test' level\n"
      << argv[0] << " desc  --> displays a description of the test\n"
      << argv[0]
      << " debug --> sets the log level to 'debug', and executes the test\n";
    return _stream.str();
}

} // namespace tester
} // namespace business
} // namespace tenacitas

#define run_test(test_class, argc, argv, desc)                                 \
    {                                                                          \
        using namespace tenacitas::business;                                   \
        logger::log::set_info();                                               \
        bool _execute = false;                                                 \
        if ((argc) == 1) {                                                     \
            std::cout << tester::syntax(argv);                                 \
        } else if ((argc) == 2) {                                              \
            std::string _param((argv)[1]);                                     \
            if (_param == "exec") {                                            \
                _execute = true;                                               \
            } else if (_param == "desc") {                                     \
                std::cout << #test_class << ": " << (desc) << std::endl;       \
            } else if (_param == "debug") {                                    \
                _execute = true;                                               \
                logger::log::set_debug();                                      \
            }                                                                  \
        }                                                                      \
        if (_execute) {                                                        \
            tester::test _test;                                                \
            test_class _test_obj;                                              \
            bool _rc = _test(#test_class, std::move(_test_obj));               \
            std::cout << #test_class << (_rc ? " SUCCESS" : " FAIL")           \
                      << std::endl;                                            \
        }                                                                      \
    }

#endif // RUN_H

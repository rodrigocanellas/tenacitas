#ifndef TENACITAS_TESTE_BUS_RUN_H
#define TENACITAS_TESTE_BUS_RUN_H

#include <iostream>
#include <sstream>
#include <string>

#include <logger/bus/cerr.h>
#include <tester/bus/internal/test.h>

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

#define run_test(test_class, argc, argv, desc)                                 \
    {                                                                          \
        tenacitas::logger::bus::configure_cerr_log();                          \
        cerr_set_info();                                                       \
        bool _execute = false;                                                 \
        if ((argc) == 1) {                                                     \
            std::cout << syntax(argv);                                         \
        } else if ((argc) == 2) {                                              \
            std::string _param((argv)[1]);                                     \
            if (_param == "exec") {                                            \
                _execute = true;                                               \
            } else if (_param == "desc") {                                     \
                std::cout << #test_class << ": " << (desc) << std::endl;       \
            } else if (_param == "debug") {                                    \
                _execute = true;                                               \
                cerr_set_debug();                                              \
            }                                                                  \
        }                                                                      \
        if (_execute) {                                                        \
            tenacitas::tester::bus::test _test;                                \
            test_class _test_obj;                                              \
            bool _rc = _test(#test_class, std::move(_test_obj));               \
            std::cout << #test_class << (_rc ? " SUCCESS" : " FAIL")           \
                      << std::endl;                                            \
        }                                                                      \
    }

#endif // RUN_H

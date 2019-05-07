#include <iostream>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <interpreter/business/internal/lexeme.h>
#include <interpreter/business/internal/symbol.h>
#include <interpreter/business/internal/symbols.h>
#include <interpreter/business/internal/type.h>
#include <interpreter/business/internal/types.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;
struct recognize_integer
{
    bool operator()(const std::string& p_str)
    {

        for (std::string::value_type _c : p_str) {
            if (std::isdigit(_c) == 0) {
                return false;
            }
        }
        return true;
    }
};

struct recognize_word
{
    bool operator()(const std::string& p_str)
    {
        for (std::string::value_type _c : p_str) {
            if (std::isalpha(_c) == 0) {
                return false;
            }
        }
        return true;
    }
};

struct recognize_token
{
    bool operator()(const std::string& p_str)
    {
        return ((p_str[0] == ';') || (p_str[0] == ',') || (p_str[0] == ')') ||
                (p_str[0] == '(') || (p_str == "!=") || (p_str == "=="));
    }
};

struct recognize_char
{
    bool operator()(const std::string& p_str)
    {
        return (std::isalpha(p_str[0]) == 1);
    }
};

struct symbol_creation
{
    bool operator()()
    {
        type _type("char", recognize_char());

        symbol _symbol(lexeme("a"), &_type);

        return (_symbol.get_lexeme() == lexeme("a") &&
                _symbol.get_type() == _type);
    }
};

// struct symbol_creation_by_move
//{
//    bool operator()()
//    {

//        symbol _symbol(std::string("bye"));
//        std::stringstream _stream;
//        _stream << _symbol;
//        std::string _str;
//        _stream >> _str;
//        cerr_test("symbol = ", _symbol, ", str = ", _str);
//        return (_str == "bye");
//    }
//};

// struct symbol_equal_to
//{
//    bool operator()()
//    {

//        symbol _symbol_1(std::string("bye"));
//        symbol _symbol_2(std::string("bye"));

//        return (_symbol_1 == _symbol_2);
//    }
//};

// struct symbol_not_equal_to
//{
//    bool operator()()
//    {

//        symbol _symbol_1(std::string("bye"));
//        symbol _symbol_2(std::string("hi"));

//        return (_symbol_1 != _symbol_2);
//    }
//};

// struct symbol_greather_than_1
//{
//    bool operator()()
//    {

//        symbol _symbol_1(std::string("bye"));
//        symbol _symbol_2(std::string("byd"));

//        return (_symbol_1 > _symbol_2);
//    }
//};

// struct symbol_greather_than_2
//{
//    bool operator()()
//    {

//        symbol _symbol_1(std::string("bye"));
//        symbol _symbol_2(std::string("byda"));

//        return (_symbol_1 > _symbol_2);
//    }
//};

// struct symbol_less_than_1
//{
//    bool operator()()
//    {

//        symbol _symbol_1(std::string("bye"));
//        symbol _symbol_2(std::string("byf"));

//        return (_symbol_1 < _symbol_2);
//    }
//};

// struct symbol_less_than_2
//{
//    bool operator()()
//    {

//        symbol _symbol_1(std::string("byd"));
//        symbol _symbol_2(std::string("byea"));

//        return (_symbol_1 < _symbol_2);
//    }
//};

// struct symbols_creation_from_string
//{
//    bool operator()()
//    {
//        symbols _symbols("; == < >=");

//        if (_symbols.empty()) {
//            cerr_error("symbols should not be empty");
//            return false;
//        }

//        if (_symbols.size() != 4) {
//            cerr_error("symbols size should be 4, but it is ",
//            _symbols.size()); return false;
//        }
//        {
//            for (symbols::const_iterator _i = _symbols.begin();
//                 _i != _symbols.end();
//                 ++_i) {
//                cerr_test("symbol = ", *_i);
//            }
//        }

//        symbols::const_iterator _ite = _symbols.begin();
//        if (*_ite != symbol(";")) {
//            cerr_error("symbols 1 should ';', but it is ", *_ite);
//            return false;
//        }

//        ++_ite;
//        if (*_ite != symbol("<")) {
//            cerr_error("symbols 2 should '<', but it is ", *_ite);
//            return false;
//        }

//        ++_ite;
//        if (*_ite != symbol("==")) {
//            cerr_error("symbols 3 should '==', but it is ", *_ite);
//            return false;
//        }
//        ++_ite;
//        if (*_ite != symbol(">=")) {
//            cerr_error("symbols 4 should '>=', but it is ", *_ite);
//            return false;
//        }

//        return true;
//    }
//};

// struct symbols_creation_from_string_and_add_more
//{
//    bool operator()()
//    {
//        symbols _symbols("; == < >=");

//        if (_symbols.empty()) {
//            cerr_error("symbols should not be empty");
//            return false;
//        }

//        if (_symbols.size() != 4) {
//            cerr_error("symbols size should be 4, but it is ",
//            _symbols.size()); return false;
//        }

//        _symbols.add(symbol("&"));
//        _symbols.add(symbol("+="));

//        {
//            for (symbols::const_iterator _i = _symbols.begin();
//                 _i != _symbols.end();
//                 ++_i) {
//                cerr_test("symbol = ", *_i);
//            }
//        }

//        symbols::const_iterator _ite = _symbols.begin();
//        if (*_ite != symbol("&")) {
//            cerr_error("symbols 1 should '&', but it is ", *_ite);
//            return false;
//        }

//        ++_ite;
//        if (*_ite != symbol("+=")) {
//            cerr_error("symbols 2 should '+=', but it is ", *_ite);
//            return false;
//        }

//        ++_ite;
//        if (*_ite != symbol(";")) {
//            cerr_error("symbols 3 should '+=', but it is ", *_ite);
//            return false;
//        }

//        ++_ite;
//        if (*_ite != symbol("<")) {
//            cerr_error("symbols 4 should '<', but it is ", *_ite);
//            return false;
//        }

//        ++_ite;
//        if (*_ite != symbol("==")) {
//            cerr_error("symbols 5 should '==', but it is ", *_ite);
//            return false;
//        }
//        ++_ite;
//        if (*_ite != symbol(">=")) {
//            cerr_error("symbols 6 should '>=', but it is ", *_ite);
//            return false;
//        }

//        return true;
//    }
//}
;

int
main(int argc, char** argv)
{
    run_test(symbol_creation,
             argc,
             argv,
             "'symbol' creation with lexeme \"a\", and type \"char\"");
    //    run_test(symbol_creation_by_move, argc, argv, "'symbol' creation by
    //    move"); run_test(symbol_equal_to, argc, argv, "'symbol' equal_to");
    //    run_test(symbol_not_equal_to, argc, argv, "'symbol' not_equal_to");
    //    run_test(symbol_greather_than_1,
    //             argc,
    //             argv,
    //             "'symbol' greather_than, equal sizes");
    //    run_test(symbol_greather_than_2,
    //             argc,
    //             argv,
    //             "'symbol' greather_than, right longer than left, but left
    //             still " "symbolicographically greater");
    //    run_test(symbol_less_than_1, argc, argv, "'symbol' less_than, equal
    //    sizes"); run_test(symbol_less_than_2,
    //             argc,
    //             argv,
    //             "'symbol' less_than, right longer than left, but left still "
    //             "symbolicographically less");
    //    run_test(symbols_creation_from_string,
    //             argc,
    //             argv,
    //             "'symbols' creation with a string with 4 symbol: ; == < >=");

    //    run_test(symbols_creation_from_string_and_add_more,
    //             argc,
    //             argv,
    //             "'symbols' creation with a string with 4 symbol: ; == < >=,
    //             and " "then added & and +=");
}

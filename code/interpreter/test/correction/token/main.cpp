#include <iostream>
#include <sstream>
#include <string>

#include <interpreter/business/internal/token.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

struct token_recognize_1
{
    bool operator()()
    {

        token _token(2, type("relational-operator"));

        _token.add("<=");
        _token.add(">=");

        type _type = _token.recognize("<=");
        if (_type == type::undefined) {
            cerr_error("'<=' was not recognized, but it should");
            return false;
        }
        cerr_test("Type recognized: '", _type, "'");
        return true;
    }
};

struct token_recognize_2
{
    bool operator()()
    {

        token _token(2, type("relational-operator"));

        _token.add("<=");
        _token.add(">=");

        type _type = _token.recognize(">=");
        if (_type == type::undefined) {
            cerr_error("'<=' was not recognized, but it should");
            return false;
        }
        cerr_test("Type recognized: '", _type, "'");
        return true;
    }
};

struct token_order_1
{
    bool operator()()
    {

        token _t1(3, type("three"));
        token _t2(4, type("four"));

        if (_t2 < _t1) {
            cerr_error(
              "Type with size 3 should come first than a type with size 4");
            return false;
        }
        return true;
    }
};

struct token_order_2
{
    bool operator()()
    {

        token _t1(3, type("c"));
        token _t2(3, type("a"));

        if (!(_t2 < _t1)) {
            cerr_error("Types with same size should be order lexographically");
            return false;
        }
        return true;
    }
};

int
main(int argc, char** argv)
{

    run_test(token_recognize_1, argc, argv, "recognizes '<='");
    run_test(token_recognize_2, argc, argv, "recognizes '>='");
    run_test(token_order_1,
             argc,
             argv,
             "checks if a token with size 3 comes before a token with size 4");
    run_test(token_order_2,
             argc,
             argv,
             "checks if a two tokens with the same size are ordered according "
             "to theirs types");
}

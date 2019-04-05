#include <iostream>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <interpreter/business/internal/lexeme.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

struct lexeme_creation_by_copy
{
    bool operator()()
    {
        lexeme _lex("hello");
        std::stringstream _stream;
        _stream << _lex;
        std::string _str;
        _stream >> _str;
        cerr_test("lex = ", _lex, ", str = ", _str);
        return (_str == "hello");
    }
};

struct lexeme_creation_by_move
{
    bool operator()()
    {

        lexeme _lex(std::string("bye"));
        std::stringstream _stream;
        _stream << _lex;
        std::string _str;
        _stream >> _str;
        cerr_test("lex = ", _lex, ", str = ", _str);
        return (_str == "bye");
    }
};

int
main(int argc, char** argv)
{
    run_test(lexeme_creation_by_copy, argc, argv, "'lexeme' creation by copy");
    run_test(lexeme_creation_by_move, argc, argv, "'lexeme' creation by move");
}

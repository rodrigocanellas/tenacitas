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

struct lexeme_equal_to
{
    bool operator()()
    {

        lexeme _lex_1(std::string("bye"));
        lexeme _lex_2(std::string("bye"));

        return (_lex_1 == _lex_2);
    }
};

struct lexeme_not_equal_to
{
    bool operator()()
    {

        lexeme _lex_1(std::string("bye"));
        lexeme _lex_2(std::string("hi"));

        return (_lex_1 != _lex_2);
    }
};

struct lexeme_greather_than_1
{
    bool operator()()
    {

        lexeme _lex_1(std::string("bye"));
        lexeme _lex_2(std::string("byd"));

        return (_lex_1 > _lex_2);
    }
};

struct lexeme_greather_than_2
{
    bool operator()()
    {

        lexeme _lex_1(std::string("bye"));
        lexeme _lex_2(std::string("byda"));

        return (_lex_1 > _lex_2);
    }
};

struct lexeme_less_than_1
{
    bool operator()()
    {

        lexeme _lex_1(std::string("bye"));
        lexeme _lex_2(std::string("byf"));

        return (_lex_1 < _lex_2);
    }
};

struct lexeme_less_than_2
{
    bool operator()()
    {

        lexeme _lex_1(std::string("byd"));
        lexeme _lex_2(std::string("byea"));

        return (_lex_1 < _lex_2);
    }
};

int
main(int argc, char** argv)
{
    run_test(lexeme_creation_by_copy, argc, argv, "'lexeme' creation by copy");
    run_test(lexeme_creation_by_move, argc, argv, "'lexeme' creation by move");
    run_test(lexeme_equal_to, argc, argv, "'lexeme' equal_to");
    run_test(lexeme_not_equal_to, argc, argv, "'lexeme' not_equal_to");
    run_test(lexeme_greather_than_1,
             argc,
             argv,
             "'lexeme' greather_than, equal sizes");
    run_test(lexeme_greather_than_2,
             argc,
             argv,
             "'lexeme' greather_than, right longer than left, but left still "
             "lexicographically greater");
    run_test(lexeme_less_than_1, argc, argv, "'lexeme' less_than, equal sizes");
    run_test(lexeme_less_than_2,
             argc,
             argv,
             "'lexeme' less_than, right longer than left, but left still "
             "lexicographically less");
}

#include <iostream>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <interpreter/business/internal/type.h>
#include <interpreter/business/internal/types.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

struct recognize_integer
{
    bool operator()(const lexeme& p_lex)
    {
        lexeme::const_iterator _end = p_lex.end();
        for (lexeme::const_iterator _ite = p_lex.begin(); _ite != _end;
             ++_ite) {
            if (std::isdigit(*_ite) != 0) {
                return false;
            }
        }
        return true;
    }
};

struct recognize_word
{
    bool operator()(const lexeme& p_lex)
    {
        lexeme::const_iterator _end = p_lex.end();
        for (lexeme::const_iterator _ite = p_lex.begin(); _ite != _end;
             ++_ite) {
            if (std::isalpha(*_ite) != 0) {
                return false;
            }
        }
        return true;
    }
};

struct type_creation_by_copy
{
    bool operator()()
    {
        type _type("integer", recognize_integer());
        std::stringstream _stream;
        _stream << _type;
        std::string _str;
        _stream >> _str;
        cerr_test("type = ", _type, ", str = ", _str);
        return (_str == "integer");
    }
};

struct type_creation_by_move
{
    bool operator()()
    {
        type _type(std::string("integer"), recognize_integer());
        std::stringstream _stream;
        _stream << _type;
        std::string _str;
        _stream >> _str;
        cerr_test("type = ", _type, ", str = ", _str);
        return (_str == "integer");
    }
};

struct type_equal_to
{
    bool operator()()
    {
        type _type_1(std::string("integer"), recognize_integer());
        type _type_2(std::string("integer"), recognize_integer());

        return (_type_1 == _type_2);
    }
};

struct type_not_equal_to
{
    bool operator()()
    {
        type _type_1(std::string("integer"), recognize_integer());
        type _type_2(std::string("word"), recognize_word());

        return (_type_1 != _type_2);
    }
};

struct type_greather_than
{
    bool operator()()
    {
        type _type_1(std::string("word"), recognize_word());
        type _type_2(std::string("integer"), recognize_integer());

        return (_type_1 > _type_2);
    }
};

struct type_less_than
{
    bool operator()()
    {
        type _type_1(std::string("integer"), recognize_integer());
        type _type_2(std::string("word"), recognize_word());

        return (_type_1 < _type_2);
    }
};

struct add_types
{
    bool operator()()
    {

        types _types;

        _types.add(type(std::string("integer"), recognize_integer()));
        _types.add(type(std::string("word"), recognize_word()));

        if (_types.empty()) {
            return false;
        }
        if (_types.size() != 2) {
            return false;
        }
        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(type_creation_by_copy, argc, argv, "'type' creation by copy");
    run_test(type_creation_by_move, argc, argv, "'type' creation by move");
    run_test(type_equal_to, argc, argv, "'type' equal_to");
    run_test(type_not_equal_to, argc, argv, "'type' not_equal_to");
    run_test(type_greather_than, argc, argv, "'type' greather_than");
    run_test(type_less_than, argc, argv, "'type' less_than");
    run_test(add_types, argc, argv, "add 2 types to 'types'");
}

#include <iostream>
#include <sstream>
#include <string>

#include <interpreter/business/type.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

struct type_creation_by_copy
{
    bool operator()()
    {
        type _type("integer");
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

        type _type(std::string("identifier"));
        std::stringstream _stream;
        _stream << _type;
        std::string _str;
        _stream >> _str;
        cerr_test("type = ", _type, ", str = ", _str);
        return (_str == "identifier");
    }
};

struct type_equal_to
{
    bool operator()()
    {

        type _type_1(std::string("integer"));
        type _type_2(std::string("integer"));

        return (_type_1 == _type_2);
    }
};

struct type_not_equal_to
{
    bool operator()()
    {

        type _type_1(std::string("integer"));
        type _type_2(std::string("identifier"));

        return (_type_1 != _type_2);
    }
};

struct type_greather_than_1
{
    bool operator()()
    {

        type _type_1(std::string("integer"));
        type _type_2(std::string("identifier"));

        return (_type_1 > _type_2);
    }
};

struct type_less_than_1
{
    bool operator()()
    {

        type _type_1(std::string("identifier"));
        type _type_2(std::string("integer"));

        return (_type_1 < _type_2);
    }
};

int
main(int argc, char** argv)
{
    run_test(type_creation_by_copy, argc, argv, "'type' creation by copy");
    run_test(type_creation_by_move, argc, argv, "'type' creation by move");
    run_test(type_equal_to, argc, argv, "'type' equal_to");
    run_test(type_not_equal_to, argc, argv, "'type' not_equal_to");
    run_test(
      type_greather_than_1, argc, argv, "'type' greather_than, equal sizes");
    run_test(type_less_than_1, argc, argv, "'type' less_than, equal sizes");
}

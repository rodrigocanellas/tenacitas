#include <iostream>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <interpreter/business/internal/type.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

struct type_creation_by_copy
{
    bool operator()()
    {
        type _type("hello");
        std::stringstream _stream;
        _stream << _type;
        std::string _str;
        _stream >> _str;
        cerr_test("type = ", _type, ", str = ", _str);
        return (_str == "hello");
    }
};

struct type_creation_by_move
{
    bool operator()()
    {

        type _type(std::string("bye"));
        std::stringstream _stream;
        _stream << _type;
        std::string _str;
        _stream >> _str;
        cerr_test("type = ", _type, ", str = ", _str);
        return (_str == "bye");
    }
};

struct type_equal_to
{
    bool operator()()
    {

        type _type_1(std::string("bye"));
        type _type_2(std::string("bye"));

        return (_type_1 == _type_2);
    }
};

struct type_not_equal_to
{
    bool operator()()
    {

        type _type_1(std::string("bye"));
        type _type_2(std::string("hi"));

        return (_type_1 != _type_2);
    }
};

struct type_greather_than_1
{
    bool operator()()
    {

        type _type_1(std::string("bye"));
        type _type_2(std::string("byd"));

        return (_type_1 > _type_2);
    }
};

struct type_greather_than_2
{
    bool operator()()
    {

        type _type_1(std::string("bye"));
        type _type_2(std::string("byda"));

        return (_type_1 > _type_2);
    }
};

struct type_less_than_1
{
    bool operator()()
    {

        type _type_1(std::string("bye"));
        type _type_2(std::string("byf"));

        return (_type_1 < _type_2);
    }
};

struct type_less_than_2
{
    bool operator()()
    {

        type _type_1(std::string("byd"));
        type _type_2(std::string("byea"));

        return (_type_1 < _type_2);
    }
};

struct types_creation_from_string
{
    bool operator()()
    {
        types _types("; == < >=");

        if (_types.empty()) {
            cerr_error("types should not be empty");
            return false;
        }

        if (_types.size() != 4) {
            cerr_error("types size should be 4, but it is ", _types.size());
            return false;
        }
        {
            for (types::const_iterator _i = _types.begin(); _i != _types.end();
                 ++_i) {
                cerr_test("type = ", *_i);
            }
        }

        types::const_iterator _ite = _types.begin();
        if (*_ite != type(";")) {
            cerr_error("types 1 should ';', but it is ", *_ite);
            return false;
        }

        ++_ite;
        if (*_ite != type("<")) {
            cerr_error("types 2 should '<', but it is ", *_ite);
            return false;
        }

        ++_ite;
        if (*_ite != type("==")) {
            cerr_error("types 3 should '==', but it is ", *_ite);
            return false;
        }
        ++_ite;
        if (*_ite != type(">=")) {
            cerr_error("types 4 should '>=', but it is ", *_ite);
            return false;
        }

        return true;
    }
};

struct types_creation_from_string_and_add_more
{
    bool operator()()
    {
        types _types("; == < >=");

        if (_types.empty()) {
            cerr_error("types should not be empty");
            return false;
        }

        if (_types.size() != 4) {
            cerr_error("types size should be 4, but it is ", _types.size());
            return false;
        }

        _types.add(type("&"));
        _types.add(type("+="));

        {
            for (types::const_iterator _i = _types.begin(); _i != _types.end();
                 ++_i) {
                cerr_test("type = ", *_i);
            }
        }

        types::const_iterator _ite = _types.begin();
        if (*_ite != type("&")) {
            cerr_error("types 1 should '&', but it is ", *_ite);
            return false;
        }

        ++_ite;
        if (*_ite != type("+=")) {
            cerr_error("types 2 should '+=', but it is ", *_ite);
            return false;
        }

        ++_ite;
        if (*_ite != type(";")) {
            cerr_error("types 3 should '+=', but it is ", *_ite);
            return false;
        }

        ++_ite;
        if (*_ite != type("<")) {
            cerr_error("types 4 should '<', but it is ", *_ite);
            return false;
        }

        ++_ite;
        if (*_ite != type("==")) {
            cerr_error("types 5 should '==', but it is ", *_ite);
            return false;
        }
        ++_ite;
        if (*_ite != type(">=")) {
            cerr_error("types 6 should '>=', but it is ", *_ite);
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
    run_test(
      type_greather_than_1, argc, argv, "'type' greather_than, equal sizes");
    run_test(type_greather_than_2,
             argc,
             argv,
             "'type' greather_than, right longer than left, but left still "
             "typeicographically greater");
    run_test(type_less_than_1, argc, argv, "'type' less_than, equal sizes");
    run_test(type_less_than_2,
             argc,
             argv,
             "'type' less_than, right longer than left, but left still "
             "typeicographically less");
    run_test(types_creation_from_string,
             argc,
             argv,
             "'types' creation with a string with 4 type: ; == < >=");

    run_test(types_creation_from_string_and_add_more,
             argc,
             argv,
             "'types' creation with a string with 4 type: ; == < >=, and "
             "then added & and +=");
}

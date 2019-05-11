#include <iostream>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <interpreter/business/internal/tokens.h>
#include <interpreter/business/internal/type.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

struct recognize_relational_1_two
{
    bool operator()(const std::string& p_str)
    {
        return ((p_str == "<=") || (p_str == ">="));
    }
};

struct recognize_relational_1_one
{
    bool operator()(const std::string& p_str)
    {
        return ((p_str == "<") || (p_str == ">"));
    }
};

struct add_token_1
{

    bool operator()()
    {
        tokens _tokens;

        if (!_tokens.add(2,
                         type::id("relational-operator"),
                         recognize_relational_1_two())) {
            cerr_test("Unable to add 'relational-operator' with 2 chars");
            return false;
        }

        if (!_tokens.add(1,
                         type::id("relational-operator"),
                         recognize_relational_1_one())) {
            cerr_test("Unable to add 'relational-operator' with 1 char");
            return false;
        }

        return true;
    }
};

struct recognize_relational_1
{

    bool operator()()
    {
        tokens _tokens;

        if (!_tokens.add(2,
                         type::id("relational-operator"),
                         recognize_relational_1_two())) {
            cerr_test("Unable to add 'relational-operator' with 2 chars");
            return false;
        }

        if (!_tokens.add(1,
                         type::id("relational-operator"),
                         recognize_relational_1_one())) {
            cerr_test("Unable to add 'relational-operator' with 1 char");
            return false;
        }

        type _type = _tokens.recognize("<");

        if (_type == type::unreconized) {
            cerr_test("Should have recognized '<'");
            return false;
        }
        cerr_test("type recognized = '", _type, "'");
        return true;
    }
};

struct recognize_relational_2
{

    bool operator()()
    {
        tokens _tokens;

        if (!_tokens.add(2,
                         type::id("relational-operator"),
                         recognize_relational_1_two())) {
            cerr_test("Unable to add 'relational-operator' with 2 chars");
            return false;
        }

        if (!_tokens.add(1,
                         type::id("relational-operator"),
                         recognize_relational_1_one())) {
            cerr_test("Unable to add 'relational-operator' with 1 char");
            return false;
        }

        type _type = _tokens.recognize("<=");
        if (_type == type::unreconized) {
            cerr_test("Should have recognized '<='");
            return false;
        }

        cerr_test("type recognized = '", _type, "'");
        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(add_token_1,
             argc,
             argv,
             "add 'relational-operator' with one and two chars");

    run_test(
      recognize_relational_1, argc, argv, "tries to recognize a '<' operator");
    run_test(
      recognize_relational_2, argc, argv, "tries to recognize a '<=' operator");
}

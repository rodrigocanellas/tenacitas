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
            cerr_error("Unable to add 'relational-operator' with 2 chars");
            return false;
        }

        if (!_tokens.add(1,
                         type::id("relational-operator"),
                         recognize_relational_1_one())) {
            cerr_error("Unable to add 'relational-operator' with 1 char");
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
            cerr_error("Unable to add 'relational-operator' with 2 chars");
            return false;
        }

        if (!_tokens.add(1,
                         type::id("relational-operator"),
                         recognize_relational_1_one())) {
            cerr_error("Unable to add 'relational-operator' with 1 char");
            return false;
        }

        type _type = _tokens.recognize("<");

        if (_type == type::unreconized) {
            cerr_error("Should have recognized '<'");
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
            cerr_error("Unable to add 'relational-operator' with 2 chars");
            return false;
        }

        if (!_tokens.add(1,
                         type::id("relational-operator"),
                         recognize_relational_1_one())) {
            cerr_error("Unable to add 'relational-operator' with 1 char");
            return false;
        }

        type _type = _tokens.recognize("<=");
        if (_type == type::unreconized) {
            cerr_error("Should have recognized '<='");
            return false;
        }

        cerr_test("type recognized = '", _type, "'");
        return true;
    }
};

struct recognize_comparasion
{
    bool operator()()
    {
        tokens _tokens;

        if (!_tokens.add(type::id("comparison-operator"), "== !=")) {
            cerr_error("Unable to add 'comparison-operator'");
            return false;
        }

        type _type_1 = _tokens.recognize("!=");
        if (_type_1 == type::unreconized) {
            cerr_error("Should have recognized '!='");
            return false;
        }
        cerr_test("type 1 recognized = '", _type_1, "'");

        type _type_2 = _tokens.recognize("==");
        if (_type_2 == type::unreconized) {
            cerr_error("Should have recognized '=='");
            return false;
        }

        cerr_test("type 2 recognized = '", _type_2, "'");
        return true;
    }
};

struct wrong_tokens_string
{
    bool operator()()
    {
        tokens _tokens;
        try {
            _tokens.add(type::id("dummy"), "( !=");
        } catch (std::exception& _ex) {
            cerr_test("Exception thrown trying to add '( !=', as expected: '",
                      _ex.what(),
                      "'");
            return true;
        }

        cerr_error("An exception should have been thrown, but it was not");
        return false;
    }
};

struct dont_find_type
{
    bool operator()()
    {
        tokens _tokens;

        if (!_tokens.add(type::id("comparison-operator"), "== !=")) {
            cerr_error("Unable to add 'comparison-operator'");
            return false;
        }

        type _type_1 = _tokens.recognize("++");
        if (_type_1 != type::unreconized) {
            cerr_error("'++' should not have been recognized, but it was ");
            return false;
        }

        cerr_test("'++' was not recognized, as expected");
        return true;
    }
};

struct reserverd_words_size_3
{
    bool operator()()
    {
        tokens _tokens;
        _tokens.add(type::id("reserved-words-size-3"), "word1 word2 word3");
        type _type_1 = _tokens.recognize("word2");
        if (_type_1 == type::unreconized) {
            cerr_error("Should have recognized 'word2'");
            return false;
        }
        cerr_test("type 1 recognized = '", _type_1, "'");

        type _type_2 = _tokens.recognize("word3");
        if (_type_2 == type::unreconized) {
            cerr_error("Should have recognized 'word3'");
            return false;
        }
        cerr_test("type 2 recognized = '", _type_2, "'");

        type _type_3 = _tokens.recognize("word1");
        if (_type_3 == type::unreconized) {
            cerr_error("Should have recognized 'word1'");
            return false;
        }
        cerr_test("type 3 recognized = '", _type_3, "'");

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
    run_test(recognize_comparasion,
             argc,
             argv,
             "tries to recognize '==' and '!=' comparison operator");
    run_test(
      wrong_tokens_string, argc, argv, "error adding tokens string '( !='");
    run_test(dont_find_type,
             argc,
             argv,
             "trying to find a type for a token not added");
    run_test(reserverd_words_size_3,
             argc,
             argv,
             "recognizes reserved words of size 3");
}

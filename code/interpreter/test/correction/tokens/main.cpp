#include <iostream>
#include <sstream>
#include <string>

#include <interpreter/business/internal/tokens.h>
#include <interpreter/business/type.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

struct list_tokens_1
{
    bool operator()()
    {
        tokens _tokens;

        _tokens.add(type("relational-operator"), "< > <= >=");

        cerr_test("tokens: ", _tokens);

        return true;
    }
};

struct list_tokens_2
{
    bool operator()()
    {
        tokens _tokens;

        _tokens.add(type("relational-operator"), "< > <= >=");
        _tokens.add(type("reserved-word"), "while if for");

        cerr_test("tokens: ", _tokens);

        return true;
    }
};

struct recognize_1
{
    bool operator()()
    {
        tokens _tokens;

        _tokens.add(type("relational-operator"), "< > <= >=");
        _tokens.add(type("reserved-word"), "while if for");

        type _type = _tokens.recognize("<");
        if (_type == type::undefined) {
            cerr_error("'<' should have been recognized, but it was not");
            return false;
        }
        if (_type != type("relational-operator")) {
            cerr_error("'<' should have been recognized as "
                       "'relational-operator', but it was recognized as '",
                       _type,
                       "'");
            return false;
        }
        cerr_test("'<' recognized as '", _type, "'");
        return true;
    }
};

struct recognize_2
{
    bool operator()()
    {
        tokens _tokens;

        _tokens.add(type("relational-operator"), "< > <= >=");
        _tokens.add(type("reserved-word"), "while if for");

        type _type = _tokens.recognize("!");
        if (_type != type::undefined) {
            cerr_error("'!' should not have been recognized, but it not as '",
                       _type,
                       "'");
            return false;
        }
        cerr_test("'!' was not recognized");
        return true;
    }
};

struct recognize_3
{
    bool operator()()
    {
        tokens _tokens;

        _tokens.add(type("relational-operator"), "< > <= >=");
        _tokens.add(type("reserved-word"), "while if for");

        type _type = _tokens.recognize(">=");
        if (_type == type::undefined) {
            cerr_error("'<' should have been recognized, but it was not");
            return false;
        }
        if (_type != type("relational-operator")) {
            cerr_error("'<' should have been recognized as "
                       "'relational-operator', but it was recognized as '",
                       _type,
                       "'");
            return false;
        }
        cerr_test("'>=' recognized as '", _type, "'");

        _type = _tokens.recognize(">");
        if (_type == type::undefined) {
            cerr_error("'<' should have been recognized, but it was not");
            return false;
        }
        if (_type != type("relational-operator")) {
            cerr_error("'<' should have been recognized as "
                       "'relational-operator', but it was recognized as '",
                       _type,
                       "'");
            return false;
        }
        cerr_test("'>' recognized as '", _type, "'");

        _type = _tokens.recognize("if");
        if (_type == type::undefined) {
            cerr_error("'<' should have been recognized, but it was not");
            return false;
        }
        if (_type != type("reserved-word")) {
            cerr_error("'if' should have been recognized as "
                       "'reserved-word', but it was recognized as '",
                       _type,
                       "'");
            return false;
        }
        cerr_test("'if' recognized as '", _type, "'");
        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(list_tokens_1,
             argc,
             argv,
             "list 'relational-operators' tokens '< > <= >='");

    run_test(list_tokens_2,
             argc,
             argv,
             "list 'relational-operators' tokens '< > <= >=', and "
             "'reserved-words' 'while for if'");

    run_test(
      recognize_1, argc, argv, "should recognize '<' as 'relational-operator'");

    run_test(recognize_2, argc, argv, "'!' should not be recognized");
    run_test(
      recognize_3, argc, argv, "'>=', '>' and 'if' should not be recognized");
}

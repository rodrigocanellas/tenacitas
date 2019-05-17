#include <iostream>
#include <sstream>
#include <string>

#include <interpreter/business/internal/tokens.h>
#include <interpreter/business/internal/type.h>
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
}

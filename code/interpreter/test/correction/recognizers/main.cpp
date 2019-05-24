#include <iostream>
#include <sstream>
#include <string>

#include <interpreter/business/internal/recognizers.h>
#include <interpreter/business/type.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

static const type word("word");
static const type integer("integer");

struct integer_recognizer
{
    type operator()(const std::string& p_str)
    {

        for (std::string::value_type _c : p_str) {
            if (std::isdigit(_c) == 0) {
                return type::undefined;
            }
        }
        return integer;
    }
};

struct word_recognizer
{
    type operator()(const std::string& p_str)
    {
        for (std::string::value_type _c : p_str) {
            if (std::isalpha(_c) == 0) {
                return type::undefined;
            }
        }
        return word;
    }
};

struct recognize_1
{
    bool operator()()
    {
        recognizers _recognizers;

        _recognizers.add(integer_recognizer());

        type _type = _recognizers("1245");

        if (_type == type::undefined) {
            cerr_error(
              "'1245' was not recognized, but it should have been as '",
              integer,
              "'");
        }

        if (_type != type("integer")) {
            cerr_error("'1245' was recognized, but not as '",
                       integer,
                       "', but as ",
                       _type);
            return false;
        }
        cerr_test("'1245' was recognized as '", _type, "'");
        return true;
    }
};

struct recognize_2
{
    bool operator()()
    {
        recognizers _recognizers;

        _recognizers.add(integer_recognizer());
        _recognizers.add(word_recognizer());

        type _type = _recognizers("1245");

        if (_type == type::undefined) {
            cerr_error(
              "'1245' was not recognized, but it should have been as '",
              integer,
              "'");
        }

        if (_type != type("integer")) {
            cerr_error("'1245' was recognized, but not as '",
                       integer,
                       "', but as ",
                       _type);
            return false;
        }
        cerr_test("'1245' was recognized as '", _type, "'");
        return true;
    }
};

struct recognize_3
{
    bool operator()()
    {
        recognizers _recognizers;

        _recognizers.add(integer_recognizer());
        _recognizers.add(word_recognizer());

        type _type = _recognizers("bycicle");

        if (_type == type::undefined) {
            cerr_error(
              "'bycicle' was not recognized, but it should have been as '",
              word,
              "'");
        }

        if (_type != word) {
            cerr_error("'bycicle' was recognized, but not as '",
                       word,
                       "', but as ",
                       _type);
            return false;
        }
        cerr_test("'bycicle' was recognized as '", _type, "'");
        return true;
    }
};

struct recognize_4
{
    bool operator()()
    {
        recognizers _recognizers;

        _recognizers.add(integer_recognizer());
        _recognizers.add(word_recognizer());

        type _type = _recognizers("bycicle+");

        if (_type != type::undefined) {
            cerr_error(
              "'bycicle+' was recognized as '", word, "', but it should not");
        }

        cerr_test("'bycicle+' was not recognized");
        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(recognize_1, argc, argv, "tries to recognize '1245' as 'integer'");
    run_test(recognize_2,
             argc,
             argv,
             "tries to recognize '1245' as 'integer', in a set of recognizers "
             "that has also a 'word_recognizer'");
    run_test(recognize_3,
             argc,
             argv,
             "tries to recognize 'bycicle' as 'word', in a set of recognizers "
             "that has also a 'integer_recognizer'");
    run_test(recognize_4,
             argc,
             argv,
             "tries to recognize 'bycicle+' in a set of recognizers "
             "that has a 'integer_recognizer' and a 'word_recognizer");
}

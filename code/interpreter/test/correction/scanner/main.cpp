#include <iostream>
#include <sstream>
#include <string>

#include <interpreter/business/internal/scanner.h>
#include <interpreter/business/internal/symbol.h>
#include <interpreter/business/type.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::interpreter::business;

static const type relational_operator("relational-operator");
static const type equality_operator("equality-operator");
static const type reserved_words("reserved-words");
static const type integer("integer");
static const type word("word");

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

struct scanner_1
{

    bool operator()()
    {
        scanner _scanner;
        const std::string _text("hi");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        while (true) {
            symbol _symbol(_scanner.get_symbol());
            if (_symbol == symbol::eot) {
                break;
            }
            type _type(_symbol.get_type());
            if (_type != type::undefined) {
                cerr_error("'",
                           _symbol.get_lexeme(),
                           "' should not have been recognized, but it was as '",
                           _type,
                           "'. Line ",
                           _scanner.get_line(),
                           ", column ",
                           _scanner.get_column());
            }
            cerr_test("'",
                      _symbol.get_lexeme(),
                      "' not recognized as '",
                      _type,
                      "' as expected");
        }
        return true;
    }
};

struct scanner_2
{

    bool operator()()
    {
        scanner _scanner;
        const std::string _text("hi 123");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        while (true) {
            symbol _symbol(_scanner.get_symbol());
            if (_symbol == symbol::eot) {
                break;
            }
            type _type(_symbol.get_type());
            if (_type != type::undefined) {
                cerr_error("'",
                           _symbol.get_lexeme(),
                           "' should not have been recognized, but it was as '",
                           _type,
                           "'. Line ",
                           _scanner.get_line(),
                           ", column ",
                           _scanner.get_column());
                return false;
            }
            cerr_test("'",
                      _symbol.get_lexeme(),
                      "' not recognized as '",
                      _type,
                      "' as expected");
        }
        return true;
    }
};

struct scanner_3
{

    bool operator()()
    {
        scanner _scanner;

        _scanner.add_tokens(relational_operator, "< > >= <=");
        _scanner.add_tokens(equality_operator, "== !=");
        _scanner.add_tokens(reserved_words, "if for while break");

        const std::string _text("for != while");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        while (true) {
            symbol _symbol(_scanner.get_symbol());
            if (_symbol == symbol::eot) {
                break;
            }
            type _type(_symbol.get_type());
            if (_type == type::undefined) {
                cerr_error(
                  "'",
                  _symbol.get_lexeme(),
                  "' should have been recognized, but it was as not. Line ",
                  _scanner.get_line(),
                  ", column ",
                  _scanner.get_column());
                return false;
            }
            cerr_test("'",
                      _symbol.get_lexeme(),
                      "' recognized as '",
                      _type,
                      "' as expected");
        }
        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(scanner_1,
             argc,
             argv,
             "'scanner' with no token or recognizer registered, and text with "
             "'hi'; so only 'type::undefined' should be found");

    run_test(scanner_2,
             argc,
             argv,
             "'scanner' with no token or recognizer registered, and text with "
             "'hi 123'; so only 'type::undefined' should be found");

    run_test(scanner_3,
             argc,
             argv,
             "'scanner' recognizes < > <= >= == != if for while break, and "
             "text is 'for != while'");
}

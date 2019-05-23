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
static const type assignment_operator("assignment-operator");
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
            cerr_test(
              "'", _symbol.get_lexeme(), "' not recognized as expected");
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
            cerr_test(
              "'", _symbol.get_lexeme(), "' not recognized as expected");
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

struct scanner_4
{

    bool operator()()
    {
        scanner _scanner;

        const std::string _text;
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        while (true) {
            symbol _symbol(_scanner.get_symbol());
            if (_symbol != symbol::eot) {
                cerr_error("Text is empty, so first 'get_symbol' should be "
                           "'symbol::eot', but it is not");
                return false;
            }
            cerr_test("Text is empty, and the first 'get_symbol' is "
                      "'symbol::eot', as it should");
            break;
        }
        return true;
    }
};

struct scanner_5
{

    bool operator()()
    {
        scanner _scanner;

        const std::string _text("    ");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        while (true) {
            symbol _symbol(_scanner.get_symbol());
            if (_symbol != symbol::eot) {
                cerr_error(
                  "Text is made of spaces, so first 'get_symbol' should be "
                  "'symbol::eot', but it is not");
                return false;
            }
            cerr_test("Text is made of spaces, and the first 'get_symbol' is "
                      "'symbol::eot', as it should");
            break;
        }
        return true;
    }
};

struct scanner_6
{

    bool operator()()
    {
        scanner _scanner;

        const std::string _text("\n\n\n");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        while (true) {
            symbol _symbol(_scanner.get_symbol());
            if (_symbol != symbol::eot) {
                cerr_error(
                  "Text is made of CRs, so first 'get_symbol' should be "
                  "'symbol::eot', but it is not");
                return false;
            }
            cerr_test("Text is made of CRs, and the first 'get_symbol' is "
                      "'symbol::eot', as it should");
            break;
        }
        return true;
    }
};

struct scanner_7
{

    bool operator()()
    {
        scanner _scanner;

        _scanner.add_tokens(relational_operator, "< > >= <=");
        _scanner.add_tokens(equality_operator, "== !=");
        _scanner.add_tokens(reserved_words, "if for while break");

        const std::string _text("for != > while");
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

struct scanner_8
{
    bool operator()()
    {
        scanner _scanner;
        _scanner.add_tokens(relational_operator, "< > >= <=");
        _scanner.add_tokens(equality_operator, "== !=");
        _scanner.add_tokens(reserved_words, "if for while break");

        const std::string _text("!=>");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        symbol _symbol = _scanner.get_symbol();
        type _type(_symbol.get_type());
        if (_type != type::undefined) {
            cerr_error("'!=>' was recognized as '", _type, "'");
            return false;
        }
        cerr_test("'!=>' was not recognized, as expected");
        return true;
    }
};

struct scanner_9
{
    bool operator()()
    {
        scanner _scanner;
        _scanner.add_tokens(relational_operator, "< > >= <=");
        _scanner.add_tokens(equality_operator, "== !=");
        _scanner.add_tokens(reserved_words, "if for while break");
        _scanner.add_tokens(assignment_operator, "=");

        const std::string _text("=");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        symbol _symbol = _scanner.get_symbol();
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
        cerr_test("'=' was recognized as '", _type, "'");
        return true;
    }
};

struct scanner_10
{

    bool operator()()
    {
        scanner _scanner;

        _scanner.add_tokens(relational_operator, "< > >= <=");
        _scanner.add_tokens(equality_operator, "== !=");
        _scanner.add_tokens(reserved_words, "if for while break");
        _scanner.add_tokens(assignment_operator, "=");

        const std::string _text("for = != = > while");
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

struct scanner_11
{

    bool operator()()
    {
        scanner _scanner;

        _scanner.add_tokens(relational_operator, "< > >= <=");
        _scanner.add_tokens(equality_operator, "== !=");
        _scanner.add_tokens(reserved_words, "if for while break");
        _scanner.add_tokens(assignment_operator, "=");

        const std::string _text("for !==");
        _scanner.set_text(_text.begin(), _text.end());
        cerr_test("text = '", _text, "'");
        while (true) {
            symbol _symbol(_scanner.get_symbol());
            if (_symbol == symbol::eot) {
                break;
            }
            type _type(_symbol.get_type());
            if (_type == type::undefined) {
                if (_symbol.get_lexeme() == lexeme("for")) {
                    cerr_error(
                      "'",
                      _symbol.get_lexeme(),
                      "' should have been recognized, but it was as not. Line ",
                      _scanner.get_line(),
                      ", column ",
                      _scanner.get_column());
                    return false;
                }
                if (_symbol.get_lexeme() == lexeme("!==")) {
                    cerr_test("'",
                              _symbol.get_lexeme(),
                              "' was not recognized as expected");
                }
            } else {
                if (_symbol.get_lexeme() == lexeme("!==")) {
                    cerr_error(
                      "'",
                      _symbol.get_lexeme(),
                      "' should not have been recognized, but it was. Line ",
                      _scanner.get_line(),
                      ", column ",
                      _scanner.get_column());
                    return false;
                }
                if (_symbol.get_lexeme() == lexeme("for")) {
                    cerr_test("'",
                              _symbol.get_lexeme(),
                              "' was recognized as '",
                              _type,
                              "' as expected");
                }
            }
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

    run_test(scanner_4,
             argc,
             argv,
             "scanning empty text, should recognize only 'symbol::eot'");

    run_test(
      scanner_5,
      argc,
      argv,
      "scanning text made of space, should recognize only 'symbol::eot'");

    run_test(scanner_6,
             argc,
             argv,
             "scanning text made of CRs, should recognize only 'symbol::eot'");

    run_test(scanner_7,
             argc,
             argv,
             "'scanner' recognizes < > <= >= == != if for while break, and "
             "text is 'for != > while', which should be all recognized");

    run_test(scanner_8,
             argc,
             argv,
             "'scanner' recognizes < > <= >= == != if for while break, and "
             "text is '!=>', which should not be recognized");

    run_test(scanner_9,
             argc,
             argv,
             "'scanner' recognizes = < > <= >= == != if for while break, and "
             "text is '=', which should be recognized");

    run_test(scanner_10,
             argc,
             argv,
             "'scanner' recognizes = < > <= >= == != if for while break, and "
             "text is 'for = != = > while', which should be all recognized");

    run_test(
      scanner_11,
      argc,
      argv,
      "'scanner' recognizes = < > <= >= == != if for while break, and "
      "text is 'for  !=='. 'for' should be recognized, but '!==' should not");
}

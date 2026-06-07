/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

<<<<<<< HEAD
// #include "recognizer_test.h"
// #include "grammar_test.h"
// #include "parser_test.h"
#include "tnct/interpreter/tst/non_terminal_standard_recognizers_test.h"
#include "tnct/interpreter/tst/scanner_test.h"
#include "tnct/interpreter/tst/terminals_test.h"
=======
// #include "scanner_test.h"
// #include "grammar_test.h"
// #include "parser_test.h"
#include "tnct/interpreter/tst/standard_scanners_test.h"
>>>>>>> eb7334ef (begining)
#include "tnct/tester/bus/test.h"

using namespace tnct;

int main(int argc, char **argv) {
  tester::bus::test _tester(argc, argv);

<<<<<<< HEAD
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_000);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_001);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_002);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_003);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_004);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_005);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_006);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_007);
  run_test(_tester,
           tnct::interpreter::tst::non_terminal_standard_recognizers_008);

  run_test(_tester, tnct::interpreter::tst::terminals_000);
  run_test(_tester, tnct::interpreter::tst::terminals_001);
  run_test(_tester, tnct::interpreter::tst::terminals_002);
  run_test(_tester, tnct::interpreter::tst::terminals_003);
  run_test(_tester, tnct::interpreter::tst::terminals_004);
  run_test(_tester, tnct::interpreter::tst::terminals_005);
  run_test(_tester, tnct::interpreter::tst::terminals_006);
  run_test(_tester, tnct::interpreter::tst::terminals_007);
  run_test(_tester, tnct::interpreter::tst::terminals_008);

  run_test(_tester, tnct::interpreter::tst::scanner_000);
  run_test(_tester, tnct::interpreter::tst::scanner_001);
  run_test(_tester, tnct::interpreter::tst::scanner_002);
  run_test(_tester, tnct::interpreter::tst::scanner_003);
  run_test(_tester, tnct::interpreter::tst::scanner_004);
  run_test(_tester, tnct::interpreter::tst::scanner_005);
  run_test(_tester, tnct::interpreter::tst::scanner_006);
  run_test(_tester, tnct::interpreter::tst::scanner_007);
  run_test(_tester, tnct::interpreter::tst::scanner_008);
  run_test(_tester, tnct::interpreter::tst::scanner_009);
  run_test(_tester, tnct::interpreter::tst::scanner_010);
  run_test(_tester, tnct::interpreter::tst::scanner_011);
  run_test(_tester, tnct::interpreter::tst::scanner_012);
  run_test(_tester, tnct::interpreter::tst::scanner_013);
  run_test(_tester, tnct::interpreter::tst::scanner_014);
  run_test(_tester, tnct::interpreter::tst::scanner_015);
=======
  run_test(_tester, tnct::interpreter::tst::standard_scanners_000);
  run_test(_tester, tnct::interpreter::tst::standard_scanners_001);
  run_test(_tester, tnct::interpreter::tst::standard_scanners_002);
  run_test(_tester, tnct::interpreter::tst::standard_scanners_003);
>>>>>>> eb7334ef (begining)
}

// int main ( int argc, char ** argv ) {

//     if ( argc != 2 ) {
//         std::cout << "Syntax : " << argv [0] << " <file-name> \n"
//                   << "Where <file-name> is the name of the file that "
//                   << "contains the text to be lexicaly analysed"
//                   << std::endl;
//     }
//     else {
// 	try {
<<<<<<< HEAD
// 	    test_recognizer l_test_recongnizer;
// 	    if ( !l_test_recognizer.exec ( ) ) {
=======
// 	    test_scanner l_test_scanner;
// 	    if ( !l_test_scanner.exec ( ) ) {
>>>>>>> eb7334ef (begining)
// 		std::cout << "ERROR on 'test_lexical' " ;
// 	    }
// 	    else {
// 		std::cout << "SUCCESS for 'test_lexical' ";
// 	    }
// 	    std::cout << std::endl << std::endl;

// 	    test_grammar_1 l_test_grammar_1;
// 	    if ( !l_test_grammar_1.exec (  ) ) {
// 		std::cout << "ERROR on 'test_grammar_1' " ;
// 	    }
// 	    else {
// 		std::cout << "SUCCESS for 'test_grammar_1' ";
// 	    }
// 	    std::cout << std::endl << std::endl;

//             test_parser_1 l_test_parser_1;
// 	    if ( !l_test_parser_1.exec ( argv[1] ) ) {
// 		std::cout << "ERROR on 'test_parser_1' " ;
// 	    }
// 	    else {
// 		std::cout << "SUCCESS for 'test_parser_1' ";
// 	    }
// 	    std::cout << std::endl << std::endl;
// 	}
// 	catch ( interpreter::fault & e ) {
// 	    cout << "Error: " << e.what ( ) << endl;
// 	}
// 	catch ( tenacitas::number::fault & e ) {
// 	    cout << "FAULT: " << e.what ( ) ;
// 	}

//         std::cout << std::endl;
//     }

//     return 0;
// }

// #include "scanner_test.h"

// using namespace tenacitas;

// using namespace tenacitas::interpreter;

// using namespace tenacitas::interpreter::test;

// //============================================================================

// //============================================================================
// //                                     test_scanner

// //----------------------------------------------------------------------------
// bool
// test_scanner::
// exec ( ) {

//     bool rc = true;

//     memory::ptr<scanner> lex;

//     try {
//         string line;
//         string text ( "palavraA palavraB\n"
//                       "palavraC      \n"
//                       "palavraD\n"
//                       "abcd 12345\n"
//                       "12345\n"
//                       "abcd 12345\n"
//                       "\n"
//                       "=              \n"
//                       "\n"
//                       "xyz wsa 112 0987\n"
//                       "Z ==\n"
//                       "3\n"
//                       "abc\n"
//                       "def\n"
//                       "= ==\n"
//                       "abc\n"
//                       "def 123,45 984 escondido\n"
//                       "\n"
//                       "\n" ) ;

//         memory::ptr<tokens> l_tokens ( new tokens ( ) );

//         l_tokens->add ( string ( "= ==" ), symbol::type ( "reserved" ) ) ;

//         memory::ptr<type_recognizers>
//             l_type_recognizers ( new type_recognizers ( ) )  ;

//         l_type_recognizers->add ( memory::ptr<type_recognizer>
//                                   ( new word_recognizer ( ) ) ) ;

//         l_type_recognizers->add ( memory::ptr<type_recognizer>
//                                   ( new number_recognizer ( ',' ) ) ) ;

//         lex = memory::ptr<scanner> ( new scanner ( ) )  ;

//         lex->set_tokens ( l_tokens ) ;
//         lex->set_type_recognizers ( l_type_recognizers ) ;

//         lex->set_text ( text.begin( ) , text.end( ) ) ;

//         memory::ptr<const symbol> symb;

//         while ( true ) {

//             symb = lex->get_symbol ( ) ;
//             if (  symb->get_value ( ) == symbol::value::eot ) break;

//             cout << "'" << symb->get_value ( ) << "', "
//                  << "'" << symb->get_type ( ) << "'"
//                  << endl;
//         }
//     }
//     catch ( interpreter::fault & e ) {
//         cout << "Error: " << e.what ( ) << endl;
//         rc = false;
//     }
//     catch ( number::fault & e ) {
//         cout << "Error: " << e.what ( ) << endl;
//         rc = false;
//     }

//     return rc;
// }

// #include "parser_test.h"

// using namespace tenacitas;

// using namespace tenacitas::interpreter;

// using namespace tenacitas::interpreter::test;

// //============================================================================
// //                                     test_parser_1

// //----------------------------------------------------------------------------
// bool
// test_parser_1::
// exec ( const string & p_file_name ) {

//     bool rc = true;

//     ifstream file ( p_file_name.c_str ( ) );
//     if ( !file.good ( ) ) {
//         cout << "Unable to open file " << p_file_name << endl;
//         rc = false;
//     }
//     else {

// 	try {
// 	    string line;
// 	    string text;

// 	    while ( !file.eof ( ) ) {
// 		getline ( file, line ) ;
// 		text.append( line ) ;
// 		text.append( string ( "\n" ) ) ;
// 	    }

//             parser l_parser ;

//             l_parser.parse<expression_evaluator> ( text.begin ( ) ,
//                                                   text.end ( )// ,
// //                                                   true
//                 ) ;
//         }
// 	catch ( interpreter::fault & e ) {
// 	    cout << "Error: " << e.what ( ) << endl;
// 	    rc = false;
// 	}
// 	catch ( number::fault & e ) {
// 	    cout << "Error: " << e.what ( ) << endl;
// 	    rc = false;
// 	}
//     }
//     return rc;
// }

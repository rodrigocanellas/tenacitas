// #include "grammar_test.h"

// using namespace tenacitas;

// using namespace tenacitas::interpreter;

// using namespace tenacitas::interpreter::test;

// //============================================================================
// //                                     expression

// //----------------------------------------------------------------------------
// expression::
// expression ( ) :
//     grammar ( grammar_id ( "expression" ) ) {

//     /// values recognizeds
//     add_token ( string ( "==  < > != <= >= + - || * / mod && ( ) !" ),
//                  symbol::type ( "reserved" ) ) ;

//     /// types  recognizeds
//     add_type_recognizer ( memory::ptr<number_recognizer>
//                           ( new number_recognizer ( ) ) );

//     /// productions

//     ///     identifiers
//     production_id expression ( "expression" );
//     production_id simple_exp ( "simple-exp" ) ;
//     production_id term ( "term" ) ;
//     production_id factor ( "factor" ) ;

//     ///     adding
//     add_production ( expression ) ;
//     add_production ( simple_exp ) ;
//     add_production ( term ) ;
//     add_production ( factor ) ;

//     ///     expression

//     add_item ( expression, 0 ) ;
//     add_item ( expression, 1 ) ;
//     add_item ( expression, 2 ) ;
//     add_item ( expression, 3 ) ;
//     add_item ( expression, 4 ) ;
//     add_item ( expression, 5 ) ;
//     add_item ( expression, 6 ) ;
//     add_item ( expression, 7 ) ;

//     set_production ( expression, 0, simple_exp ) ;
//     set_production ( expression, 7, simple_exp ) ;

//     set_value      ( expression, 1, symbol::value ( "==" ) ) ;
//     set_value      ( expression, 2, symbol::value ( "<" ) ) ;
//     set_value      ( expression, 3, symbol::value ( ">" ) ) ;
//     set_value      ( expression, 4, symbol::value ( "!=" ) ) ;
//     set_value      ( expression, 5, symbol::value ( ">=" ) ) ;
//     set_value      ( expression, 6, symbol::value ( "<=" ) ) ;

//     set_suc ( expression, 0, 1 ) ;
//     set_suc ( expression, 1, 7 ) ;
//     set_suc ( expression, 2, 7 ) ;
//     set_suc ( expression, 3, 7 ) ;
//     set_suc ( expression, 4, 7 ) ;
//     set_suc ( expression, 5, 7 ) ;
//     set_suc ( expression, 6, 7 ) ;

//     set_alt ( expression, 1, 2 ) ;
//     set_alt ( expression, 2, 3 ) ;
//     set_alt ( expression, 3, 4 ) ;
//     set_alt ( expression, 4, 5 ) ;
//     set_alt ( expression, 5, 6 ) ;

//     accepts_anything ( expression, 6 ) ;

//     ///     simple-exp

//     add_item ( simple_exp, 0 ) ;
//     add_item ( simple_exp, 1 ) ;
//     add_item ( simple_exp, 2 ) ;
//     add_item ( simple_exp, 3 ) ;
//     add_item ( simple_exp, 4 ) ;
//     add_item ( simple_exp, 5 ) ;

//     set_production ( simple_exp, 2, term ) ;

//     set_value ( simple_exp, 0, symbol::value ( "+" ) ) ;
//     set_value ( simple_exp, 1, symbol::value ( "-" ) ) ;
//     set_value ( simple_exp, 3, symbol::value ( "+" ) ) ;
//     set_value ( simple_exp, 4, symbol::value ( "-" ) ) ;
//     set_value ( simple_exp, 5, symbol::value ( "||" ) ) ;

//     set_suc ( simple_exp, 0, 2 ) ;
//     set_suc ( simple_exp, 1, 2 ) ;
//     set_suc ( simple_exp, 2, 3 ) ;
//     set_suc ( simple_exp, 3, 2 ) ;
//     set_suc ( simple_exp, 4, 2 ) ;
//     set_suc ( simple_exp, 5, 2 ) ;

//     set_alt ( simple_exp, 0, 1 ) ;
//     set_alt ( simple_exp, 1, 2 ) ;
//     set_alt ( simple_exp, 3, 4 ) ;
//     set_alt ( simple_exp, 4, 5 ) ;

//     accepts_anything ( simple_exp, 5 ) ;

//     ///     term

//     add_item ( term, 0 ) ;
//     add_item ( term, 1 ) ;
//     add_item ( term, 2 ) ;
//     add_item ( term, 3 ) ;
//     add_item ( term, 4 ) ;

//     set_production ( term, 0, factor ) ;

//     set_value ( term, 1, symbol::value ( "*" ) ) ;
//     set_value ( term, 2, symbol::value ( "/" ) ) ;
//     set_value ( term, 3, symbol::value ( "mod" ) ) ;
//     set_value ( term, 4, symbol::value ( "&&" ) ) ;

//     set_suc ( term, 0, 1 ) ;
//     set_suc ( term, 1, 0 ) ;
//     set_suc ( term, 2, 0 ) ;
//     set_suc ( term, 3, 0 ) ;
//     set_suc ( term, 4, 0 ) ;

//     set_alt ( term, 1, 2 ) ;
//     set_alt ( term, 2, 3 ) ;
//     set_alt ( term, 3, 4 ) ;

//     accepts_anything ( term, 4 ) ;

//     ///     factor

//     add_item ( factor, 0 ) ;
//     add_item ( factor, 1 ) ;
//     add_item ( factor, 2 ) ;
//     add_item ( factor, 3 ) ;
//     add_item ( factor, 4 ) ;
//     add_item ( factor, 5 ) ;
//     add_item ( factor, 6 ) ;

//     set_production ( factor, 3, expression ) ;
//     set_production ( factor, 6, factor ) ;

//     set_value ( factor, 2, symbol::value ( "(" ) ) ;
//     set_value ( factor, 4, symbol::value ( ")" ) ) ;
//     set_value ( factor, 5, symbol::value ( "!" ) ) ;

//     set_type ( factor, 0, symbol::type ( "decimal_integer" ) ) ;
//     set_type ( factor, 1, symbol::type ( "real" ) ) ;

//     set_suc ( factor, 2, 3 ) ;
//     set_suc ( factor, 3, 4 ) ;
//     set_suc ( factor, 5, 6 ) ;

//     set_alt ( factor, 0, 1 ) ;
//     set_alt ( factor, 1, 2 ) ;
//     set_alt ( factor, 2, 5 ) ;

// }

// //----------------------------------------------------------------------------
// expression::
// ~expression ( ) {}

// //============================================================================
// //                                     expression_evaluator

// //----------------------------------------------------------------------------
// expression_evaluator::
// expression_evaluator ( ) :
//     expression ( ) {

//     /// routines
//     add_production_routine ( memory::ptr<factor_routine>
//                              ( new factor_routine ( ) ) ) ;

//     add_production_routine ( memory::ptr<term_routine>
//                              ( new term_routine ( ) ) ) ;

//     add_production_routine ( memory::ptr<simple_exp_routine>
//                              ( new simple_exp_routine ( ) ) ) ;

//     add_production_routine ( memory::ptr<expression_routine>
//                              ( new expression_routine ( ) ) ) ;
// }

// //----------------------------------------------------------------------------
// expression_evaluator::
// ~expression_evaluator ( ) {}

// //============================================================================
// //            expression_evaluator::expression_evaluator_routine

// //----------------------------------------------------------------------------
// expression_evaluator::expression_evaluator_routine::
// expression_evaluator_routine ( const routine_id & p_routine_id,
//                                const production_id & p_production_id ) :
//     production_routine ( p_routine_id, p_production_id ) {}

// //----------------------------------------------------------------------------
// expression_evaluator::expression_evaluator_routine::
// ~expression_evaluator_routine ( ) {}

// //----------------------------------------------------------------------------
// bool
// expression_evaluator::expression_evaluator_routine::
// is_real ( symbols::const_iterator p_symb_1 ) const {
//     return ( ( *p_symb_1 )->get_type ( ) ==
//              symbol::type ( "real" ) ) ;
// }

// //----------------------------------------------------------------------------
// bool
// expression_evaluator::expression_evaluator_routine::
// is_real ( symbols::const_iterator p_symb_1,
//           symbols::const_iterator p_symb_2) const {

//     return  ( ( ( *p_symb_1 )->get_type ( ) ==
//                 symbol::type ( "real" ) )
//               ||
//               ( ( *p_symb_2 )->get_type ( ) ==
//                 symbol::type ( "real" ) ) );
// }

// //============================================================================
// //                                expression_evaluator::expression_routine

// //----------------------------------------------------------------------------
// expression_evaluator::expression_routine::
// expression_routine ( ) :
//     expression_evaluator_routine ( routine_id ( "expression_routine" ),
//                                    production_id ( "expression" ) )  {}

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::expression_routine::
// exec( symbols::iterator p_first,
//       symbols::iterator p_end ) const {

//     memory::ptr<symbol> rc;
//     cerr << "\n---------------\n"
//          << "'expression_routine' - IN" << endl;

//     symbols::size_type dist =
//         static_cast<symbols::size_type>
//         ( std::distance( p_first, p_end ) ) ;

//     cerr << "Number of parameters = '" << dist << "'" << endl;

//     switch ( dist ) {
//     case 1:
//         rc = *p_first;
//         break;

//     case 3:
//     {
//         symbols::iterator l_op = p_first;
//         ++l_op;
//         symbols::iterator l_second = l_op;
//         ++l_second;

//         if ( ( *l_op )->get_value ( ) == symbol::value ( "==" ) ) {
//             rc = equal_to ( p_first, l_second );
//         }
//         if ( ( *l_op )->get_value ( ) == symbol::value ( "!=" ) ) {
//             rc = not_equal_to ( p_first, l_second ) ;
//         }
//         if ( ( *l_op )->get_value ( ) == symbol::value ( "<" ) ) {
//             rc = less_than ( p_first, l_second ) ;
//         }
//         if ( ( *l_op )->get_value ( ) == symbol::value ( "<=" ) ) {
//             rc = less_or_equal_to ( p_first, l_second ) ;
//         }
//         if ( ( *l_op )->get_value ( ) == symbol::value ( ">" ) ) {
//             rc = greater_than ( p_first, l_second ) ;
//         }
//         if ( ( *l_op )->get_value ( ) == symbol::value ( "<=" ) ) {
//             rc = greater_or_equal_to ( p_first, l_second ) ;
//         }

//     }
//     default:
//         cerr << "Invalid number of parameters" << endl;
//     }

//     if ( rc ) {
//         cerr << "returning: "
//              << "'" << rc->get_value ( ) << "', "
//              << "'" << rc->get_type ( ) << "'"
//              << endl;

//         cout << "\n\n\nRESULT OF THE EXPRESSION: "
//              << "'" << rc->get_value ( ) << "', "
//              << "'" << rc->get_type ( ) << "'"
//              << "\n\n\n" << endl;
//     }

//     cerr << "\n'expression_routine' - OUT" << endl;

//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::expression_routine::
// equal_to ( symbols::iterator p_symb_1,
//            symbols::iterator p_symb_2 ) const {

//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) ==
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) ==
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::expression_routine::
// less_than ( symbols::iterator p_symb_1,
//             symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) <
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) <
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::expression_routine::
// greater_than ( symbols::iterator p_symb_1,
//                symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) >
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) >
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::expression_routine::
// not_equal_to ( symbols::iterator p_symb_1,
//                symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) !=
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) !=
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::expression_routine::
// greater_or_equal_to ( symbols::iterator p_symb_1,
//                       symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) >=
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) >=
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::expression_routine::
// less_or_equal_to ( symbols::iterator p_symb_1,
//                    symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) <=
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) <=
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //============================================================================
// //                              expression_evaluator::simple_exp_routine

// //----------------------------------------------------------------------------
// expression_evaluator::simple_exp_routine::
// simple_exp_routine ( ) :
//     expression_evaluator_routine ( routine_id ( "production_routine" ) ,
//                                    production_id ( "simple-exp" ) ) {}

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::simple_exp_routine::
// exec( symbols::iterator p_first,
//       symbols::iterator p_end ) const {

//     memory::ptr<symbol> rc;

//     cerr << "\n---------------\n"
//          << "'simple_exp_routine' - IN" << endl;

//     symbols::size_type dist =
//         static_cast<symbols::size_type>
//         ( std::distance( p_first, p_end ) ) ;

//     cerr << "Number of parameters = '" << dist << "'" << endl;

//     switch ( dist ) {
//     case 1:
//         rc = *p_first;
//         break;

//     case 2:
//     {
//         cerr << "two parameters: \n";

//         symbols::iterator l_op = p_first;
//         cerr << "'" << ( *l_op )->get_value ( ) << "', "
//              << "'" << ( *l_op )->get_type ( ) << "'"
//              << endl;

//         symbols::iterator l_val = l_op;
//         ++l_val;
//         cerr << "'" << ( *l_val )->get_value ( ) << "', "
//              << "'" << ( *l_val )->get_type ( ) << "'"
//              << endl;

//         rc = invert_sign ( l_val ) ;

//         break;

//     }

//     case 3:
//     {
//         cerr << "three parameters: \n";

//         symbols::iterator l_val1 = p_first;
//         cerr << "'" << ( *l_val1 )->get_value ( ) << "', "
//              << "'" << ( *l_val1 )->get_type ( ) << "'"
//              << endl;

//         symbols::iterator l_op = l_val1;
//         ++l_op;
//         cerr << "'" << ( *l_op )->get_value ( ) << "', "
//              << "'" << ( *l_op )->get_type ( ) << "'"
//              << endl;

//         symbols::iterator l_val2 = l_op;
//         ++l_val2;
//         cerr << "'" << ( *l_val2 )->get_value ( ) << "', "
//              << "'" << ( *l_val2 )->get_type ( ) << "'"
//              << endl;

//         rc = evaluate ( l_val1, l_op, l_val2 ) ;
//         break;
//     }

//     case 4:
//     {
//         symbols::iterator val1 = p_first;

//         if ( ( *p_first )->get_value ( ) == symbol::value ( "-" ) ) {
//             ++val1;
//             string l_string = "-";
//             l_string += ( *val1 )->get_value ( );

//             ( *val1 )->set_value ( symbol::value ( l_string ) );
//         }

//         symbols::iterator op = p_first;
//         ++op;
//         ++op;
//         symbols::iterator val2 = op;
//         ++val2;

//         rc = evaluate ( val1, op, val2 ) ;
//         break;
//     }

//     default:
//         cerr << "Invalid number of parameters" << endl;
//     }

//     if ( rc ) {
//         cerr << "returning: "
//              << "'" << rc->get_value ( ) << "', "
//              << "'" << rc->get_type ( ) << "'"
//              << endl;
//     }

//     cerr << "\n'simple_exp_routine' - OUT" << endl;

//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::simple_exp_routine::
// evaluate ( symbols::iterator p_val1,
//            symbols::iterator p_op,
//            symbols::iterator p_val2 ) const {

//     memory::ptr<symbol> rc;

//     if ( ( *p_op )->get_value ( ) == symbol::value ( "+" ) ) {
//         rc = add ( p_val1, p_val2 ) ;
//     }
//     else if ( ( *p_op )->get_value ( ) == symbol::value ( "-" ) ) {
//         rc = minus ( p_val1, p_val2 ) ;
//     }
//     else if ( ( *p_op )->get_value ( ) == symbol::value ( "||" ) ) {
//         rc = logical_or ( p_val1, p_val2 ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::simple_exp_routine::
// invert_sign ( symbols::iterator p_symb_1 ) const {

//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1 ) )  {
//         rc = create_symbol<double>
//             ( - s2n<double> ( p_symb_1 ) ,
//               symbol::type ( "real" ) );
//     }
//     else {
//         rc = create_symbol<number::imaxt>
//             ( - s2n<number::imaxt> ( p_symb_1 ) ,
//               symbol::type ( "decimal_integer" ) );

//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::simple_exp_routine::
// add ( symbols::iterator p_symb_1,
//       symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<double>
//             ( s2n<double> ( p_symb_1 ) +
//               s2n<double> ( p_symb_2 ),
//               symbol::type ( "real" ) ) ;
//     }
//     else  {
//         rc = create_symbol<double>
//             ( s2n<number::imaxt> ( p_symb_1 ) +
//               s2n<number::imaxt> ( p_symb_2 ),
//               symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::simple_exp_routine::
// minus ( symbols::iterator p_symb_1,
//         symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<double>
//             ( s2n<double> ( p_symb_1 ) -
//               s2n<double> ( p_symb_2 ),
//               symbol::type ( "real" ) ) ;
//     }
//     else  {
//         rc = create_symbol<double>
//             ( s2n<number::imaxt> ( p_symb_1 ) -
//               s2n<number::imaxt> ( p_symb_2 ),
//               symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::simple_exp_routine::
// logical_or ( symbols::iterator p_symb_1,
//              symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) ||
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) ||
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //============================================================================
// //                                     expression_evaluator::term_routine

// //----------------------------------------------------------------------------
// expression_evaluator::term_routine::
// term_routine ( ) :
//     expression_evaluator_routine ( routine_id ( "term_routine" ),
//                                    production_id ( "term" ) ) {}

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::term_routine::
// exec( symbols::iterator p_first, symbols::iterator p_end ) const {

//     memory::ptr<symbol> rc;

//     cerr << "\n---------------\n"
//          << "'term_routine' - IN" << endl;

//     symbols::size_type dist =
//         static_cast<symbols::size_type>
//         ( std::distance( p_first, p_end ) ) ;

//     cerr << "Number of parameters = '" << dist << "'" << endl;

//     switch ( dist ) {
//     case 1:
//     {

//         cerr << "one parameter: "
//              << "'" << ( *p_first ) ->get_value ( ) << "', "
//              << "'" << ( *p_first ) ->get_type ( ) << "'"
//              << endl;

//         rc = *p_first;
//         break;
//     }

//     case 3:
//     {
//         cerr << "three parameters: \n"
//              << "'" << ( *p_first )->get_value ( ) << "', "
//              << "'" << ( *p_first )->get_type ( ) << "'"
//              << endl;

//         symbols::iterator l_second = p_first;
//         ++l_second;
//         cerr << "'" << ( *l_second )->get_value ( ) << "', "
//              << "'" << ( *l_second )->get_type ( ) << "'"
//              << endl;

//         symbols::iterator l_third = l_second;
//         ++l_third;
//         cerr << "'" << ( *l_third )->get_value ( ) << "', "
//              << "'" << ( *l_third )->get_type ( ) << "'"
//              << endl;

//         if ( ( *l_second )->get_value ( ) == symbol::value ( "*" ) ) {
//             rc = multiply ( p_first, l_third ) ;
//         }
//         else
//             if ( ( *l_second )->get_value ( ) == symbol::value ( "/" ) ) {
//                 rc = divide ( p_first, l_third ) ;
//             }
//             else if ( ( *l_second )->get_value ( ) == symbol::value ( "mod" )
//             ) {
//                 rc = modulus ( p_first, l_third ) ;
//             }
//             else if ( ( *l_second )->get_value ( ) == symbol::value ( "&&" )
//             ) {
//                 rc = logical_and ( p_first, l_third ) ;
//             }
//         break;
//     }

//     default:
//         cerr << "Invalid number of parameters" << endl;
//     }

//     if ( rc ) {
//         cerr << "returning: "
//              << "'" << rc->get_value ( ) << "', "
//              << "'" << rc->get_type ( ) << "'"
//              << endl;
//     }

//     cerr << "\n'term_routine' - OUT" << endl;
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::term_routine::
// multiply ( symbols::iterator p_symb_1,
//            symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<double>
//             ( s2n<double> ( p_symb_1 ) *
//               s2n<double> ( p_symb_2 ),
//               symbol::type ( "real" ) ) ;
//     }
//     else  {
//         rc = create_symbol<double>
//             ( s2n<number::imaxt> ( p_symb_1 ) *
//               s2n<number::imaxt> ( p_symb_2 ),
//               symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::term_routine::
// divide ( symbols::iterator p_symb_1,
//          symbols::iterator p_symb_2 ) const {

//     return
//         create_symbol<double> ( s2n<double> ( p_symb_1 ) /
//                                 s2n<double> ( p_symb_2 ),
//                                 symbol::type ( "real" ) ) ;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::term_routine::
// modulus ( symbols::iterator p_symb_1,
//           symbols::iterator p_symb_2 ) const {
//     return
//         create_symbol<double>
//         ( fmod ( s2n<double> ( p_symb_1 ) ,
//                  s2n<double> ( p_symb_2 ) ),
//           symbol::type ( "real" ) ) ;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::term_routine::
// logical_and ( symbols::iterator p_symb_1,
//               symbols::iterator p_symb_2 ) const {
//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1, p_symb_2 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( s2n<double> ( p_symb_1 ) &&
//               s2n<double> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( s2n<number::imaxt> ( p_symb_1 ) &&
//               s2n<number::imaxt> ( p_symb_2 )? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //============================================================================
// //                                     expression_evaluator::factor_routine

// //----------------------------------------------------------------------------
// expression_evaluator::factor_routine::
// factor_routine ( ) :
//     expression_evaluator_routine ( routine_id ( "factor_routine" ),
//                                    production_id ( "factor" ) ) {}

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::factor_routine::
// exec( symbols::iterator p_first, symbols::iterator p_end ) const {

//     memory::ptr<symbol> rc;

//     cerr << "\n---------------\n"
//          << "'factor_routine' - IN" << endl;

//     symbols::size_type dist =
//         static_cast<symbols::size_type>
//         ( std::distance( p_first, p_end ) ) ;

//     cerr << "Number of parameters = '" << dist << "'" << endl;

//     switch ( dist ) {
//     case 1:
//     {

//         cerr << "one parameter: "
//              << "'" << ( *p_first )->get_value ( ) << "', "
//              << "'" << ( *p_first )->get_type ( ) << "'"
//              << endl;

//         rc = *p_first;
//         break;
//     }

//     case 2:
//     {
//         cerr << "two parameter: "
//              << "'" << ( *p_first ) ->get_value ( ) << "', "
//              << "'" << ( *p_first ) ->get_type ( ) << "'"
//              << endl;

//         symbols::iterator l_second = p_first;
//         ++l_second;
//         cerr << "'" << ( *l_second ) ->get_value ( ) << "'"
//              << "'" << ( *l_second ) ->get_type ( ) << "'"
//              << endl;

//         rc = logical_not( l_second ) ;

//         break;
//     }

//     case 3:
//     {
//         cerr << "three parameters: "
//              << "'" << ( *p_first ) ->get_value ( ) << "', "
//              << "'" << ( *p_first ) ->get_type ( ) << "'"
//              << endl;

//         symbols::iterator l_aux = p_first;
//         ++l_aux;
//         cerr << "'" << ( *l_aux ) ->get_value ( ) << "', "
//              << "'" << ( *l_aux ) ->get_type ( ) << "'"
//              << endl;

//         rc = *l_aux;

//         ++l_aux;
//         cerr << "'" << ( *l_aux ) ->get_value ( ) << "', "
//              << "'" << ( *l_aux ) ->get_type ( ) << "'"
//              << endl;
//         break;
//     }

//     default:
//         cerr << "Invalid number of parameters" << endl;
//     }

//     if ( rc ) {
//         cerr << "returning: "
//              << "'" << rc->get_value ( ) << "', "
//              << "'" << rc->get_type ( ) << "'"
//              << endl;
//     }

//     cerr << "\n'factor_routine' - OUT" << endl;
//     return rc;
// }

// //----------------------------------------------------------------------------
// memory::ptr<symbol>
// expression_evaluator::factor_routine::
// logical_not ( symbols::iterator p_symb_1 ) const {

//     memory::ptr<symbol> rc;

//     if ( is_real ( p_symb_1 ) ) {
//         rc = create_symbol<number::u16t> (
//             ( !s2n<double> ( p_symb_1 ) ? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     else {
//         rc = create_symbol<number::u16t> (
//             ( !s2n<number::imaxt> ( p_symb_1 ) ? 1 : 0 ) ,
//             symbol::type ( "decimal_integer" ) ) ;
//     }
//     return rc;
// }

// //============================================================================
// //                                     test_grammar_1

// //----------------------------------------------------------------------------
// bool
// test_grammar_1::
// exec ( ) {
//     expression g1;
//     g1.report ( cout );
//     return true;
// }

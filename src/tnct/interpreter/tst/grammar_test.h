// #ifndef _tenacitas__interpreter__test__grammar_test_h_
// #define _tenacitas__interpreter__test__grammar_test_h_

// // #include "lexical.h"
// // #include "symbol.h"
// // #include "recognizer.h"
// //#include "recognizers.h"
// // #include "identifiers.h"
// // #include "types.h"
// // #include "grammar.h"

// //#include "source.h"

// #include "interpreter.h"

// #include "scanner_test.h"

// #include <tenacitas.memory/ptr.h>
// #include <tenacitas.number/converter.h>

// #include <sstream>
// #include <fstream>
// #include <iostream>
// #include <cctype>

// using namespace tenacitas;

// using namespace std;

// namespace tenacitas {

//     namespace interpreter {

//         namespace test {

//             /** @brief  Implements the following Grammar:

//                 Expressed in EBNF:

//                 expression = simple-expression [ relational-operator
//                 simple-expression ]
//                 simple-expression = [ sign ] term { addition-operator term }
//                 . term = factor { multiplication-operator factor } factor =
//                 number | "(" expression ")" | not factor relational-operator
//                 = "==" | "<>" | "<" | "<=" | ">" | ">=" addition-operator =
//                 "+" | "-" | or multiplication-operator = "*" | "/" | div |
//                 mod | and number = integer-number | real-number
//                 integer-number = digit-sequence
//                 real-number = digit-sequence "." [ unsigned-digit-sequence ]
//                 unsigned-digit-sequence = digit { digit } .
//                 digit-sequence = [ sign ] unsigned-digit-sequence .

//                 Expressed more closely to our implementation:

//                 expression =
//                 {
//                 ( 0, p simple_exp,      1, # )
//                 ( 1, v ==,              7, 2 )
//                 ( 2, v <,               7, 3 )
//                 ( 3, v >,               7, 4 )
//                 ( 4, v !=               7, 5 )
//                 ( 5, v >=               7, 6 )
//                 ( 5, v <=               7, y )
//                 }

//                 simple_exp =
//                 {
//                 ( 0, v +,               2, 1 )
//                 ( 1, v -,               2, 2 )
//                 ( 2, p term,            3, y )
//                 ( 3, v +,               2, 4 )
//                 ( 4, v -,               2, 5 )
//                 ( 5, v ||,              2, # )
//                 }

//                 term =
//                 {
//                 ( 0, p factor,          1, # )
//                 ( 1, v *,               0, 1 )
//                 ( 2, v /,               0, 3 )
//                 ( 3, v mod,             0, 4 )
//                 ( 4, v &&,              0, y )
//                 }

//                 factor =
//                 {
//                 ( 0, t decimal_integer, #, 1 )
//                 ( 1, t real,            #, 2 )
//                 ( 2, v (,               3, 5 )
//                 ( 3, p expression,      4, # )
//                 ( 4, v ),               #, # )
//                 ( 5, v !,               6, # )
//                 ( 6, p factor,          #, # )
//                 }

//                 @nosubgrouping */
//             class expression : public grammar {
//             public:
//                 //
//                 -------------------------------------------------------------
//                 //                         expression
//                 /// @name                  Public Constructors
//                 /// @{

//                 /** @brief Constructor */
//                 expression ( );

//                 /** @brief Destructor */
//                 virtual ~expression ( );

//                 /// @}                     ----------
//             };

//             /** @brief Specialization of the @p expression class that
//             associates
//                 @p routine objects to evaluated it

//                 @nosubgrouping */
//             class expression_evaluator : public expression {
//             public:
//                 //
//                 -------------------------------------------------------------
//                 //                         expression_evaluator
//                 /// @name                  Public Constructors
//                 /// @{

//                 /** @brief Constructor */
//                 expression_evaluator ( );

//                 virtual ~expression_evaluator ( );

//                 /// @}                     ----------

//             private:

//                 //
//                 -------------------------------------------------------------
//                 //                         expression_evaluator
//                 /// @name                  Private Internal Types
//                 /// @{

//                 /** @brief Base tenacitas::interpreter::production_routine
//                 class
//                     for @p expression_evaluator objects, providing commom
//                     methods
//                     @nosubgrouping */
//                 class expression_evaluator_routine : public
//                 production_routine {

//                 public:
//                     //
//                     ---------------------------------------------------------
//                     //                         expression_evaluator_routine
//                     /// @name                  Public Constructors
//                     /// @{

//                     /** @brief @copydoc
//                         tenacitas::interpreter::production_routine */
//                     expression_evaluator_routine
//                     ( const routine_id & p_routine_id,
//                       const production_id & p_production_id );

//                     /** @brief Destructor */
//                     virtual ~expression_evaluator_routine ( );

//                     /// @}                     ----------

//                 protected:

//                     //
//                     ---------------------------------------------------------
//                     //                         expression_evaluator_routine
//                     /// @name                  Protected Processors
//                     /// @{

//                     /** @brief Converts a @p symbol::value to a number

//                         @param [in] p_symb is the
//                         tenacitas::interpreter::symbol which
//                         tenacitas::interpreter::symbol::value should be
//                         converted to a number

//                         @return a actual number with the value of <tt>
//                         p_symb->get_value ( ) </tt>

//                         The @p t_number type parameter defines the type of
//                         number that the @p p_symb should be converted to */
//                     template <typename t_number>
//                     t_number s2n ( symbols::const_iterator p_symb ) const {
//                         t_number rc;
//                         number::s2n<number::base10>( ( *p_symb )->get_value (
//                         ),
//                                                      rc ) ;
//                         return rc;
//                     }

//                     /** @brief Converts a number to a @p symbol::value

//                         @param p_number [in] is the number that shoul be
//                         converted to a tenacitas::interpreter::symbol::value
//                         object

//                         @return a @p symbol::value object, representing the
//                         @p p_number

//                         The @p t_number type parameter defines the type of
//                         number of @p p_number */
//                     template <typename t_number>
//                     symbol::value n2s ( const t_number & p_number ) const {
//                         ostringstream l_stream;
//                         number::n2s<number::base10> ( p_number, l_stream ) ;
//                         return symbol::value ( l_stream.str( ) ) ;
//                     }

//                     /** @brief Given a @p symbol::type and a number, creates
//                     a @p
//                         symbol

//                         @param [in] p_number is the number that will be
//                         converted to a @p aymbol::value

//                         @param [in] p_symbol_type is the @p symbol::type of
//                         the created @p symbol

//                         The @p t_number type parameter defines the type of
//                         number of @p p_number*/
//                     template <typename t_number>
//                     memory::ptr<symbol>
//                     create_symbol ( const t_number & p_number,
//                                     const symbol::type & p_symbol_type  )
//                                     const {
//                         ostringstream l_stream;
//                         number::n2s<number::base10> ( p_number, l_stream ) ;
//                         return memory::ptr<symbol>
//                             ( new symbol ( l_stream.str( ) , p_symbol_type  )
//                             );
//                     }

//                     /** @brief Indicates if the @p symbol::type of a @p
//                     symbol
//                         is <tt> symbol::type ( "real" ) </tt>

//                         @param [in] p_symb_1 is a (reference) to a <tt>
//                         memory::ptr<symbol> </tt> object

//                         @return @p true if <tt> (*p_symb_1)->get_symbol_type
//                         ( )
//                         == symbol::type ( "real" ) </tt>

//                         @return @p false otherwise */
//                     bool is_real ( symbols::const_iterator p_symb_1 ) const;

//                     /** @brief Indicates if the @p symbol::type of one of two
//                     @p
//                         symbol objects is <tt> symbol::type ( "real" ) </tt>

//                         @param [in] p_symb_1 is a (reference) to a <tt>
//                         memory::ptr<symbol> </tt> object

//                         @param [in] p_symb_2 is a (reference) to a <tt>
//                         memory::ptr<symbol> </tt> object

//                         @return @p true if <tt> (*p_symb_1)->get_symbol_type
//                         ( )
//                         == symbol::type ( "real" ) ||
//                         (*p_symb_1)->get_symbol_type ( ) == symbol::type (
//                         "real" ) </tt>

//                         @return @p false otherwise */
//                     bool is_real ( symbols::const_iterator p_symb_1,
//                                    symbols::const_iterator p_symb_2) const;

//                     /// @}                     ----------

//                 };

//                 /** @brief tenacitas::interpreter::production_routine to be
//                     executed when the @p expression production is recognized

//                     @nosubgrouping */
//                 class expression_routine : public
//                 expression_evaluator_routine { public:
//                     //
//                     ---------------------------------------------------------
//                     //                         expression_routine
//                     /// @name                  Public Constructors
//                     /// @{

//                     /** @brief Constructor */
//                     expression_routine ( ) ;

//                     /// @}                     ----------

//                     //
//                     ---------------------------------------------------------
//                     //                         expression_routine
//                     /// @name                  Public Processors
//                     /// @{

//                     /** @brief @copydoc tenacitas::interpreter::routine::exec
//                     */ memory::ptr<symbol> exec( symbols::iterator p_first,
//                                               symbols::iterator p_end )
//                                               const;

//                     /// @}                     ----------

//                 private:

//                     //
//                     ---------------------------------------------------------
//                     //                         expression_routine
//                     /// @name                  Private Processors
//                     /// @{

//                     /** @brief Calculates if two
//                     tenacitas::interpreter::symbol
//                         objects are equal

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) == (
//                         *p_symb_2
//                         )->get_value ( ) </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) != (
//                         *p_symb_2
//                         )->get_value ( ) </tt> */
//                     memory::ptr<symbol>
//                     equal_to ( symbols::iterator p_symb_1,
//                                symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates if one
//                     tenacitas::interpreter::symbol
//                         object is less than another

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) < (
//                         *p_symb_2
//                         )->get_value ( ) </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) >= (
//                         *p_symb_2
//                         )->get_value ( ) </tt> */
//                     memory::ptr<symbol>
//                     less_than ( symbols::iterator p_symb_1,
//                                 symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates if one
//                     tenacitas::interpreter::symbol
//                         object is less than another

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) < (
//                         *p_symb_2
//                         )->get_value ( ) </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) >= (
//                         *p_symb_2
//                         )->get_value ( ) </tt> */
//                     memory::ptr<symbol>
//                     greater_than ( symbols::iterator p_symb_1,
//                                    symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates if one
//                     tenacitas::interpreter::symbol
//                         object is not equal to another

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) != (
//                         *p_symb_2
//                         )->get_value ( ) </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) == (
//                         *p_symb_2
//                         )->get_value ( ) </tt> */
//                     memory::ptr<symbol>
//                     not_equal_to ( symbols::iterator p_symb_1,
//                                    symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates if one
//                     tenacitas::interpreter::symbol
//                         object is greater or equal to another

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) >= (
//                         *p_symb_2
//                         )->get_value ( ) </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) < (
//                         *p_symb_2
//                         )->get_value ( ) </tt> */
//                     memory::ptr<symbol>
//                     greater_or_equal_to ( symbols::iterator p_symb_1,
//                                           symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates if one
//                     tenacitas::interpreter::symbol
//                         object is less or equal to another

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) <= (
//                         *p_symb_2
//                         )->get_value ( ) </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) > (
//                         *p_symb_2
//                         )->get_value ( ) </tt> */
//                     memory::ptr<symbol>
//                     less_or_equal_to ( symbols::iterator p_symb_1,
//                                        symbols::iterator p_symb_2 ) const;

//                     /// @}                     ----------
//                 };

//                 /** @brief tenacitas::interpreter::production_routine to be
//                     executed when the @p simple_exp production is recognized

//                     @nosubgrouping */
//                 class simple_exp_routine : public
//                 expression_evaluator_routine { public:
//                     //
//                     ---------------------------------------------------------
//                     //                         simple_exp_routine
//                     /// @name                  Public Constructors
//                     /// @{

//                     /** @brief Constructor */
//                     simple_exp_routine ( ) ;

//                     /// @}                     ----------

//                     //
//                     ---------------------------------------------------------
//                     //                         simple_exp_routine
//                     /// @name                  Public Processors
//                     /// @{

//                     /** @brief @copydoc tenacitas::interpreter::routine::exec
//                     */ memory::ptr<symbol> exec( symbols::iterator p_first,
//                                               symbols::iterator p_end )
//                                               const;

//                     /// @}                     ----------

//                 private:

//                     //
//                     ---------------------------------------------------------
//                     //                         simple_exp_routine
//                     /// @name                  Private Processors
//                     /// @{

//                     /** @brief Evaluates the simple expression, according to
//                         operands and operation recognizeds

//                         @param p_val1 [in] is the first operand
//                         @param p_op [in] is the operation
//                         @param p_val2 [in] is the second operand

//                         @return a <tt> memory::ptr<symbol> </tt> with the
//                         result of the operation evaluated */
//                     memory::ptr<symbol>
//                     evaluate ( symbols::iterator p_val1,
//                                symbols::iterator p_op,
//                                symbols::iterator p_val2 ) const ;

//                     /** @brief Inverts the sign value of the a @p symbol
//                     object

//                         @param p_symb_1 [in] is the reference to the <tt>
//                         memory::ptr<symbol> </tt> object that will have its
//                         <tt> symbol::value </tt> changed

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, or <tt> get_type ( ) == symbol_type ( "real" )
//                         </tt>, and <tt> get_value ( ) </tt> is the opposite
//                         value found in @p p_symb_1 */
//                     memory::ptr<symbol>
//                     invert_sign ( symbols::iterator p_symb_1 ) const;

//                     /** @brief Calculates the addition of two
//                         tenacitas::interpreter::symbol object

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         addition

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         addition

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, or <tt> get_type ( ) == symbol_type ( "real" )
//                         </tt>, and <tt> get_value ( )</tt> is the addition of
//                         <tt> p_symb_1->get_value ( )</tt> and <tt>
//                         p_symb_2->get_value ( )</tt> */
//                     memory::ptr<symbol>
//                     add ( symbols::iterator p_symb_1,
//                           symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates the subtraction of two
//                         tenacitas::interpreter::symbol object

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         subtraction

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         subtraction

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, or <tt> get_type ( ) == symbol_type ( "real" )
//                         </tt>, and <tt> get_value ( )</tt> is the subtraction
//                         of <tt> p_symb_1->get_value ( )</tt> and <tt>
//                         p_symb_2->get_value ( )</tt> */
//                     memory::ptr<symbol>
//                     minus ( symbols::iterator p_symb_1,
//                             symbols::iterator p_symb_2 ) const;

//                     /** @brief Indicates if one of two
//                         tenacitas::interpreter::symbol is non zero

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) != 0 </tt>
//                         or <tt> ( *p_symb_2 )->get_value ( ) != 0 </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) != 0 </tt>
//                         and <tt> ( *p_symb_2 )->get_value ( ) != 0 </tt>*/
//                     memory::ptr<symbol>
//                     logical_or ( symbols::iterator p_symb_1,
//                                  symbols::iterator p_symb_2 ) const;

//                     /// @}                     ----------

//                 };

//                 /** @brief tenacitas::interpreter::production_routine to be
//                     executed when the @p term production is recognized

//                     @nosubgrouping */
//                 class term_routine : public expression_evaluator_routine {
//                 public:

//                     //
//                     ---------------------------------------------------------
//                     //                         term_routine
//                     /// @name                  Public Constructors
//                     /// @{

//                     /** @brief Constructor */
//                     term_routine ( );

//                     /// @}                     ----------

//                     //
//                     ---------------------------------------------------------
//                     //                         term_routine
//                     /// @name                  Public Processors
//                     /// @{

//                     /** @brief @copydoc tenacitas::interpreter::routine::exec
//                     */ memory::ptr<symbol> exec( symbols::iterator p_first,
//                                               symbols::iterator p_end )
//                                               const;

//                     /// @}                     ----------

//                 private:

//                     //
//                     ---------------------------------------------------------
//                     //                         term_routine
//                     /// @name                  Private Processors
//                     /// @{

//                     /** @brief Calculates the product of two
//                         tenacitas::interpreter::symbol object

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         product

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         product

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, or <tt> get_type ( ) == symbol_type ( "real" )
//                         </tt>, and <tt> get_value ( )</tt> is the product of
//                         <tt> p_symb_1->get_value ( )</tt> and <tt>
//                         p_symb_2->get_value ( )</tt> */
//                     memory::ptr<symbol>
//                     multiply ( symbols::iterator p_symb_1,
//                                symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates the division of two
//                         tenacitas::interpreter::symbol object

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         division

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         division

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "real" ) </tt>, and
//                         <tt> get_value ( )</tt> is the division of <tt>
//                         p_symb_1->get_value ( )</tt> and <tt>
//                         p_symb_2->get_value ( )</tt> */
//                     memory::ptr<symbol>
//                     divide ( symbols::iterator p_symb_1,
//                              symbols::iterator p_symb_2 ) const;

//                     /** @brief Calculates the modulus of two
//                         tenacitas::interpreter::symbol object

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         modulus

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         modulus

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "real" ) </tt>, and
//                         <tt> get_value ( )</tt> is the modulus of <tt>
//                         p_symb_1->get_value ( )</tt> and <tt>
//                         p_symb_2->get_value ( )</tt> */
//                     memory::ptr<symbol>
//                     modulus ( symbols::iterator p_symb_1,
//                               symbols::iterator p_symb_2 ) const;

//                     /** @brief Indicates if both of two
//                         tenacitas::interpreter::symbol are non zero

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @param p_symb_2 [in] is the reference to the second
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "1" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) != 0 </tt>
//                         and <tt> ( *p_symb_2 )->get_value ( ) != 0 </tt>

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) != 0 </tt>
//                         or <tt> ( *p_symb_2 )->get_value ( ) != 0 </tt>*/
//                     memory::ptr<symbol>
//                     logical_and ( symbols::iterator p_symb_1,
//                                   symbols::iterator p_symb_2 ) const;

//                     /// @}                     ----------

//                 };

//                 /** @brief tenacitas::interpreter::production_routine to be
//                     executed when the @p factor production is recognized

//                     @nosubgrouping */
//                 class factor_routine : public expression_evaluator_routine {
//                 public:

//                     //
//                     ---------------------------------------------------------
//                     //                         factor_routine
//                     /// @name                  Public Constructors
//                     /// @{

//                     /** @brief Constructor */
//                     factor_routine ( );
//                     /// @}                     ----------

//                     //
//                     ---------------------------------------------------------
//                     //                         factor_routine
//                     /// @name                  Public Processors
//                     /// @{

//                     /** @brief @copydoc tenacitas::interpreter::routine::exec
//                     */ memory::ptr<symbol> exec( symbols::iterator p_first,
//                                               symbols::iterator p_end )
//                                               const;

//                     /// @}                     ----------

//                 private:

//                     //
//                     ---------------------------------------------------------
//                     //                         factor_routine
//                     /// @name                  Private Processors
//                     /// @{

//                     /** @brief Inverts the logical value of a
//                         tenacitas::interpreter::symbol

//                         @param p_symb_1 [in] is the reference to the first
//                         <tt> memory::ptr<symbol> </tt> object used in the
//                         comparision

//                         @return a <tt> memory::ptr<symbol> </tt>, where <tt>
//                         get_type ( ) == symbol_type ( "decimal_integer" )
//                         </tt>, and <tt> get_value ( ) == symbol_value ( "0" )
//                         </tt> if <tt> ( *p_symb_1 )->get_value ( ) != 0 </tt>
//                         or <tt> get_value ( ) == symbol_value ( "1" ) </tt>
//                         if <tt> ( *p_symb_1 )->get_value ( ) == 0 </tt> */
//                     memory::ptr<symbol>
//                     logical_not ( symbols::iterator p_symb_1 ) const;

//                     /// @}                     ----------

//                 };
//             };

//             /** @brief Prints information about the graph of the @p
//             expression
//                 grammar  */
//             class test_grammar_1 {
//             public:
//                 /** @brief Excecutes the test

//                     @return @p true if the test was ok
//                     @return @p false otherwise */
//                 bool exec ( );
//             };

//         }
//     }
// }

// #endif

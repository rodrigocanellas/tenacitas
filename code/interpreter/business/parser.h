
#ifndef __tenacitas_interpreter__parser__h__
#define __tenacitas_interpreter__parser__h__


// =============================================================================
/** @file
    @brief   */



// =============================================================================
//                                 C++ Headers

#include <string>
#include <list>
#include <sstream>
#include <algorithm>
#include <iostream>


// =============================================================================
//                                 3rds Headers


// =============================================================================
//                                 Our Headers

#include <tenacitas.number/ints.h>

#include <tenacitas.memory/ptr.h>
#include <tenacitas.number/fault.h>

#include "symbol.h"
#include "grammar.h"
#include "scanner.h"

// =============================================================================
//                                 Namespaces
using namespace std;
using namespace tenacitas;




// =====================================================================
//                                 Macro Commands

/** @brief Used when stating a function */
#define tin {if ( m_trace ) { m_stream << "\n---------------\n'" << __FUNCTION__ << "' - IN " << endl;} } 

                                                                            
/** @brief Used when finishing a function */
#define tout {if ( m_trace ) {report ( ) ; m_stream << "\n'" << __FUNCTION__ << "' - OUT " << endl;} } 






// =============================================================================
//                                 Macro Commands

/** @brief   */






/** @brief Namespace of the organization */
namespace tenacitas {
  /** @brief Identifiers to allow the creation of a interpreter */
  namespace interpreter {

    // =====================================================================
    //                             Typedefs

    /** @brief */



    // =====================================================================
    //                             Pre-Declarations





    // =====================================================================
    //                             Attributes

    /** @brief    */





    // =====================================================================
    //                             Functions

    /** @brief */





    // =====================================================================
    //                             Classes

    /** @brief Parses a text according to a tenacitas::interpreter::grammar

	@nosubgrouping */
    class parser {

    public:


      // -----------------------------------------------------------------
      //                         parser
      /// @name                  Public Constructors
      /// @{ 

      /** @brief Constructor
                
	  @param [in] p_to_ignore is a string containing chars to
	  be ignored while analysing the text */
      parser ( const string & p_to_ignore = " \r\t\n" ) ;
            

      /** @brief Destructor */
      ~parser ( ) ;

      /// @}                     ----------


      // -----------------------------------------------------------------
      //                         parser
      /// @name                  Public Processors
      /// @{

      /* @brief Parses a text according to a
	 tenacitas::interpreter::grammar

	 The template parameter @p t_grammar is the class of @p grammar
	 that should be used to parse the text

	 @param [in] p_begin refereces the first char in the text being
	 analysed 

	 @param [in] p_end references the end of the text being analysed

	 @param [in] p_trace indicates if messages detailing the parsing
	 process should be printed to @p cerr */
      template <typename t_grammar>
	void parse ( string::const_iterator p_begin,
		     string::const_iterator p_end,
		     bool p_trace = false ) ;
	    
      /// @}                     ----------

      // -----------------------------------------------------------------
      //                         parser
      /// @name                  Public Attributes
      /// @{

      /** @brief Indicates that an error occurred dutin the parser */ 
      static const number::u16t error_parsing_text;
	    
      /// @}                     ----------

    private:

      // -----------------------------------------------------------------
      //                         parser
      /// @name                  Private Internal Types
      /// @{

      /** @brief Helps the @p parser when parsing a text according to a
	  grammar

	  This auxiliar structure is used to remember the @p item objects
	  found during the parser that referenced
	  tenacitas::interpreter::grammar::production objects, and the
	  tenacitas::interpreter::symbol objects that compunded that  @p
	  production  

	  @nosubgrouping */
      class stack;

               
      /// @}                     ----------

      // -----------------------------------------------------------------
      //                         parser
      /// @name                  Private Processors
      /// @{

      template <typename t_grammar>
	void configure_scanner ( const t_grammar & p_grammar,
				 string::const_iterator p_begin,
				 string::const_iterator p_end );

      memory::ptr<symbol>
	exec_routine ( memory::ptr<const grammar::production::item> p_item );

      void exec_symbol_routine (  );
            
      memory::ptr<symbol> exec_production_routine ( );

      bool end_of_production ( ) const ;

      void initialize_stack ( );

      bool points_to_production ( ) const;

      void read_symbol ( );
            
      bool end_of_input ( ) const;
            
      void follow_successor ( );
            
      bool empty_stack ( ) const;

      bool is_terminal ( ) const;
            
      bool has_alternative ( ) const ;
            
      bool recognize_symbol ( )const ;

      void pop_item ( );

      void push_item ( );

      void push_symbol ( );

      void push_symbol ( memory::ptr<symbol> p_symbol );
            
      void new_production ( ) ;

      void follow_alternative ( ) ;

      bool accepts_anything ( ) const;
            
      template <typename t_grammar>
	void goto_first_item ( const t_grammar & p_grammar );

      void begin_loop ( );
            
      /// @}                     ----------


      // -----------------------------------------------------------------
      //                         parser
      /// @name                  Private Helpers
      /// @{

      void report ( ) const ;

      /// @}                     ----------
            

      // -----------------------------------------------------------------
      //                         parser
      /// @name                  Private Attributes
      /// @{

      /** @brief The stream used to print messages */ 
      ostream & m_stream;

      /** @brief The scanner object hat will try to retrieve symbols from
	  the input text */ 
      scanner m_scanner;

      /** @brief Indicates if messages detailing the parsing process
	  should be printed to @p cerr  */
      bool m_trace;
	    
      /** @brief The current
	  tenacitas::interpreter::grammar::production::item being
	  analysed  */ 
      memory::ptr<const grammar::production::item> m_item;

      /** @brief The current tenacitas::interpreter::symbol */ 
      memory::ptr<const symbol> m_symbol;

      /** @brief The auxiliar structure used to remember the @p item
	  objects found during the parser that referenced
	  tenacitas::interpreter::grammar::production objects, and the
	  tenacitas::interpreter::symbol objects that compunded that  @p
	  production  */ 
      memory::ptr<stack> m_stack;

      /// @}                     ----------

    };



    /** @copydoc tenacitas::interpreter::parser::stack

	@nosubgrouping */            
    class parser::stack {

    public:

      // ------------------------------------------------------------
      //                         stack
      /// @name                  Public Constructors
      /// @{

      /** @brief Constructor */
      stack ( ) ;

      /** @brief Destructor */
      ~stack ( ) ;

      /// @}                     ----------

      // ------------------------------------------------------------
      //                         stack
      /// @name                  Public Accessors
      /// @{

            
      /** @brief Retrieves the @p item that is on the top of the
	  stack  */
      memory::ptr<const grammar::production::item> get_top_item ( ) ;
	    
      /// @}                     ----------

      // ------------------------------------------------------------
      //                         stack
      /// @name                  Public Helpers
      /// @{

      /** @brief Prints the current state of the stack

	  @param [in] p_stream is the stream used to print the state
	  of the stack */
      void report ( ostream & p_stream ) const ;
	    
      /// @}                     ----------

      // ------------------------------------------------------------
      //                         stack
      /// @name                  Public Processors
      /// @{

      /** @brief Puts a
	  tenacitas::intepreter::grammar::production::item on the top
	  of the stack  */  
      void push ( memory::ptr<const grammar::production::item>
		  p_item ) ;


      /** @brief Retrieves the
	  tenacitas::intepreter::grammar::production::item 
	  from the top of the stack */ 
      memory::ptr<const grammar::production::item> pop ( ) ;

      /** @brief Puts a tenacitas::interpreter::symbol on the top of
	  the stack

	  @param [in] p_symbol is the @p symbol to be put on the top
	  of the stack */ 
      void push ( memory::ptr<symbol> p_symbol ) ;

      /** @brief Informs if the stack is empty */ 
      bool empty ( ) const;

      /** @brief Retrieves a reference to the first symbol in the
	  stack  */ 
      symbols::iterator first_symbol ( ) ;

      /** @brief Retrieves a reference to the last symbol in the
	  stack  */  
      symbols::iterator last_symbol ( ) ;

      /** @brief Replaces the @p symbol at the top of the stack */ 
      void replace ( memory::ptr<symbol> p_symbol ) ;
                
	    
      /// @}                     ----------

    private:

      // ------------------------------------------------------------
      //                         stack
      /// @name                  Private Internal Types
      /// @{ 

      /** @brief An item in the stack of
	  tenacitas::interpreter::grammar::production::item

	  A @p syntatic_item contains a reference to the @p item that
	  points to a tenacitas::interpreter::grammar::production, and
	  the location in the tenacitas::interpreter::symbols just
	  before the first tenacitas::interpreter::symbol object
	  associated to that @p production.item

	  For examplem suppose the grammar for an expression:

	  0               1               7
	  expression := simple-exp --s--> '==' --s--> simple-exp
	  |
	  |       2             7
	  a--> '<' --s--> simple-exp
	  |
	  |     3             7
	  a--> '>' --s--> simple-exp
	  |
	  |     4               7
	  a--> '!=' --s--> simple-exp
	  |
	  |      5               7
	  a--> '>=' --s--> simple-exp
	  |
	  |      6               7
	  a--> '<=' --s--> simple-exp
	  |
	  |
	  a--> anything

 
	  0            2            3           2
	  simple-exp := '+' --s--> term -----s--> '+' --s--> term
	  |                         |
	  |     1          2        |     4         2
	  a--> '-' --s--> term      a--> '-' --s-> term
	  |                         |
	  |       2                 |      5         2
	  a --> term                a--> '||' --s-> term
	  |
	  |      
	  a--> anything


	  0           1             0
	  term := factor --s--> '*' --s--> factor
	  |
	  |      2            0
	  a--> '/' --s--> factor
	  |
	  |       3              0
	  a--> 'mod' --s--> factor
	  |
	  |      4              0
	  a--> '&&' --s--> factor
	  |
	  |
	  a--> anything

	  0
	  factor := decimal_integer
	  |
	  |    1
	  a-> real
	  |
	  |      2          3                4
	  a--> '(' --s--> expression --s--> ')'
	  |
	  |    5           6
	  a--> ! --s--> factor


	  Suppose to parse the text '4 + 2'.
	  Here it is a development log of the
	  tenacitas::interpreter::parser::parse method. The more
	  important parts are marked with '+++'.
                 


	  ---------------
	  'parse' - IN 

	  ---------------
	  'configure_scanner' - IN 

	  no-symbol
	  no-item

	  'configure_scanner' - OUT 

	  ---------------
	  'read_symbol' - IN 

	  symbol: '4', 'decimal_integer'
	  no-item

	  'read_symbol' - OUT 

	  ---------------
	  'initialize_stack' - IN 

	  symbol: '4', 'decimal_integer'
	  no-item

	  +++ This is what the
	  tenacitas::interpreter::parser::stack::report method
	  prints. At this point, the @p syntatic stack has no @p item,
	  and the @p symbols list has a dummy symbol, which helps to
	  syncronize the @p syantic stack
                 
	  syntatic
	  prod-id        item-id        index          

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'initialize_stack' - OUT 

	  ---------------
	  'goto_first_item' - IN 

	  symbol: '4', 'decimal_integer'
	  item: '!root-prod!', '0'

	  syntatic
	  prod-id        item-id        index          

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'goto_first_item' - OUT 

	  ---------------
	  'push_item' - IN 

	  symbol: '4', 'decimal_integer'
	  item: '!root-prod!', '0'

	  +++ The tenacitas::interpreter::parser::push_item method
	  pushes a @p item onto the @p synatatic stack. The @p item
	  pushed is the only @p item of the (first) internal @p
	  production of the @p parser - "!root-prod!". That @p item
	  points to the actual first @p production of the grammar,
	  "exp".  
                 
	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'push_item' - OUT 

	  ---------------
	  'new_production' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'new_production' - OUT 

	  ==============================

	  ---------------
	  'end_of_production' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'end_of_production' - OUT 

	  ---------------
	  'accepts_anything' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'accepts_anything' - OUT 

	  ---------------
	  'is_terminal' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'is_terminal' - OUT 

	  ---------------
	  'push_item' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'exp', '0'

	  +++ As (exp, 0) is not a terminal, it references the @p
	  production "simple-exp", it is pushed onto the @p syntatic
	  stack. The 'index' in the @p syntatic stack references the
	  position in the @p symbols list imediatley before where the
	  @p symbol objects that form the "simple-exp" @p production.

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'push_item' - OUT 

	  ---------------
	  'new_production' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'new_production' - OUT 

	  ==============================

	  ---------------
	  'end_of_production' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'end_of_production' - OUT 

	  ---------------
	  'accepts_anything' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'accepts_anything' - OUT 

	  ---------------
	  'is_terminal' - IN 
	  rc = true

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'is_terminal' - OUT 

	  ---------------
	  'recognize_symbol' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'recognize_symbol' - OUT 

	  ---------------
	  'has_alternative' - IN 
	  rc = true

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'has_alternative' - OUT 

	  ---------------
	  'follow_alternative' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '1'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'follow_alternative' - OUT 

	  ==============================

	  ---------------
	  'end_of_production' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '1'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'end_of_production' - OUT 

	  ---------------
	  'accepts_anything' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '1'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'accepts_anything' - OUT 

	  ---------------
	  'is_terminal' - IN 
	  rc = true

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '1'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'is_terminal' - OUT 

	  ---------------
	  'recognize_symbol' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '1'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'recognize_symbol' - OUT 

	  ---------------
	  'has_alternative' - IN 
	  rc = true

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '1'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'has_alternative' - OUT 

	  ---------------
	  'follow_alternative' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '2'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'follow_alternative' - OUT 

	  ==============================

	  ---------------
	  'end_of_production' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '2'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'end_of_production' - OUT 

	  ---------------
	  'accepts_anything' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '2'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'accepts_anything' - OUT 

	  ---------------
	  'is_terminal' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '2'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'is_terminal' - OUT 

	  ---------------
	  'push_item' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'simple-exp', '2'

	  +++ ('simple-exp', '2') references the @p production "term", 
	  so, we push in onto the @p syntatic stack. The 'index' in
	  the @p syntatic stack references the position in the @p
	  symbols list imediatley before where the @p symbol objects
	  that form the "term" @p production. 

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'push_item' - OUT 

	  ---------------
	  'new_production' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'term', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'new_production' - OUT 

	  ==============================

	  ---------------
	  'end_of_production' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'term', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'end_of_production' - OUT 

	  ---------------
	  'accepts_anything' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'term', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'accepts_anything' - OUT 

	  ---------------
	  'is_terminal' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'term', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'is_terminal' - OUT 

	  ---------------
	  'push_item' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'term', '0'

	  +++ The (term, 0) item references the "factor" @p
	  production, so it is pushed onto the stack. The 'index' in
	  the @p syntatic stack references the position in the @p
	  symbols list imediatley before where the @p symbol objects
	  that form the "factor" @p production. 

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'push_item' - OUT 

	  ---------------
	  'new_production' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'factor', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'new_production' - OUT 

	  ==============================

	  ---------------
	  'end_of_production' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'factor', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'end_of_production' - OUT 

	  ---------------
	  'accepts_anything' - IN 
	  rc = false

	  symbol: '4', 'decimal_integer'
	  item: 'factor', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'accepts_anything' - OUT 

	  ---------------
	  'is_terminal' - IN 
	  rc = true

	  symbol: '4', 'decimal_integer'
	  item: 'factor', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'is_terminal' - OUT 

	  ---------------
	  'recognize_symbol' - IN 
	  symbol ('4' , 'decimal_integer') recognized in production
	  'factor', item '0' 
	  rc = true

	  symbol: '4', 'decimal_integer'
	  item: 'factor', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       

	  'recognize_symbol' - OUT 

	  ---------------
	  'push_symbol' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'factor', '0'

	  +++ As the symbol (4, decimal_integer) was recognized, its
	  pushed onto the @p symbols list. Pay attention that this @p
	  symbol occupies position 1 in the @p symbols list, so it is
	  the first @p symbol of the @p item on the top of the @p
	  syntatic stack, (term, 0) 

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       
	  1              4              decimal_integer

	  'push_symbol' - OUT 

	  ---------------
	  'exec_symbol_routine' - IN 

	  symbol: '4', 'decimal_integer'
	  item: 'factor', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	  simple-exp     2              0              
	  term           0              0              

	  symbols
	  index          value          type           
	  0              !x_value!      !x_type!       
	  1              4              decimal_integer

	  'exec_symbol_routine' - OUT 

	  ---------------
	  'read_symbol' - IN 

	  symbol: '+', 'reserved'
	  item: 'factor', '0'

	  syntatic
	  prod-id        item-id        index          
	  !root-prod!    0              0              
	  exp            0              0              
	simple-exp     2              0              
                term           0              0              

                symbols
                index          value          type           
                0              !x_value!      !x_type!       
                1              4              decimal_integer

                'read_symbol' - OUT 

                ---------------
                'follow_successor' - IN 

                symbol: '+', 'reserved'
                item: no-prod-owner, '-1'

                syntatic
                prod-id        item-id        index          
                !root-prod!    0              0              
                exp            0              0              
                simple-exp     2              0              
                term           0              0              

                symbols
                index          value          type           
                0              !x_value!      !x_type!       
                1              4              decimal_integer

                'follow_successor' - OUT 

                ==============================

                ---------------
                'end_of_production' - IN 
                rc = true

                symbol: '+', 'reserved'
                item: no-prod-owner, '-1'

                syntatic
                prod-id        item-id        index          
                !root-prod!    0              0              
                exp            0              0              
                simple-exp     2              0              
                term           0              0              

                symbols
                index          value          type           
                0              !x_value!      !x_type!       
                1              4              decimal_integer

                'end_of_production' - OUT 

                ---------------
                'empty_stack' - IN 
                rc = false

                symbol: '+', 'reserved'
                item: no-prod-owner, '-1'

                syntatic
                prod-id        item-id        index          
                !root-prod!    0              0              
                exp            0              0              
                simple-exp     2              0              
                term           0              0              

                symbols
                index          value          type           
                0              !x_value!      !x_type!       
                1              4              decimal_integer

                'empty_stack' - OUT 

                ---------------
                'exec_production_routine' - IN 

                symbol: '+', 'reserved'
                item: no-prod-owner, '-1'

                syntatic
                prod-id        item-id        index          
                !root-prod!    0              0              
                exp            0              0              
                simple-exp     2              0              
                term           0              0              

                symbols
                index          value          type           
                0              !x_value!      !x_type!       
                1              4              decimal_integer

                'exec_production_routine' - OUT 

                ---------------
                'pop_item' - IN 

                symbol: '+', 'reserved'
                item: 'term', '0'

                +++ We reached the end of the @p production "factor", so we
                pop the @p item (term, 0) where "factor" started.

                syntatic
                prod-id        item-id        index          
                !root-prod!    0              0              
                exp            0              0              
                simple-exp     2              0              

                symbols
                index          value          type           
                0              !x_value!      !x_type!       

                'pop_item' - OUT 

                ---------------
                'follow_successor' - IN 

                symbol: '+', 'reserved'
                item: 'term', '1'

                syntatic
                prod-id        item-id        index          
                !root-prod!    0              0              
                exp            0              0              
                simple-exp     2              0              

                symbols
                index          value          type           
                0              !x_value!      !x_type!       

                'follow_successor' - OUT
                 
                .
                . 
                . 

                And this goes on until the end of the parsing
                 
            */ 
        
            class syntatic_item {
            public:
                memory::ptr<const grammar::production::item> m_item;
                symbols::iterator m_before_first;
                syntatic_item ( memory::ptr<const grammar::production::item>
                                p_item, 
                                symbols::iterator p_before_first ) :
                    m_item ( p_item ) ,
                    m_before_first ( p_before_first ) {}
                    
            };

            /** @brief The stack of syntatic item */ 
            typedef list<syntatic_item> syntatic;

            /// @}                     ----------


            // ------------------------------------------------------------
            //                         stack
            /// @name                  Private Helper
            /// @{

            /** @brief Retrieves the index of a certain @p symbol::value in
                the @p symbols list  */  
            number::u16t index ( const symbol::value & p_symbol_value )
                const; 

            /// @}                     ----------

            // ------------------------------------------------------------
            //                         stack
            /// @name                  Private Attributes
            /// @{

            /** @brief The syntatic stack */    
            syntatic m_syntatic;

            /** @brief The symbols list */ 
            symbols m_symbols;
	    
            /// @}                     ----------

        };

    

    }
}


// ====================================================================
// ====================================================================
// ====================================================================

//         I M P L E M E N T A T I O N 

// ====================================================================
// ====================================================================
// ====================================================================


using namespace tenacitas::interpreter;


// #####################################################################
//                                  parser
            
        
        
// --------------------------------------------------------------------
template <typename t_grammar>
void
parser::
configure_scanner ( const t_grammar & p_grammar,
                    string::const_iterator p_begin,
                    string::const_iterator p_end ) {
    tin ;
    
    /// retrieving the collection of @p tenacitas::interpreter::symbol::value
    /// objects, along with their tenacitas::interpreter::symbol::type, that
    /// should be recognized a tenacitas::interpreter::symbol directly from the
    /// input text  
    m_scanner.set_tokens ( p_grammar.get_tokens ( ) ) ;

    /// retrieving the collection of tenacitas::interpreter::type_recognizer
    /// objects, which are able to recognize tenacitas::interpreter::symbol
    /// objects that conform to a format pattern 
    m_scanner.set_type_recognizers ( p_grammar.get_type_recognizers ( ) ) ;


    /// defining to the tenacitas::interpreter::scanner the text from where it
    /// will retrieve tenacitas::interpreter::symbol objects 
    m_scanner.set_text ( p_begin, p_end ) ;

    tout ;
    
}

// --------------------------------------------------------------------
template <typename t_grammar>
void
parser::
goto_first_item ( const t_grammar & p_grammar ) {
    tin ;
    m_item = p_grammar.get_first_production ( )->get_first ( );
    tout ;
}

// --------------------------------------------------------------------
template <typename t_grammar>
bool
parser::
parse ( string::const_iterator p_begin,
        string::const_iterator p_end,
        bool p_trace ) {

    tin ;

    bool l_rc = true;

    /// saving if we should print detailed messages of the parsing process
    m_trace = p_trace;

    /// an object of the @p grammar being analysed
    const t_grammar l_grammar;

    /// configuring the scanner to retrieve symbols from the input text
    configure_scanner ( l_grammar, p_begin, p_end ) ;
            
    /// from now on, we follow, as close as possible, the algorithm described at
    /// page 62 and 63 of "A construção de um Compilador", by Valdemar W. Setzer
    /// and  Inês S. Homem de Melo, Editora Campus, ISBN 85-7001-130-X

    /// reading a symbol from the input text
    read_symbol ( ) ;

    if ( m_symbol->get_value ( ) != symbol::value::eot ) {
        /// we actually have an input text
        try {
            /// initialize the stack of @p item objects that reference @p
            /// production objects, and the @p symbols list of recognized @p
            /// symbol objects
            initialize_stack ( ) ; 

            /// make the only @p item of the "root" production of the @p grammar
            /// the current @p item
            goto_first_item ( l_grammar ) ;

            /// push this @p item onto the stack of @p item objects that
            /// reference @p production objects
            push_item ( ) ; 

            /// and make the first @p item of this @p production referenced by
            /// this @p item, which is actually the first @p production inserted
            /// by the user, the  current @p item
            new_production ( ) ;

            while ( true ) {

                /// just print a visible mark that a new loop is begining
                begin_loop ( );
                
                if ( !end_of_production( ) ) {
                    /// a production is still being evaluated

                    if ( accepts_anything ( ) ) {
                        /// the current item accepts any symbol, so we go to its
                        /// sucessor, which will cause the end of the production
                        follow_successor ( ) ;
                    }
                    else if ( is_terminal ( ) ) {
                        /// the current item references a @p symbol::value or
                        /// @p symbol::type
                        if ( recognize_symbol ( ) ) {
                            /// symbol read was recognized

                            /// so we save this symbol in the @p symbols list of
                            /// recognized @p symbol objects
                            push_symbol ( ) ;

                            /// execute the @p routine associated to it, if any
                            exec_symbol_routine ( ) ;
                            
                            /// read a new symbol
                            read_symbol ( ) ;

                            /// and go to the successor item
                            follow_successor ( ) ;
                        }
                        else {
                            /// symbol was not recognized

                            if ( has_alternative ( ) ) {
                                /// and it has an alternative, so we follow it
                                follow_alternative ( ) ;
                            }
                            else {
                                /// but it has no alternative... error!
                                l_rc = false;
                                
                                // throw ( create_interpreter_fault (
                                //             parser, error_parsing_text,
                                //             m_scanner.get_line( ) ,
                                //             m_scanner.get_column ( ) ) ) ;
                            }
                        }
                    }
                    else {
                        /// it is not a terminal, i.e, it references a production

                        /// we save the item that is referencing this production
                        push_item ( ) ;

                        /// configure parser to start analysing a new
                        /// production, i.e., go to the first item of the new
                        /// production 
                        new_production ( ) ;
                    }
                }
                else {
                    /// end of a production

                    /// execute the @p routine associated to the just recognized
                    /// @p production, i.e., the @p routine associated to the @p
                    /// item where the just recognized @p production started
                    memory::ptr<symbol> l_symbol = exec_production_routine ( ) ;

                    /// remove this @p production from the stack of to be
                    /// recognized productions  
                    pop_item ( ) ;

                    if ( l_symbol ) {
                        /// if the @p routine executed returned any @p symbol,
                        /// so we save this symbol in the @p symbols list of
                        /// recognized @p symbol objects
                        push_symbol ( l_symbol ) ;
                    }
                    
                    if ( !empty_stack ( ) ) {
                        /// after the @p pop_item, the stack of @p item objects
                        /// that referenced @p production is not empty, so we
                        /// follow the sucessor @p item of the @p item that
                        /// referenced the just recognized @p production
                        follow_successor ( ) ;
                    }

                    else {
                        /// after the @p pop_item, the stack of @p item objects
                        /// that referenced @p production is empty
                        if ( !end_of_input ( ) ) {
                            /// but the input text was not all read... error!
                            l_rc = false;
                            
                            // throw ( create_interpreter_fault
                            //         ( parser, error_parsing_text,
                            //           m_scanner.get_line( ) ,
                            //           m_scanner.get_column ( ) ) ) ;
                        }
                        else {
                            /// success!!!
                            break;
                        }
                    }
                }
            }
        }
        catch ( number::fault & e ) {
            l_rc = false;
            
            // throw ( interpreter::fault ( e,
            //                              m_scanner.get_line ( ) ,
            //                              m_scanner.get_column ( ) ) );
        }
    }
    tout ;
    
}


#endif

#include "fault.h"

using namespace tenacitas::interpreter;


// ############################################################################
//                                  fault

// ----------------------------------------------------------------------------
interpreter::fault::
fault ( const string & p_class_name,
        const fault::code & p_code,
        const string & p_method_name,
        const string & p_file_name,
        const number::u32t & p_line_number,
        const line_number & p_input_text_line,
        const column_number &  p_input_text_column ) :
    tenacitas::number::fault ( p_class_name,
                               p_code,
                               p_method_name,
                               p_file_name,
                               p_line_number) ,
    m_input_text_line ( p_input_text_line ) ,
    m_input_text_column ( p_input_text_column ) {}


// ----------------------------------------------------------------------------
interpreter::fault::
fault ( const tenacitas::number::fault & e,
        const line_number & p_input_text_line ,
        const column_number &  p_input_text_column ) :
    tenacitas::number::fault ( e ) ,
    m_input_text_line ( p_input_text_line ) ,
    m_input_text_column ( p_input_text_column ) {}

// ----------------------------------------------------------------------------
interpreter::fault::
~fault ( ) throw ( ) { }

// ----------------------------------------------------------------------------
line_number
interpreter::fault::
get_line ( ) const {return m_input_text_line;}


// ----------------------------------------------------------------------------
column_number
interpreter::fault::
get_column ( ) const {return m_input_text_column;}


// ----------------------------------------------------------------------------
const char*
interpreter::fault::
what() const throw() {
    static ostringstream l_what ;
    l_what << tenacitas::number::fault::what ( )
           << ", input text line = " << get_line ( )
           << ", input text column = " << get_column ( ) ;

    return l_what.str( ).c_str( ) ;
}




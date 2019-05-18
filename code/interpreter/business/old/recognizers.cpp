#include "recognizers.h"

using namespace tenacitas::interpreter;




// ############################################################################
//                                     word_recognizer

const symbol::type word_recognizer::word ( "word" ) ;

//----------------------------------------------------------------------------
inline word_recognizer::
word_recognizer ( ) :
    type_recognizer ( recognizer_id ( "word_recognizer" ),
                      string ( "word" ) ) { }

//----------------------------------------------------------------------------
bool
word_recognizer::
try_to_recognize ( string::const_iterator p_end,
                   string::const_iterator & p_walker,
                   string & p_str_symbol_type ) const {
    
    bool rc = true;
    
    if ( !isalpha ( *p_walker ) ) {
        rc = false;
    }
    else {
        while ( true ) {
            if ( !isalpha ( *p_walker ) ) {
                break;
            }
            ++p_walker;
        }
    }

    if ( rc ) {
        p_str_symbol_type = string ( "word" ) ;
    }
    

    return rc;

}



// ############################################################################
//                                    number_recognizer


const symbol::type number_recognizer::real ( "real" ) ;
const symbol::type number_recognizer::decimal_integer ( "decimal_integer" ) ;


//----------------------------------------------------------------------------
inline number_recognizer::
number_recognizer ( char p_decimal_separator ) :
    type_recognizer ( recognizer_id ( "number_recognizer" ),
                      string ( "decimal_integer real" ) ),
    m_dec_sep ( p_decimal_separator ) { }

//----------------------------------------------------------------------------
bool
number_recognizer::
try_to_recognize ( string::const_iterator p_end,
                   string::const_iterator & p_walker,
                   string & p_str_symbol_type ) const {

    bool rc = false;

    if ( !isdigit ( *p_walker ) ) {
	rc = false;
	goto end_;
    }

    goto digit_found_;


  digit_found_:
    {
	while ( isdigit ( *p_walker ) ) {
	    ++p_walker;
	}

	if ( *p_walker == m_dec_sep ) {
	    ++p_walker;
	    goto dec_sep_found_;
	}

	rc = true;
	p_str_symbol_type = "decimal_integer";
	goto end_;
    }

  dec_sep_found_:
    {
	if ( !isdigit ( *p_walker ) ) {
	    rc = false;
	    goto end_;
	}
	
	while ( isdigit ( *p_walker ) ) {
	    ++p_walker;
	}

	rc = true;
	p_str_symbol_type = "real";
	goto end_;
    }
    

  end_:
    ;
	

//     if ( !isdigit ( *p_walker ) ) {
// 	rc = false;
//     }
//     else {
// 	while ( isdigit ( *p_walker ) ) {
// 	    ++p_walker;
// 	}

// 	if ( *p_walker == m_dec_sep ) {
// 	    ++p_walker;

// 	    if ( !isdigit ( *p_walker ) ) {
// 		rc = false;
// 	    }
// 	    else {
// 		while ( isdigit ( *p_walker ) ) {
// 		    ++p_walker;
// 		}

// 		rc = true;
// 		p_str_symbol_type = "real";
// 	    }
// 	}
// 	else {
// 	    rc = true;
// 	    p_str_symbol_type = "decimal_integer";
// 	}
//     }
	
    return rc;
}



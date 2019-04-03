#include "type_recognizer.h"

using namespace tenacitas::interpreter;


// ############################################################################
//                                    type_recognizer

const tenacitas::number::fault::code
type_recognizer::error_type_not_registered ( 1 ) ;


const tenacitas::number::fault::code
type_recognizer::error_empty_info_string ( 2 ) ;



// --------------------------------------------------------------------
const recognizer_id &
type_recognizer::
get_id ( ) const {return m_reco_id;}

// --------------------------------------------------------------------
bool
type_recognizer::
operator == ( const type_recognizer & p_reco ) const {
    return m_reco_id == p_reco.m_reco_id;
}

// --------------------------------------------------------------------
bool
type_recognizer::
operator != ( const type_recognizer & p_reco ) const  {
    return m_reco_id != p_reco.m_reco_id;
}


// --------------------------------------------------------------------
type_recognizer::
type_recognizer ( const recognizer_id & p_reco_id,
             const string & p_symbols_types ) :
    m_reco_id ( p_reco_id ),
    m_symbols_types ( new symbols_types ( p_symbols_types ) ) {

} 

// --------------------------------------------------------------------
type_recognizer::
~type_recognizer ( ) {
//     cout << "~type_recognizer" << endl;
}


// --------------------------------------------------------------------
memory::ptr<const symbols_types>
type_recognizer::
get_recognizeds ( ) const {return m_symbols_types;}

// --------------------------------------------------------------------
        
memory::ptr<const symbol::type>
type_recognizer::
recognizes ( const symbol::type & p_symbol_type ) const {
    return ( m_symbols_types->find ( p_symbol_type ) ) ;
}


// ----------------------------------------------------------------------------
memory::ptr<const symbol>
type_recognizer::
recognize ( string::const_iterator p_end,
            string::const_iterator & p_walker ) const {

    memory::ptr<const symbol> rc;

    /// saving the current referenced position in the input text
    string::const_iterator l_walker = p_walker;

    /// possible @p symbol::type of the recognized symbol
    string l_str_symbol_type;
    

    /// trying to recognize
    if ( try_to_recognize ( p_end, p_walker, l_str_symbol_type ) ) {

        /// ok, a @p symbol was recognized

        /// but the programmer who wrote @p try_to_recognize set a valid @p
        /// symbol::type in @p l_str_symbol::type?  
        symbol::type l_symbol_type ( l_str_symbol_type );
        memory::ptr<const symbol::type> l_ptr_symbol_type =
            recognizes ( l_symbol_type );

        if ( l_ptr_symbol_type ) {

            /// ok, we have a valid @p symbol::type

            /// assembling the @p symbol::value
            memory::ptr<const symbol::value> l_ptr_symbol_value;

            if ( std::distance( p_walker, l_walker ) == 0 ) {
                l_ptr_symbol_value =
                    memory::ptr<const symbol::value>
                    ( new symbol::value ( string ( 1, *p_walker ) ) ) ;
            }
            else {
                l_ptr_symbol_value =
                    memory::ptr<const symbol::value>
                    ( new symbol::value ( string ( l_walker, p_walker ) ) ) ;
            }

            /// creating the recognized @p symbol object 
            rc = memory::ptr<symbol>
                ( new symbol ( *l_ptr_symbol_value, *l_ptr_symbol_type ) ) ;
        
        }
    }
    return rc;
}



// ############################################################################
//                                    type_recognizers


const tenacitas::number::fault::code
type_recognizers::
error_recognizer_already_registered ( 1 ) ;


const tenacitas::number::fault::code
type_recognizers::
error_info_already_recognized ( 2 ) ;


// -----------------------------------------------------------------------------
type_recognizers::
type_recognizers ( ) :
    m_type_recognizers ( ) {}


// -----------------------------------------------------------------------------
type_recognizers::
~type_recognizers ( ) {
//     cout << "~type_recognizers" << endl;
}

// -----------------------------------------------------------------------------
type_recognizers::const_iterator
type_recognizers::
begin ( ) const {return m_type_recognizers.begin ( ) ;}


// -----------------------------------------------------------------------------
type_recognizers::const_iterator 
type_recognizers::
end ( ) const  {return m_type_recognizers.end ( ) ;}


// -----------------------------------------------------------------------------
memory::ptr<const type_recognizer>
type_recognizers::
find ( const recognizer_id & p_reco_id ) const {
    
    memory::ptr<const type_recognizer> rc;

    for ( const_iterator i = m_type_recognizers.begin( ) ; 
          i != m_type_recognizers.end( ) ;
          ++i ) {
        if ( ( *i )->get_id ( ) == p_reco_id ) {
            rc = *i;
            break;
        }
    }
    return rc;
}


// -----------------------------------------------------------------------------
memory::ptr<const symbol::type>
type_recognizers::
find ( const symbol::type & p_symbol_type ) const { 

    memory::ptr<const symbol::type> rc;

    for ( const_iterator i = begin ( ) ; i != end ( ) ; ++i ) {
        rc = ( *i )->recognizes ( p_symbol_type );
        if ( rc != 0 ) {
            break;
        }
    }
    
    return rc;
}


// -----------------------------------------------------------------------------
memory::ptr<const symbol>
type_recognizers::
recognize ( string::const_iterator p_end,
            string::const_iterator & p_walker ) const {

    memory::ptr<const symbol> rc;

    for ( const_iterator recos_ite = m_type_recognizers.begin ( ) ; 
          recos_ite != m_type_recognizers.end( ) ;
          ++recos_ite ) {

        memory::ptr<const type_recognizer> l_reco = *recos_ite;
        
        /// if the @p recos_ite recognizer recognizes a symbol, we stop
        rc = l_reco->recognize ( p_end, p_walker );

        if ( rc != 0 ) {
            break;
        }
    }

    return rc;
}


// ----------------------------------------------------------------------------
void
type_recognizers::
add ( memory::ptr<const type_recognizer> p_reco ) {

    if ( !find ( p_reco->get_id ( ) ) ) {
        if ( may_add ( p_reco ) ) {
            m_type_recognizers.push_front ( p_reco ) ;
        }
    }
}


// ----------------------------------------------------------------------------
bool
type_recognizers::
may_add ( memory::ptr<const type_recognizer> p_reco ) const  {

    bool l_rc = true;
    

    /// retrieves all the @p t_symbol_info objects that the candidate @p
    /// p_reco recognizes 
    memory::ptr<const symbols_types> l_symbols_info =
        p_reco->get_recognizeds ( ) ;

    /// for each of these @p symbol::type objects...
    symbols_types::const_iterator ite_symbols = l_symbols_info->begin( );
            
    for ( ; ite_symbols != l_symbols_info->end ( ) ; ++ite_symbols ) {

        /// checks if any of the <tt> recognizer<t_symbol_info> </tt> in
        /// the collection already recognizes it
        
        if ( find( **ite_symbols ) ) {
            /// ups! we can not allow two <tt> recognizer<t_symbol_info>
            /// </tt> recognizing the same @p t_symbol_info
            l_rc = false;
            break;
        }
    }
    break l_rc;
    
}
            

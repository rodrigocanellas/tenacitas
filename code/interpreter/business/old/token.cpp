#include "token.h"

using namespace tenacitas::interpreter;



// ############################################################################
//                                  tokens::token

// ----------------------------------------------------------------------------
tokens::token::
token ( const string & p_symbol_values,
        const symbol::type & p_symbol_type ) :
//     m_values ( new  symbols_info <symbol::value> ( p_symbol_values ) )  ,
    m_values ( new  symbols_values ( p_symbol_values ) )  ,
    m_type ( new symbol::type ( p_symbol_type ) ) {}


// ----------------------------------------------------------------------------
tokens::token::
~token ( ) {
 //    cout << "~token" << endl;
}

// ----------------------------------------------------------------------------
tokens::token::const_iterator
tokens::token::
begin ( ) const {return m_values->begin ( ) ;}

// ----------------------------------------------------------------------------
tokens::token::const_iterator
tokens::token::
end ( ) const {return m_values->end ( ) ;}

// ----------------------------------------------------------------------------
memory::ptr<const symbol::value>
tokens::token::
is_defined ( const symbol::value & p_symbol_value ) const {
    return m_values->find ( p_symbol_value ) ;
}

// ----------------------------------------------------------------------------
memory::ptr<const symbol::type>
tokens::token::
get_type ( ) const {return m_type;}

               

// ############################################################################
//                                  tokens

// ----------------------------------------------------------------------------
tokens::
tokens ( ) {}


// ----------------------------------------------------------------------------
tokens::
~tokens ( ) { }

// ----------------------------------------------------------------------------
void
tokens::
add ( const string & p_values, const symbol::type & p_type ) {
    m_list.push_back (
        memory::ptr<token> ( new token ( p_values, p_type ) )
        ) ;
}

// ----------------------------------------------------------------------------
void
tokens::
add ( const symbol::value & p_value,
      const symbol::type & p_type ) {

    string l_str ( p_value ) ;
    
    m_list.push_back (
        memory::ptr<token> ( new token ( l_str, p_type ) )
        ) ;
    
}

// ----------------------------------------------------------------------------
memory::ptr<const symbol>
tokens::
is_defined ( const symbol::value & p_symbol_value ) const {
    memory::ptr<const symbol> rc;

    memory::ptr<const symbol::value> l_symbol_value_ptr;

    /// for all the @p token objects...
    for ( token_list::const_iterator l_ite = m_list.begin ( ) ;
          l_ite != m_list.end( ) ;
          ++l_ite ) {
        /// if the current @p token object recognizes the @p p_symbol_value
        /// informed 
        l_symbol_value_ptr = ( *l_ite )->is_defined ( p_symbol_value ) ;
        if ( l_symbol_value_ptr ) {
            /// we create a @p symbol object based on the <tt> symbol::value
            /// </tt> and on the <tt> symbol::type </tt> of the @p token object
            /// found 
            rc = memory::ptr<symbol> (
                new symbol ( *l_symbol_value_ptr,
                             *( *l_ite )->get_type ( ) ) ) ;
            break;
        }
    }

    return rc;
}

// ----------------------------------------------------------------------------
void
tokens::
import ( const tokens & p_tokens ) {

    /// for all the @p token objects being imported...
    for ( token_list::const_iterator tokens_ite = p_tokens.m_list.begin ( ) ;
          tokens_ite != p_tokens.m_list.end ( ) ;
          ++tokens_ite ) {

        /// this is just to make the code more readable
        memory::ptr<const token> l_token = *tokens_ite;

        /// for all the <tt> symbol::value </tt> objects of the current @p token
        /// object... 
        for ( tokens::token::const_iterator token_ite = l_token->begin ( ) ;
              token_ite != l_token->end ( );
              ++token_ite ) {

            /// checking if the current <tt> symbol::value </tt> is defined in
            /// this @p tokens object
            memory::ptr<const symbol> l_symbol = is_defined ( **token_ite );

            if ( l_symbol == 0 ) {
                /// it is not, so we can add to the collection of @p tokens
                add ( **token_ite, *l_token->get_type ( ) );
            }
            else {
                /// there is already a <tt> symbol::value </tt> in this @p tokens
                /// object 
                
                /// if this <tt> symbol::value </tt> defined in the imported @p
                /// tokens has a different <tt> symbol::type </tt>, we
                /// abort. why? because our dear user, a programmer, is dealing
                /// with a fatal inconsistency: either the <tt> symbol::value
                /// </tt> has the <tt> symbol::type </tt> in the imported @p
                /// tokens, or the one defined in this @p tokens

                assert ( *( l_token->get_type ( ) ) != l_symbol->get_type ( ) );
            }
        }
    }    

    

}

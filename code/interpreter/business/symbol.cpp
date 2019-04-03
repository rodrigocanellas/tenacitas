#include "symbol.h"


using namespace tenacitas::interpreter;



// ############################################################################
//                                    symbol


// ----------------------------------------------------------------------------
symbol::
symbol ( const symbol::value & p_value,
         const symbol::type & p_type) :
    m_value ( p_value ),
    m_type ( p_type ) {}

// ----------------------------------------------------------------------------
symbol::
~symbol ( ) {
}

// ----------------------------------------------------------------------------
const symbol::value &
symbol::
get_value ( ) const {return m_value;}

// ----------------------------------------------------------------------------
const symbol::type &
symbol::
get_type ( ) const {return m_type;}

// ----------------------------------------------------------------------------
void
symbol::
set_value ( const symbol::value & p_symbol_value ) {
    m_value = p_symbol_value;
}

// ----------------------------------------------------------------------------
void
symbol::
set_type ( const symbol::type & p_symbol_type ) {
    m_type = p_symbol_type;
}

// -----------------------------------------------------------------------------
bool
symbol::
operator == ( const symbol & p_symbol ) const {
    return ( ( m_value == p_symbol.m_value ) &&
             ( m_type == p_symbol.m_type ) ) ;
}

// -----------------------------------------------------------------------------
bool
symbol::
operator != ( const symbol & p_symbol ) const  {
    return ( ( m_value != p_symbol.m_value ) ||
             ( m_type != p_symbol.m_type ) ) ;
}

// -----------------------------------------------------------------------------
symbol &
symbol::
operator = ( const symbol & p_symbol ) {
    if ( this != & p_symbol ) {
        m_value = p_symbol.m_value;
        m_type = p_symbol.m_type;
    }
    return *this;
}


// ############################################################################
//                                    symbol::value

const symbol::value symbol::value::eot  ( "!!eot!!" );

// ----------------------------------------------------------------------------
symbol::value::
~value ( ) {}


// ----------------------------------------------------------------------------
symbol::value::
value ( const string & p_string ) :
    m_string ( p_string ) {}

// ----------------------------------------------------------------------------
symbol::value::
operator string ( ) const {return m_string;}

// ----------------------------------------------------------------------------
bool
symbol::value::
operator == ( const symbol::value & p_symbol_value ) const {
    return m_string == p_symbol_value.m_string;
}

// ----------------------------------------------------------------------------
bool
symbol::value::
operator != ( const symbol::value & p_symbol_value ) const  {
    return m_string != p_symbol_value.m_string;
}

// ----------------------------------------------------------------------------
symbol::value &
symbol::value::
operator = ( const symbol::value & p_symbol_value )  {
    m_string = p_symbol_value.m_string;
    return *this;
}



// ############################################################################
//                                    symbol::type

const symbol::type symbol::type::eot  ( "!!eot!!" );

// ----------------------------------------------------------------------------
symbol::type::
~type ( ) {}

// ----------------------------------------------------------------------------
symbol::type::
type ( const string & p_string ) :
    m_string ( p_string ) {}

// ----------------------------------------------------------------------------
symbol::type::
operator string ( ) const {return m_string;}

// ----------------------------------------------------------------------------
bool
symbol::type::
operator == ( const symbol::type & p_symbol_type ) const {
    return m_string == p_symbol_type.m_string;
}

// ----------------------------------------------------------------------------
bool
symbol::type::
operator != ( const symbol::type & p_symbol_type ) const  {
    return m_string != p_symbol_type.m_string;
}

// ----------------------------------------------------------------------------
symbol::type &
symbol::type::
operator = ( const symbol::type & p_symbol_type )  {
    m_string = p_symbol_type.m_string;
    return *this;
}


// ############################################################################
//                                  symbols

//----------------------------------------------------------------------------
symbols::
symbols ( ) :
    m_list ( ) {}

//----------------------------------------------------------------------------
symbols::
~symbols ( ) {}

//----------------------------------------------------------------------------
void
symbols::
add ( memory::ptr<symbol> p_symbol ) {
    m_list.push_back ( p_symbol ) ;
}

//----------------------------------------------------------------------------
symbols::iterator
symbols::
begin ( ) {return m_list.begin ( ) ;}

//----------------------------------------------------------------------------
symbols::iterator
symbols::
end ( ) {return m_list.end( ) ;}

//----------------------------------------------------------------------------
symbols::const_iterator
symbols::
begin ( ) const {return m_list.begin ( ) ;}

//----------------------------------------------------------------------------
symbols::const_iterator
symbols::
end ( ) const {return m_list.end( ) ;}

//----------------------------------------------------------------------------
void
symbols::
erase ( iterator p_begin, iterator p_end ) {m_list.erase ( p_begin, p_end ) ;}


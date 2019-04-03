#include "parser.h"






// ############################################################################
//                                  parser

const tenacitas::number::u16t parser::error_parsing_text ( 1 ) ;

// ----------------------------------------------------------------------------
parser::
parser ( const string & p_to_ignore ) :
    m_stream ( cerr ), 
    m_scanner ( p_to_ignore ),
    m_trace ( false ) {}

// ----------------------------------------------------------------------------
parser::
~parser ( ) {
    tin ;
    tout ;
}



// ----------------------------------------------------------------------------
bool
parser::
recognize_symbol ( )const {
    tin ;
    
    bool rc = false;
    
    if (
        ( ( m_item->get_symbol_value ( ) != 0 )  &&
          /// and the symbol read has this same @p symbol::value 
          ( *m_item->get_symbol_value ( ) == m_symbol->get_value ( ) )
            ) 

        ||

        ( ( m_item->get_symbol_type ( ) != 0 )  &&
          /// and the symbol read has this same @p symbol::type 
          ( *m_item->get_symbol_type ( ) == m_symbol->get_type ( ) )
            )
        )  {

        if ( m_trace ) {
            
            m_stream
                << "symbol ('" << m_symbol->get_value ( ) <<
                "' " << ", '" << m_symbol->get_type ( ) <<
                "') " << "recognized in production '" <<
                m_item->get_production_owner( )->get_id ( )
                << "', item '" << m_item->get_id ( ) << "'"
                << endl;
        }
        
        rc = true;
    }

    if ( m_trace ) {
        m_stream << "rc = "  << ( rc ? "true" : "false" ) << endl;
    }
    

    tout ;
    
    return rc;
    
}



// ----------------------------------------------------------------------------
void
parser::
initialize_stack ( ) {
    tin ;
    m_stack = memory::ptr<stack> ( new stack ( ) ) ;
    tout ;
    
}


// ----------------------------------------------------------------------------
bool
parser::
end_of_production ( ) const {
    tin ;
    
    
//     return ( ( m_item->get_suc ( ) == 0 ) ||
//              ( m_item->accepts_anything ( ) ) ) ;
    bool rc = m_item->is_last ( );
    if ( m_trace ) {
        m_stream << "rc = "  << ( rc ? "true" : "false" ) << endl;
    }
    

    tout ;
    
    return  rc ;

}

// ----------------------------------------------------------------------------
bool
parser::
points_to_production ( ) const {
    tin ;
    
    bool rc = m_item->points_to_production ( ) ;

    if ( m_trace ) {
        m_stream << "rc = "  << ( rc ? "true" : "false" ) << endl;
    }
    

    tout ;
    
    return rc;
}



// ----------------------------------------------------------------------------
void 
parser::
read_symbol ( ) {
    tin ;
    
    m_symbol = m_scanner.get_symbol ( ) ;

    tout ;
    
}


// ----------------------------------------------------------------------------
bool
parser::
end_of_input ( ) const {
    tin ;
    
    bool rc = ( m_symbol->get_value ( ) == symbol::value::eot ) ;
    if ( m_trace ) {
        m_stream << "rc = "  << ( rc ? "true" : "false" ) << endl;
    }
    
    tout ;
    
    return rc;
}

// ----------------------------------------------------------------------------
void 
parser::
follow_successor ( ) {
    tin ;
    m_item = m_item->get_suc ( ) ;
    tout ;
    
}

// ----------------------------------------------------------------------------
bool
parser::
empty_stack ( ) const {
    tin ;
    
    bool rc = m_stack->empty ( ) ;

    if ( m_trace ) {
        m_stream << "rc = "  << ( rc ? "true" : "false" ) << endl;
    }

    tout ;
    
    return rc;
    
}

// ----------------------------------------------------------------------------
bool
parser::    
is_terminal ( ) const {
    tin ;
    
    bool rc = ( ( m_item->points_to_value ( ) ) ||
                ( m_item->points_to_type ( ) ) ) ;

    if ( m_trace ) {
        m_stream << "rc = "  << ( rc ? "true" : "false" ) << endl;
    }
    

    tout ;
    
    return rc;
    
}

// ----------------------------------------------------------------------------
bool
parser::
has_alternative ( ) const {
    tin ;
    
    bool rc =  ( m_item->get_alt ( ) != 0 ) ;
    if ( m_trace ) {
        m_stream << "rc = " << ( rc ? "true" : "false" ) << endl;
    }
    
    tout ;

    return rc;
    
}


// ----------------------------------------------------------------------------
void
parser::
follow_alternative ( ) {
    tin ;
    
    m_item = m_item->get_alt ( ) ;

    tout ;
    
}

// ----------------------------------------------------------------------------
void 
parser::
pop_item ( ) {

    tin ;
    
    m_item = m_stack->pop ( ) ;

    tout ;
}


// ----------------------------------------------------------------------------
void
parser::
push_item ( ) {
    tin ;
    
    m_stack->push ( m_item ) ;

    tout ;
    
}


// ----------------------------------------------------------------------------
void
parser::
push_symbol ( ) {
    tin ;
    
    m_stack->push ( memory::ptr<symbol> ( new symbol ( *m_symbol ) ) ) ;

    tout ;
}


// ----------------------------------------------------------------------------
void
parser::
push_symbol ( memory::ptr<symbol> p_symbol ) {
    tin ;
    
    m_stack->push ( memory::ptr<symbol> ( new symbol ( *p_symbol ) ) ) ;

    tout ;
}

    

// ----------------------------------------------------------------------------
void
parser::
new_production ( ) {
    tin ;
    m_item = m_item->get_production( )->get_first ( ) ;
    tout ;
    
}

// ----------------------------------------------------------------------------
bool
parser::
accepts_anything ( ) const {
    tin ;
    
    bool rc = m_item->accepts_anything ( ) ;

    if ( m_trace ) {
        m_stream << "rc = "  << ( rc ? "true" : "false" ) << endl;
    }
    
    tout ;
    
    return rc;
    
}
        
// --------------------------------------------------------------------
memory::ptr<symbol>
parser::
exec_routine ( memory::ptr<const grammar::production::item> p_item ) {
    memory::ptr<symbol> rc;
    
    if ( p_item ) {
        
        memory::ptr<const routine> l_routine = p_item->get_routine ( ) ;

        if ( l_routine ) {
            rc = l_routine->exec ( m_stack->first_symbol ( ) ,
                                   m_stack->last_symbol ( ) ) ;
        }
    }
    return rc;
}

// --------------------------------------------------------------------
void
parser::
exec_symbol_routine (  ) {
    tin ;
    memory::ptr<symbol> l_symbol = exec_routine ( m_item ) ;

    if ( l_symbol ) {
        m_stack->replace ( l_symbol ) ;
    }

    tout ;
}


// --------------------------------------------------------------------
memory::ptr<symbol>
parser::
exec_production_routine ( ) {
    tin ;    
    memory::ptr<symbol> rc;
    
    memory::ptr<const grammar::production::item> l_item =
        m_stack->get_top_item ( ) ;

    rc = exec_routine ( l_item ) ;
    tout ;
    return rc;
}

// --------------------------------------------------------------------
void
parser::
report ( ) const {
    if ( m_trace ) {
        m_stream << endl;
    
        if ( m_symbol ) {
        
            m_stream << "symbol: "
                     << "'" << m_symbol->get_value ( ) << "', "
                     << "'" << m_symbol->get_type ( ) << "'";
        
        }
        else {
            m_stream << "no-symbol";
        }
        m_stream  << endl;

        if ( m_item ) {
        
            m_stream << "item: " ;
    
            if ( m_item->get_production_owner ( ) ) {
                m_stream << "'" << m_item->get_production_owner ( )->get_id ( )
                         << "', " ;
            }
            else {
                m_stream << "no-prod-owner, ";
            }
            m_stream << "'" << m_item->get_id ( ) << "'";
        }
        else {
            m_stream << "no-item" ;
        }
        m_stream << endl;
        
    
        if ( m_stack ) {
            m_stack->report ( m_stream ) ;
        }
    }
    
}

// --------------------------------------------------------------------
void
parser::
begin_loop ( ) {
    if ( m_trace ) {
        m_stream << "\n==============================" << endl;
    }
}

    

// ############################################################################
//                                  parser::stack

// --------------------------------------------------------------------
parser::stack::
~stack ( ) {}


// --------------------------------------------------------------------
parser::stack::
stack ( ) :
    m_syntatic ( ) ,
    m_symbols ( ) {

    m_symbols.add ( memory::ptr<symbol>
                    ( new symbol ( symbol::value ( "!x_value!" ),
                                   symbol::type ( "!x_type!") ) ) )  ;

}

// --------------------------------------------------------------------
void
parser::stack::
push ( memory::ptr<const grammar::production::item> p_item ) {

    m_syntatic.push_back ( syntatic_item ( p_item, --m_symbols.end ( ) ) ) ;
}


// --------------------------------------------------------------------
memory::ptr<const grammar::production::item>
parser::stack::
pop ( ) {
//     syntatic::const_iterator l_syntatic_ite m_syntatic.end( ) ;
//     --l_syntatic_ite;
    memory::ptr<const grammar::production::item> rc =
        m_syntatic.back( ).m_item;

    m_symbols.erase ( first_symbol ( ) , m_symbols.end( ) ) ;

    m_syntatic.erase ( --m_syntatic.end( ) ) ;
    return rc;
}

    
// --------------------------------------------------------------------
void
parser::stack::
push ( memory::ptr<symbol> p_symbol ) {m_symbols.add ( p_symbol ) ;}


// --------------------------------------------------------------------
symbols::iterator
parser::stack::
first_symbol ( )  {
    symbols::iterator l_first = m_syntatic.back( ).m_before_first;
    ++l_first;
    return l_first;
}


// --------------------------------------------------------------------
symbols::iterator
parser::stack::
last_symbol ( ) {
    symbols::iterator l_end = m_symbols.end( ) ;
//     --l_end;
    return l_end;
}


// --------------------------------------------------------------------
bool
parser::stack::
empty ( ) const {return m_syntatic.empty ( ) ;}


// --------------------------------------------------------------------
void
parser::stack::
replace ( memory::ptr<symbol> p_symbol ) {

    symbols::iterator l_ite = m_symbols.end( ) ;
    --l_ite;
    *l_ite = p_symbol;
}

// --------------------------------------------------------------------
memory::ptr<const grammar::production::item>
parser::stack::get_top_item ( ) {
    memory::ptr<const grammar::production::item> rc;
    
    if ( !m_syntatic.empty( ) ) {
        rc =  m_syntatic.back ( ).m_item;
    }
    return rc;
}

// --------------------------------------------------------------------
number::u16t
parser::stack::
index ( const symbol::value & p_symbol_value ) const {
    number::u16t rc = 0;
    symbols::const_iterator m_ite = m_symbols.begin ( ) ;
    
    for ( ; m_ite != m_symbols.end ( ) ; ++m_ite ) {
        if ( ( *m_ite )->get_value ( ) == p_symbol_value ) {
            break;
        }
        ++rc;
    }
    return rc;
}



// --------------------------------------------------------------------
void
parser::stack::
report ( ostream & p_stream )  const {
    p_stream << "\nsyntatic" << endl;

    const number::u8t w = 15;
    
    p_stream.width ( w ) ; p_stream.fill ( ' ' ) ; 

    p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
    p_stream << left << "prod-id" ;
    p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
    p_stream << left << "item-id";
    p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
    p_stream << left << "index" << endl;
    
    for ( syntatic::const_iterator l_synt_ite = m_syntatic.begin ( ) ;
          l_synt_ite != m_syntatic.end ( ) ;
          ++l_synt_ite ) {

        p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
        p_stream << ( l_synt_ite->m_item->get_production_owner( ) ?
                      l_synt_ite->m_item->get_production_owner( )->get_id ( ) :
                      string( "no-owner" ) ) ;

        p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
        p_stream << l_synt_ite->m_item->get_id ( );

        p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
        if ( l_synt_ite->m_before_first != m_symbols.end( ) ) {
            p_stream << index ( ( *l_synt_ite->m_before_first)->get_value ( ) ) ;
        }
        else {
            p_stream << string ( "no-symbol" );
        }
        
        p_stream << endl;

    }

    p_stream << endl;

    p_stream << "symbols" << endl;

    number::u16t l_u16 = 0;
    
    p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
    p_stream << left<< "index";
    p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
    p_stream << left << "value";
    p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
    p_stream << left <<  "type" << endl;
    
    for ( symbols::const_iterator l_symb_ite = m_symbols.begin ( ) ;
          l_symb_ite != m_symbols.end ( ) ;
          ++l_symb_ite ) {

        p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
        p_stream << l_u16++ ;

        p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;
        p_stream << ( *l_symb_ite )->get_value ( ) ;

        p_stream.width ( w ) ; p_stream.fill ( ' ' ) ;        
        p_stream << ( *l_symb_ite )->get_type ( ) ;

        p_stream << endl;
    }
    
}


#include "grammar.h"


// // ############################################################################
// //                                  grammar


const tenacitas::number::u16t
grammar::error_production_colision ( 1 ) ;
            
const tenacitas::number::u16t
grammar::error_routine_colision ( 2 ) ;
            
const tenacitas::number::u16t
grammar::error_production_exists ( 3 );
            
const tenacitas::number::u16t
grammar::error_recognizer_exists ( 4 );
            
const tenacitas::number::u16t
grammar::error_routine_exists ( 5 );
            
const tenacitas::number::u16t
grammar::error_item_exists ( 6 );
            
const tenacitas::number::u16t
grammar::error_non_existing_production ( 7 );
            
const tenacitas::number::u16t
grammar::error_non_existing_item ( 8 );

const tenacitas::number::u16t
grammar::error_item_refs_type ( 9 ) ;
            
const tenacitas::number::u16t
grammar::error_non_existing_recognizer ( 10 );
            
// const tenacitas::number::u16t
// grammar::error_non_existing_routine ( 11 );

const tenacitas::number::u16t
grammar::error_item_refs_production ( 12 ) ;

const tenacitas::number::u16t
grammar::error_item_has_successor ( 13 ) ;

const tenacitas::number::u16t
grammar::error_item_has_alternative ( 14 ) ;

// const tenacitas::number::u16t
// grammar::error_item_refs_routine ( 15 ) ;

const tenacitas::number::u16t
grammar::error_item_refs_value ( 16 ) ;



// ----------------------------------------------------------------------------
grammar::
grammar( const grammar_id & p_id ) :
    m_id( p_id ),
    m_type_recognizers ( new type_recognizers( ) ) ,
    m_tokens ( new tokens( ) ) {

    /// this production is like the "root" production of the grammar;
    /// in other terms, it is the production from where all the other
    /// productions emerge;
    /// still, this is production that ultimately must be recognized by a parser
    add_production ( production_id ( "!root-prod!" ) ) ;

}

// ----------------------------------------------------------------------------
grammar::
~grammar() {
//     cout << "~grammar" << endl;
}

// ----------------------------------------------------------------------------
const grammar_id &
grammar::
get_id ( )  const {
    return m_id;
}



    


// ----------------------------------------------------------------------------
void
grammar::
add_type_recognizer ( memory::ptr<const type_recognizer> p_recognizer ) {

    memory::ptr<const type_recognizer> l_reco =
        m_type_recognizers->find ( p_recognizer->get_id ( ) ) ;


    if ( !l_reco )  {
        m_type_recognizers->add ( p_recognizer ) ;
    }
    
}

// ----------------------------------------------------------------------------
void
grammar::
add_token ( const string & p_values,
            const symbol::type & p_type )
    m_tokens->add ( p_values, p_type ) ;
    
}

    

// ----------------------------------------------------------------------------
void
grammar::
add_production  ( const production_id & p_prod_id )  {

    memory::ptr<production> l_prod = find_production ( p_prod_id ) ;

    if  ( !l_prod ) {

        l_prod = memory::ptr<production> ( new production ( p_prod_id ) );
    
        m_productions.push_back ( l_prod ) ;

        if  ( m_productions.size ( ) == 2 )  {
            /// @p prod is the second production in the @p grammar;
            /// so it is the production that the only item of the 
            /// initial (root) production production will reference
        
            memory::ptr<production> l_initial_production = get_first_production ( );
        
            memory::ptr<production::item> l_item = 
                l_initial_production->add ( item_id ( 0 ) ) ;

            l_item->set_production ( l_prod ) ;
        }
    }
    
}

// ----------------------------------------------------------------------------
void
grammar::
add_routine ( memory::ptr<const routine> p_rout )  {

    memory::ptr<const routine> l_rout = find_routine ( p_rout->get_id ( ) ) ;

    if  ( !l_rout ) {
        m_routines.add ( p_rout ) ;
    }
}

// ----------------------------------------------------------------------------
void
grammar::
add_item( const production_id & p_prod_id,
          const item_id & p_item_id ) {

    memory::ptr<production> l_prod = find_production ( p_prod_id ) ;

    if  ( l_prod ) {
        memory::ptr<production::item> l_item = l_prod->find ( p_item_id ) ;

        if  ( !l_item )  {
            l_prod->add ( p_item_id ) ;
        }
    }
}


// ----------------------------------------------------------------------------
void
grammar::
set_production ( const production_id & p_prod_id_owner,
		 const item_id & p_item_id,
		 const production_id & prod_id ) {

    memory::ptr<production> l_prod_owner = find_production ( p_prod_id_owner ) ;
    if  ( l_prod_owner ) {
        memory::ptr<const production> l_prod = find_production ( prod_id ) ;
        if  ( l_prod ) {
            memory::ptr<production::item> l_item = l_prod_owner->find ( p_item_id ) ;
            if  ( l_item ) {
  
                if (  ( !l_item->points_to_type ( ) ) && 
                      ( !l_item->points_to_value ( ) ) &&
                      ( !l_item->points_to_production ( ) ) ) {
                        
                        l_item->set_production ( l_prod ) ;
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
void
grammar::
set_type ( const production_id & p_prod_id,
           const item_id & p_item_id,
           const symbol::type & p_symbol_type ) {

    memory::ptr<production> l_prod = find_production ( p_prod_id ) ;
    if  ( l_prod ) {
        memory::ptr<const symbol::type> l_symbol_type = find_type ( p_symbol_type ) ;

        if  ( l_symbol_type ) {

            memory::ptr<production::item> l_item = l_prod->find ( p_item_id ) ;
            if  ( l_item ) {

                if  (  ( !l_item->points_to_type ( ) ) &&
                       ( !l_item->points_to_value ( ) ) &&
                       ( !l_item->points_to_production ( ) ) ) {

                           l_item->set_symbol_type ( l_symbol_type ) ;
                }
            }
        }
    }
}


// ----------------------------------------------------------------------------
void
grammar::
set_value ( const production_id & p_prod_id,
            const item_id & p_item_id,
            const symbol::value & p_symbol_value )  {

    memory::ptr<production> l_prod = find_production ( p_prod_id ) ;
    if  ( l_prod ) {

        memory::ptr<const symbol::value> l_symbol_value =
            find_value ( p_symbol_value ) ;
        
        if  ( l_symbol_value ) {

            memory::ptr<production::item> l_item = l_prod->find ( p_item_id ) ;
            if  ( l_item ) {

                if  ( ( !l_item->points_to_type ( ) ) && 
                      ( !l_item->points_to_value ( ) ) &&
                      ( !l_item->points_to_production ( ) ) )  {
                    l_item->set_symbol_value ( l_symbol_value ) ;
                }
            }
        }
    }
    
}


// ----------------------------------------------------------------------------
void
grammar::
add_production_routine ( memory::ptr<const production_routine> p_routine ) {
    

    for ( productions::iterator l_prod_ite = m_productions.begin ( ) ;
          l_prod_ite != m_productions.end ( ) ;
          ++l_prod_ite ) {

        ( *l_prod_ite )->add_production_routine ( p_routine ) ;
    }
}



// ----------------------------------------------------------------------------
void
grammar::
add_type_routine ( memory::ptr<const symbol_type_routine> p_routine )  {
    
    for ( productions::iterator l_prod_ite = m_productions.begin ( ) ;
          l_prod_ite != m_productions.end ( ) ;
          ++l_prod_ite ) {

        ( *l_prod_ite )->add_symbol_type_routine ( p_routine ) ;
    }
}


            

// ----------------------------------------------------------------------------
void
grammar::
add_value_routine ( memory::ptr<const symbol_value_routine> p_routine )  {
    
    for ( productions::iterator l_prod_ite = m_productions.begin ( ) ;
          l_prod_ite != m_productions.end ( ) ;
          ++l_prod_ite ) {

        ( *l_prod_ite )->add_symbol_value_routine ( p_routine ) ;
    }
}





// ----------------------------------------------------------------------------
void
grammar::
set_suc ( const production_id & p_prod_id,
          const item_id & p_item_id,
          const item_id & p_suc_id )   {

    memory::ptr<production> l_prod = find_production ( p_prod_id ) ;
    if  ( l_prod ) {

        memory::ptr<production::item> l_item = l_prod->find ( p_item_id ) ;
        if  ( l_item ) {

            memory::ptr<const production::item> l_suc =
                l_prod->find ( p_suc_id ) ;
            if  ( l_suc )  {
                if  ( l_item->get_suc( )->is_last ( ) ) {
                    l_item->set_suc ( l_suc ) ;
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
void
grammar::
set_alt ( const production_id & p_prod_id,
          const item_id & p_item_id,
          const item_id & p_alt_id ) {

    memory::ptr<production> l_prod = find_production ( p_prod_id ) ;
    if  ( l_prod ) {

        memory::ptr<production::item> l_item = l_prod->find ( p_item_id ) ;
        if  ( l_item ) {

            memory::ptr<const production::item> l_alt = l_prod->find ( p_alt_id ) ;
            if  ( l_alt ) {

                if  ( !l_item->get_alt ( ) ) {
                    l_item->set_alt ( l_alt ) ;
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
void
grammar::
accepts_anything ( const production_id & p_prod_id,
                   const item_id & p_item_id )  {

    memory::ptr<production> l_prod = find_production ( p_prod_id ) ;
    if  ( l_prod ) {

        memory::ptr<production::item> l_item = l_prod->find ( p_item_id ) ;
        if  ( l_item ) {

            if  ( !l_item->get_alt ( ) ) {
  
                l_item->accepts_anything ( ) ;
            }
        }
    }
}


// ----------------------------------------------------------------------------
memory::ptr<const type_recognizers>
grammar::
get_type_recognizers ( ) const {return m_type_recognizers;}

// ----------------------------------------------------------------------------
memory::ptr<const tokens>
grammar::
get_tokens ( ) const {return m_tokens;}


// ----------------------------------------------------------------------------
static const number::i16t mark_w = 2;
static const number::i16t owner_w = 25;
static const number::i16t item_w  = 7;
static const number::i16t type_w = 25;
static const number::i16t value_w = 25;
static const number::i16t prod_w = owner_w;
static const number::i16t routine_w = 15;
static const number::i16t suc_w = item_w;
static const number::i16t alt_w = item_w;


// ----------------------------------------------------------------------------
void
print_line ( ostringstream & s,
	     const production_id &o_id,
             const item_id       &i_id,
             const symbol::type   &t_id,
             const symbol::value  &v_id,
             const production_id &p_id,
             const routine_id    &r_id,
             const item_id       &s_id,
             const item_id       &a_id,
             const char mark )  {


    s.width ( mark_w ) ; s.fill ( ' ' ) ; s << left << mark;
    s.width ( owner_w ) ; s.fill ( ' ' ) ; s << left << o_id;
    s.width ( item_w ) ; s.fill ( ' ' ) ; s << left << i_id;
    s.width ( type_w ) ; s.fill ( ' ' ) ; s << left << t_id;
    s.width ( value_w ) ; s.fill ( ' ' ) ; s << left << v_id;
    s.width ( prod_w ) ; s.fill ( ' ' ) ; s << left << p_id;
    s.width ( routine_w ) ; s.fill ( ' ' ) ; s << left << r_id;
    s.width ( suc_w ) ; s.fill ( ' ' ) ; s << right << s_id;
    s.width ( alt_w ) ; s.fill ( ' ' ) ; s << right << a_id;

}

// ----------------------------------------------------------------------------
void
print_header ( ostringstream &s, 
               const string &o_id,
               const string &i_id,
               const string &t_id,
               const string &v_id,
               const string &p_id,
               const string &r_id,
               const string &s_id,
               const string &a_id )  {

    s.width ( mark_w ) ; s.fill ( ' ' ) ; s << left << ' ';
    s.width ( owner_w ) ; s.fill ( ' ' ) ; s << left << o_id;
    s.width ( item_w ) ; s.fill ( ' ' ) ; s << left << i_id;
    s.width ( type_w ) ; s.fill ( ' ' ) ; s << left << t_id;
    s.width ( value_w ) ; s.fill ( ' ' ) ; s << left << v_id;
    s.width ( prod_w ) ; s.fill ( ' ' ) ; s << left << p_id;
    s.width ( routine_w ) ; s.fill ( ' ' ) ; s << left << r_id;
    s.width ( suc_w ) ; s.fill ( ' ' ) ; s << right << s_id;
    s.width ( alt_w ) ; s.fill ( ' ' ) ; s << right << a_id;
    s << "\n";
}

// ----------------------------------------------------------------------------
void
grammar::
report ( ostream & o )  const {

    ostringstream s;

    print_header ( s, "owner", "item", "type", "value",
                   "prod", "routine", "suc", "alt" ) ;

    for  ( productions::const_iterator p = m_productions.begin ( ) ; 
           p != m_productions.end ( ) ;
           ++p ) {
        ( *p )->report ( s ) ;
    }

    o << s.str( ) ;
    
}



// ----------------------------------------------------------------------------
inline bool
grammar::
operator ==  ( const grammar & p_grammar )  const {
    return m_id == p_grammar.m_id;
}

// ----------------------------------------------------------------------------
inline bool
grammar::
operator !=  ( const grammar & p_grammar )  const {
    return m_id != p_grammar.m_id;
}


// ----------------------------------------------------------------------------
memory::ptr<grammar::production> 
grammar::
get_first_production ( )  const {
    return * ( m_productions.begin ( ) );
}

// ----------------------------------------------------------------------------
memory::ptr<const symbol::type>
grammar::
find_type ( const symbol::type & p_symbol_type )  const {

    memory::ptr<const symbol::type> rc ;
    
    if ( m_type_recognizers->find ( p_symbol_type ) != 0 ) {
        rc = memory::ptr<const symbol::type>
            ( new symbol::type ( p_symbol_type) );
    }
    return rc;
}

// ----------------------------------------------------------------------------
memory::ptr<const symbol::value>
grammar::
find_value ( const symbol::value & p_symbol_value )  const {

    memory::ptr<const symbol::value> rc;
    
    if ( m_tokens->is_defined ( p_symbol_value ) != 0 ) {
        rc = memory::ptr<const symbol::value>
            ( new symbol::value ( p_symbol_value ) );
    }
    return rc;
}


// ----------------------------------------------------------------------------
memory::ptr<const type_recognizer>
grammar::
find_type_recognizer ( const recognizer_id & p_reco_id ) const  {
    return m_type_recognizers->find ( p_reco_id ) ;
}


// ----------------------------------------------------------------------------
memory::ptr<const routine>
grammar::
find_routine ( const routine_id & p_rout_id )  const  {
    return m_routines.find ( p_rout_id ) ;
}

// ----------------------------------------------------------------------------
memory::ptr<grammar::production>
grammar::
find_production ( const production_id & p_prod_id )  const {
    memory::ptr<grammar::production> rc;

    for ( productions::const_iterator i_prod = m_productions.begin ( ) ;
          i_prod != m_productions.end ( );
          ++i_prod ) {
        if ( ( *i_prod )->get_id ( ) == p_prod_id ) {
            rc = *i_prod;
            break;
        }
    }

    return rc;
}

// ----------------------------------------------------------------------------
bool
grammar::
already_imported ( memory::ptr<const grammar> p_grammar )  const {
    
    return ( std::find ( m_imported_grammars.begin ( ),
                         m_imported_grammars.end ( ),
                         p_grammar->get_id( ) ) !=
             m_imported_grammars.end ( ) ) ;
}

// ----------------------------------------------------------------------------
bool
grammar::
import_productions ( const grammar & p_grammar ) {
    bool rc = true;

    for ( productions::const_iterator prod_ite =
              p_grammar.m_productions.begin ( ) ;
          prod_ite != p_grammar.m_productions.end ( ) ;
          ++prod_ite ) {

        if ( find_production ( ( *prod_ite )->get_id ( ) ) != 0 ) {
            rc = false;
            break;
        }

        /// the currenty analysed @p production was not found in this @p grammar,
        /// so we add it to the @p productions of this @p grammar
        m_productions.push_back ( *prod_ite ) ;
        
    }
    
    return rc;
}


// ----------------------------------------------------------------------------
void
grammar::
import_type_recognizers  ( const grammar & p_grammar )  {

    for ( type_recognizers::const_iterator type_reco_ite =
              p_grammar.m_type_recognizers->begin ( ) ;
          type_reco_ite != p_grammar.m_type_recognizers->end ( ) ;
          ++type_reco_ite ) {

        if ( find_type_recognizer ( ( *type_reco_ite )->get_id ( ) ) == 0 ) {
            /// no @p type_recognizer in this @p grammar with the same @p
            /// recognizer_id of the currently analysed @p type_recognizer was
            /// found, so we add it to the @p type_recognizers collection of this
            /// @p grammar
            m_type_recognizers->add ( *type_reco_ite );
        }
    }
}


// ----------------------------------------------------------------------------
void
grammar::
import_tokens ( const grammar & p_grammar )  {

    m_tokens->import ( *p_grammar.get_tokens ( ) ) ;
    

}
 

// ----------------------------------------------------------------------------
bool
grammar::
import_routines  ( const grammar & p_grammar )  {
    bool rc = ( check_production_routines ( p_grammar ) &&
                check_value_routines ( p_grammar )      &&
                check_type_routines ( p_grammar )       ) ;

    if ( rc ) {
        import_production_routines ( p_grammar );
        import_value_routines ( p_grammar ) ;
        import_type_routines ( p_grammar ) ;
    }

    return rc;
}


// ----------------------------------------------------------------------------
bool
grammar::
check_production_routines ( const grammar & p_grammar ) {
    bool rc = false;
    return rc;
}



// ----------------------------------------------------------------------------
bool
grammar::
check_value_routines ( const grammar & p_grammar ) {
    bool rc = false;
    return rc;
}



// ----------------------------------------------------------------------------
bool
grammar::
check_type_routines ( const grammar & p_grammar )  {
    bool rc = false;
    return rc;
}



// ----------------------------------------------------------------------------
void
grammar::
import_production_routines ( const grammar & p_grammar ){ }

// ----------------------------------------------------------------------------
void
grammar::
import_value_routines ( const grammar & p_grammar ) { }

// ----------------------------------------------------------------------------
void
grammar::
import_type_routines ( const grammar & p_grammar ) { }






// ############################################################################
//                                  grammar::production


// ----------------------------------------------------------------------------
grammar::production::
~production ( )  {
 //    cout << "~production" << endl;
//     cout << "   " << get_id ( ) << endl;
}

// ----------------------------------------------------------------------------
grammar::production::
production ( const production_id & p_id )  :
    m_id ( p_id )  { }

// ----------------------------------------------------------------------------
const production_id &
grammar::production::
get_id ( )  const {
    return m_id;
}

// ----------------------------------------------------------------------------
bool 
grammar::production::
operator ==  ( const production & p_prod )  const {
    return m_id == p_prod.m_id;
}

// ----------------------------------------------------------------------------
bool
grammar::production::
operator !=  ( const production & p_prod )  const {
    return m_id != p_prod.m_id;
}


// ----------------------------------------------------------------------------
void
grammar::production::
add_production_routine ( memory::ptr<const production_routine> p_routine ) {
    
    for ( production::itens::iterator l_item_ite = m_itens.begin ( ) ;
          l_item_ite != m_itens.end ( );
          ++l_item_ite ) {
        ( *l_item_ite )->set_production_routine ( p_routine ) ;
    }

}

// ----------------------------------------------------------------------------
void
grammar::production::
add_symbol_type_routine ( memory::ptr<const symbol_type_routine> p_routine ) {
    
    for ( production::itens::iterator l_item_ite = m_itens.begin ( ) ;
          l_item_ite != m_itens.end ( );
          ++l_item_ite ) {
        ( *l_item_ite )->set_symbol_type_routine ( p_routine ) ;
    }

}

// ----------------------------------------------------------------------------
void
grammar::production::
add_symbol_value_routine ( memory::ptr<const symbol_value_routine> p_routine ) {
    
    for ( production::itens::iterator l_item_ite = m_itens.begin ( ) ;
          l_item_ite != m_itens.end ( );
          ++l_item_ite ) {
        ( *l_item_ite )->set_symbol_value_routine ( p_routine ) ;
    }

}



// ----------------------------------------------------------------------------
memory::ptr<grammar::production::item>
grammar::production::
find ( const item_id & p_item_id )  const {

    memory::ptr<grammar::production::item> rc;

    if  ( p_item_id == item::accepts_any_symbol_id )  {
        rc = item::accepts_any_symbol;
    }
    else {
        itens::const_iterator i = m_itens.begin ( ) ;
        for  ( ; i != m_itens.end ( ) ; ++i )  {
            if  (  ( *i ) ->get_id ( ) == p_item_id ) 
                break;
        }

        if  ( i != m_itens.end ( ) ) {
            rc = *i;
        }
        
//         itens::const_iterator l_item = std::find ( m_itens.begin ( ) ,
//                                                    m_itens.end ( ) ,
//                                                    p_item_id ) ;
//         if ( l_item != m_itens.end ( ) ) {
//             rc = *l_item;
//         }
        
    } 
    return rc;
}

// ----------------------------------------------------------------------------
memory::ptr<grammar::production::item>
grammar::production::
add ( const item_id & p_item_id )  {

    memory::ptr<grammar::production::item> l_item = this->find ( p_item_id ) ;

    if  ( !l_item )  {
        l_item = memory::ptr<item> ( new item ( p_item_id, *this ) ) ;
        m_itens.push_back ( l_item ) ;


        l_item->set_suc ( item::dummy_item ) ;
    }
    return l_item;
}




// ----------------------------------------------------------------------------
memory::ptr<grammar::production::item>
grammar::production::
get_first ( )  const {
    memory::ptr<grammar::production::item> rc;

    if  ( m_itens.size ( ) ) {
        rc =  ( *m_itens.begin ( ) ) ;
    }
    
    return rc;
}

// ----------------------------------------------------------------------------
void
grammar::production::
report ( ostringstream & s )  const {


    for  ( itens::const_iterator i = m_itens.begin ( ) ;
           i != m_itens.end ( ) ;
           ++i ) {
        if  ( *i ) {
            ( *i )->report ( s ) ;
        }
    }
}




// ############################################################################
//                                  grammar::production::item

const item_id grammar::production::item::dummy_item_id ( -1 ) ;
const item_id grammar::production::item::accepts_any_symbol_id  ( -2 ) ;


const memory::ptr<const grammar::production::item> 
grammar::production::item::dummy_item 
( new grammar::production::item ( dummy_item_id ) ) ;

const memory::ptr<grammar::production::item> 
grammar::production::item::accepts_any_symbol 
( new grammar::production::item ( accepts_any_symbol_id ) ) ;


// ---------------------------------------------------------------------------- 
grammar::production::item::
item ( const item_id & p_id, memory::ptr<const production> p_prod )  :
    m_id ( p_id ) ,
    m_prod_owner ( p_prod ) ,
    m_symbol_type ( 0 ) ,
    m_symbol_value ( 0 ) ,
    m_prod ( 0 ) ,
    m_suc ( 0 ) ,
    m_alt ( 0 ) , 
    m_routine ( 0 ) {

    if  ( m_id != dummy_item_id ) 
        m_suc = dummy_item;
}

// ----------------------------------------------------------------------------
grammar::production::item::
~item ( )  {
//     cout << "~item" << endl;
//     cout << "   " << get_id ( ) << endl;
}


// ----------------------------------------------------------------------------
const item_id &
grammar::production::item::
get_id ( )  const {
    return m_id;
}



// ----------------------------------------------------------------------------
bool
grammar::production::item::
points_to_type ( )  const {
    return  ( m_symbol_type != 0 ) ;
}

// ----------------------------------------------------------------------------
bool
grammar::production::item::
points_to_value ( )  const {
    return  ( m_symbol_value != 0 ) ;
}

// ----------------------------------------------------------------------------
bool
grammar::production::item::
points_to_production ( )  const {
    return  ( m_prod != 0 ) ;
}

// ----------------------------------------------------------------------------
memory::ptr<const routine>
grammar::production::item::
get_routine ( )  const {
    return m_routine;
}

// ----------------------------------------------------------------------------
memory::ptr<const symbol::type> 
grammar::production::item::
get_symbol_type ( )  const {
    return m_symbol_type;
}

// ----------------------------------------------------------------------------
memory::ptr<const symbol::value> 
grammar::production::item::
get_symbol_value ( )  const {
    return m_symbol_value;
}

// ----------------------------------------------------------------------------
memory::ptr<const grammar::production::production>
grammar::production::item::
get_production ( )  const {
    return m_prod;
}

// ----------------------------------------------------------------------------
memory::ptr<const grammar::production::production>
grammar::production::item::
get_production_owner ( )  const {
    return m_prod_owner;
}

// ----------------------------------------------------------------------------
memory::ptr<const grammar::production::item>
grammar::production::item::
get_suc ( )  const {
    return m_suc;
}

// ----------------------------------------------------------------------------
memory::ptr<const grammar::production::item>
grammar::production::item::
get_alt ( )  const {
    return m_alt;
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
accepts_anything ( )  {
    m_alt = accepts_any_symbol;
}

// ----------------------------------------------------------------------------
bool
grammar::production::item::
is_last ( )  const {
    return get_id ( ) == dummy_item_id;
}


// ----------------------------------------------------------------------------
void
grammar::production::item::
set_production_routine ( memory::ptr<const production_routine> p_routine ) {
    
    if ( ( points_to_production ( ) ) &&
         ( get_production ( )->get_id ( ) == p_routine->get_prod_id ( ) ) ) { 
        set_routine ( p_routine ) ;
    }
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
set_symbol_type_routine ( memory::ptr<const symbol_type_routine> p_routine ) {
    
    if ( ( points_to_type ( ) ) &&
         ( *get_symbol_type ( ) == p_routine->get_type ( ) ) ) { 
        set_routine ( p_routine ) ;
    }
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
set_symbol_value_routine ( memory::ptr<const symbol_value_routine> p_routine ) {
    
    if ( ( points_to_value ( ) ) &&
         ( *get_symbol_value ( ) == p_routine->get_value ( ) ) ) { 
        set_routine ( p_routine ) ;
    }
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
set_routine ( memory::ptr<const routine> p_rout )   {
    m_routine = p_rout;
}


// ----------------------------------------------------------------------------
void
grammar::production::item::
set_symbol_type ( memory::ptr<const symbol::type> p_symbol_type )  {
  
    m_symbol_type = p_symbol_type;

    m_prod = 0;

    m_symbol_value = 0;
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
set_symbol_value ( memory::ptr<const symbol::value> p_symbol_value )  {
  
    m_symbol_type = 0;

    m_prod = 0;

    m_symbol_value = p_symbol_value;
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
set_production ( memory::ptr<const production> p_prod )   {
  
    m_prod = p_prod;

    m_symbol_type = 0;

    m_symbol_value = 0;
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
set_suc ( memory::ptr<const grammar::production::item> p_suc )   {
    m_suc = p_suc;
}

// ----------------------------------------------------------------------------
void
grammar::production::item::
set_alt ( memory::ptr<const grammar::production::item> p_alt )   {
    m_alt = p_alt;
}


// ----------------------------------------------------------------------------
bool
grammar::production::item::
accepts_anything ( )  const {
    return  get_id ( ) == accepts_any_symbol_id;
}


// ----------------------------------------------------------------------------
void
grammar::production::item::
report ( ostringstream & s ) const {
    print_line ( s,
                 m_prod_owner ? m_prod_owner->get_id ( ) : production_id ( ) ,
                 m_id,
                 points_to_type ( ) ? *get_symbol_type ( ) : symbol::type ( ) ,
                 points_to_value ( ) ? *get_symbol_value ( ) : symbol::value ( ) ,
                 points_to_production ( ) ? get_production ( )->get_id ( ) :
                 production_id ( ) ,
                 get_routine ( ) ? get_routine ( )->get_id ( ) : routine_id ( ) ,
                 get_suc ( ) ? get_suc ( )->get_id ( ) : item_id ( ) ,
                 get_alt ( ) ? get_alt ( )->get_id ( ) : item_id ( ) ,
                 ' ' );

    s << "\n";
    
}

// ----------------------------------------------------------------------------
bool
grammar::production::item::
operator ==  ( const grammar::production::item & p_item )  const {
    return  ( m_id == p_item.m_id ) ;
}

// ----------------------------------------------------------------------------
bool
grammar::production::item::
operator != ( const grammar::production::item & p_item )  const {
    return  ( m_id != p_item.m_id ) ;
}


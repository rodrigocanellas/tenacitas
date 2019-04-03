#include "routine.h"


using namespace tenacitas::interpreter;

            	  	    


// ############################################################################
//                                  routine

inline routine::
routine( const routine_id & p_id ) :
    m_id( p_id ) {

}

inline routine::
~routine() {
//     cout << "~routine" << endl;
}


inline const routine_id &
routine::
get_id() const {
    return m_id;
}


inline bool
routine::
operator == (const routine &r) {
    return m_id == r.m_id;
}

inline bool
routine::
operator != (const routine &r) {
    return m_id != r.m_id;
}


// ############################################################################
//                                  production_routine

//----------------------------------------------------------------------------
production_routine::
~production_routine ( ) {}

//----------------------------------------------------------------------------
inline const production_id &
production_routine::
get_prod_id ( ) const {return m_production_id;}


//----------------------------------------------------------------------------
production_routine::
production_routine ( const routine_id & p_routine_id,
                     const production_id & p_production_id ) :
    
    routine ( p_routine_id ) ,
    m_production_id ( p_production_id ) {}


// ############################################################################
//                                  symbol_value_routine

//----------------------------------------------------------------------------
symbol_value_routine::
~symbol_value_routine ( ) {}

//----------------------------------------------------------------------------
inline const symbol::value &
symbol_value_routine::
get_value ( ) const {return m_value;}


//----------------------------------------------------------------------------
symbol_value_routine::
symbol_value_routine ( const routine_id & p_routine_id,
                       const symbol::value & p_value ) :
    
    routine ( p_routine_id ) ,
    m_value ( p_value ) {}


// ############################################################################
//                                  symbol_type_routine

//----------------------------------------------------------------------------
symbol_type_routine::
~symbol_type_routine ( ) {}

//----------------------------------------------------------------------------
inline const symbol::type &
symbol_type_routine::
get_type ( ) const {return m_type;}


//----------------------------------------------------------------------------
symbol_type_routine::
symbol_type_routine ( const routine_id & p_routine_id,
                       const symbol::type & p_type ) :
    
    routine ( p_routine_id ) ,
    m_type ( p_type ) {}



// ############################################################################
//                                  routines

//----------------------------------------------------------------------------
routines::
routines ( ) :
    m_list ( ) {}


//----------------------------------------------------------------------------
void
routines::
add ( memory::ptr<const routine> p_routine ) {m_list.push_back ( p_routine ) ;}


//----------------------------------------------------------------------------
routines::const_iterator
routines::
begin ( ) const {return m_list.begin ( ) ;}


//----------------------------------------------------------------------------
routines::const_iterator
routines::
end ( ) const {return m_list.end( ) ;}

//----------------------------------------------------------------------------
memory::ptr<const routine>
routines::
find ( const routine_id & p_rout_id ) const {
    memory::ptr<const routine> rc;
    
    for ( list<memory::ptr<const routine> >::const_iterator i =
              m_list.begin ( ) ;
          i != m_list.end( ) ;
          ++i ) {
        if ( ( *i )->get_id ( ) == p_rout_id ) {
            rc = *i;
            break;
        }
    }
    
    return rc;
}




#ifndef __tenacitas_interpreter__routine__h__
#define __tenacitas_interpreter__routine__h__


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

#include "symbol.h"
#include "fault.h"
#include "identifiers.h"




// =============================================================================
//                                 Namespaces
using namespace std;
using namespace tenacitas;





// =============================================================================
//                                 Macro Contants

/**  @brief */





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



        // =====================================================================
        //                             Typedefs



        // =====================================================================
        //                             Classes      	 


        /** @brief Allows semantic actions to be performed during the
            recognition of a grammar
    
            Routines are function objects that will be executed when a
            tenacitas::interpreter::production::item is recognized.
            When this happens, the tenacitas::interpreter::parser calls the 
            routine associated tenacitas::interpreter::production::item, 
            passing a list with the tenacitas::interpreter::symbol objects
            recognized.
     
            For instance:
            The tenacitas::interpreter::production below: 

            <tt> atrib : var_iden = exp </tt>
     
            meaning: the tenacitas::interpreter::symbol @p var_iden, 
            the @p symbol::value @p =, the @p tenacitas::interpreter::production
            @p exp. 
     
            One can associate a @p routine, say @p r_var, @p r_atrib_sign and 
            @p r_exp, to each tenacitas::interpreter::production::item, via the 
            tenacitas::interpreter::grammar::set_routine method.
     
            So, when @p exp is recognized, @p r_exp will be executed and a list 
            of tenacitas::interpreter::symbol objects, in this case a 
            tenacitas::interpreter::symbol pointing to a @p var_iden, 
            a tenacitas::interpreter::symbol pointing to a @p =, and a 
            tenacitas::interpreter::symbol pointing to the result of the 
            recognition of @p exp, will be passed to the @p r_exp function
            object. 

            @nosubgrouping */

        class routine {

        public:

            // -----------------------------------------------------------------
            //                         routine
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Virtual destructor because we will have sub-classes */
            virtual ~routine();

            /// @}                     ----------
        
            // -----------------------------------------------------------------
            //                         routine
            /// @name                  Public Accessors
            /// @{

            /** @brief Informs the tenacitas::interpreter::routine_id of this @p
                routine */
            const routine_id & get_id() const;
	    	    
            /// @}                     ----------


            // -----------------------------------------------------------------
            //                         routine
            /// @name                  Public Operators
            /// @{

            /** @brief Method that will perform the semantic action
             
                @param [in] p_first is the first recognized 
                tenacitas::interpreter::symbol
            
                @param [in] p_end is the end of the recognizeds @p
                tenacitas::interpreter::symbol; it is not a valid symbol, but a
                "mark" of the end of the collection, just like in the standard
                containers 
             
                @return a (possibly new) tenacitas::interpreter::symbol, result
                of the semantic action */
            virtual memory::ptr<symbol> exec( symbols::iterator p_first,
                                              symbols::iterator p_last )
                const = 0; 

            /** @brief Two @p routine objects are equal if theirs
                tenacitas::interpreter::routine_id are equal */
            bool operator == ( const routine & p_rout );

            /** @brief Two @p routine objects are not equal if theirs
                tenacitas::interpreter::routine_id are different */
            bool operator != ( const routine & p_rout );
	    
            /// @}                     ----------

        protected:

            // -----------------------------------------------------------------
            //                         routine
            /// @name                  Protected Constructors
            /// @{ 

            /** @brief creates a @p routine object, defining its @p
                tenacitas::interpreter::routine_id */
            routine( const routine_id & p_id );

            /// @}                     ----------

        private:

            // -----------------------------------------------------------------
            //                         routine
            /// @name                  Private Attributes
            /// @{
	    
            /** @brief the tenacitas::interpreter::routine_id of the @p
                routine  */
            routine_id m_id;
	    
            /// @}                     ----------
        };



        // =====================================================================
        //                             Typedefs

        /** @brief Functor that defines a default value for a
            tenacitas::interpreter::production_id */
        class production_default {
        public:
            typedef string value_type;
            
            static string value ( ) {
                return "!!no-production-id!!";
            }
        };

        
        /** @brief tenacitas::interpreter::routine to be associated to a
            tenacitas::interpreter::grammar::production::item that references a
            tenacitas::interpreter::grammar::production 

            @nosubgrouping */
        class production_routine : public routine {

        public:

            // -----------------------------------------------------------------
            //                         production_routine
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Destructor */
            virtual ~production_routine ( ) ;

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         production_routine
            /// @name                  Public Accessors
            /// @{

            const production_id & get_prod_id ( ) const;
	    
            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         production_routine
            /// @name                  Public Processors
            /// @{

            /** @copydoc tenacitas::interpreter::routine::exec */
            virtual memory::ptr<symbol> exec( symbols::iterator p_first,
                                              symbols::iterator p_last )
                const = 0; 
	    	    
            /// @}                     ----------

        protected:

            // -----------------------------------------------------------------
            //                         production_routine
            /// @name                  Protected Constructors
            /// @{

            production_routine ( const routine_id & p_routine_id,
                                 const production_id & p_production_id ) ;
            
            /// @}                     ----------

        private:

            // -----------------------------------------------------------------
            //                         production_routine
            /// @name                  Private Attributes
            /// @{

            const production_id m_production_id;
            
            /// @}                     ----------

        };

        /** @brief tenacitas::interpreter::routine to be associated to a
            tenacitas::interpreter::grammar::production::item that references a
            tenacitas::interpreter::grammar::symbol::value

            @nosubgrouping */
        class symbol_value_routine : public routine {

        public:

            // -----------------------------------------------------------------
            //                         symbol_value_routine
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Destructor */
            virtual ~symbol_value_routine ( ) ;

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         symbol_value_routine
            /// @name                  Public Accessors
            /// @{

            const symbol::value & get_value ( ) const;
	    
            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         symbol_value_routine
            /// @name                  Public Processors
            /// @{

            /** @copydoc tenacitas::interpreter::routine::exec */
            virtual memory::ptr<symbol> exec( symbols::iterator p_first,
                                              symbols::iterator p_last )
                const = 0; 
	    	    
            /// @}                     ----------

        protected:

            // -----------------------------------------------------------------
            //                         symbol_value_routine
            /// @name                  Protected Constructors
            /// @{

            symbol_value_routine ( const routine_id & p_routine_id,
                                   const symbol::value & p_value ) ;
            
            /// @}                     ----------

        private:

            // -----------------------------------------------------------------
            //                         symbol_value_routine
            /// @name                  Private Attributes
            /// @{

            const symbol::value m_value;
            
            /// @}                     ----------

        };



        /** @brief tenacitas::interpreter::routine to be associated to a
            tenacitas::interpreter::grammar::production::item that references a
            tenacitas::interpreter::grammar::symbol::type

            @nosubgrouping */
        class symbol_type_routine : public routine {

        public:

            // -----------------------------------------------------------------
            //                         symbol_type_routine
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Destructor */
            virtual ~symbol_type_routine ( ) ;

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         symbol_type_routine
            /// @name                  Public Accessors
            /// @{

            const symbol::type & get_type ( ) const;
	    
            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         symbol_type_routine
            /// @name                  Public Processors
            /// @{

            /** @copydoc tenacitas::interpreter::routine::exec */
            virtual memory::ptr<symbol> exec( symbols::iterator p_first,
                                              symbols::iterator p_last )
                const = 0; 
	    	    
            /// @}                     ----------

        protected:

            // -----------------------------------------------------------------
            //                         symbol_type_routine
            /// @name                  Protected Constructors
            /// @{

            symbol_type_routine ( const routine_id & p_routine_id,
                                   const symbol::type & p_type ) ;
            
            /// @}                     ----------

        private:

            // -----------------------------------------------------------------
            //                         symbol_type_routine
            /// @name                  Private Attributes
            /// @{

            const symbol::type m_type;
            
            /// @}                     ----------

        };
        


        /** @brief Collection of references to tenacitas::interpreter::routine
            objects  

            @nosubgrouping */
        class routines {

        public:

            // -----------------------------------------------------------------
            //                         routines
            /// @name                  Public Internal Types
            /// @{

            typedef list<memory::ptr<const routine> >::const_iterator
            const_iterator; 
            

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         routines
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Constructor */
            routines ( ) ;
            
            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         routines
            /// @name                  Public Accessors
            /// @{

            /** @brief Adds a tenacitas::interpreter::routine to the collection

                @param p_routine is the @p routine to be added to the
                collection   */ 
            void add ( memory::ptr<const routine> p_routine ) ;

            /** @brief References the first <tt>
                tenacitas::memory::ptr<const tenacitas::interpreter::routine
                </tt> of the collection */ 
            const_iterator begin ( ) const ;

            /** @brief The end of the collection */ 
            const_iterator end ( ) const ;

            /** @brief Tries to find a reference to a
                tenacitas::interpreter::routine based on a
                tenacitas::interpreter::routine_id

                @param [in] p_rout_id id the tenacitas::interpreter::routine_id
                used to find a <tt> memory::ptr<const routine> </tt>

                @return a valid <tt> memory::ptr<const routine> </tt>, if found
                @return a null reference, otherwise */ 
            memory::ptr<const routine> find ( const routine_id & p_rout_id )
                const; 
            
            	  	    
            /// @}                     ----------

        private:
            // -----------------------------------------------------------------
            //                         routines
            /// @name                  Private Attributes
            /// @{

            /** @brief The collection of <tt>
                tenacitas::memory::ptr<tenacitas::interpreter::routine </tt> */ 
            list<memory::ptr<const routine> > m_list;
	    
            /// @}                     ----------

        };

    }
}



#endif

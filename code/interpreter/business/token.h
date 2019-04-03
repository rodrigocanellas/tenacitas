
#ifndef __tenacitas_interpreter__token__h__
#define __tenacitas_interpreter__token__h__



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
#include <cassert>


// =============================================================================
//                                 3rds Headers





// =============================================================================
//                                 Our Headers


#include <tenacitas.memory/ptr.h>
#include "symbol.h"


// =============================================================================
//                                 Namespaces
using namespace std;





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

                
        /** @brief This class associates a group of
            tenacitas::interpreter::symbol::value objects to a single
            tenacitas::interpreter::symbol::type

            For example: the tenacitas::interpreter::symbol::value objects encoded
            in the string "== != < > <= >=" are associated to the
            tenacitas::interpreter::symbol::type "relational_operator".

            For example: the tenacitas::interpreter::symbol::value objects encoded
            in the string "const while do" are associated to the
            tenacitas::interpreter::symbol::type "reserverd_word".

            These tenacitas::interpreter::symbol::value shall be recognized
            directly from the input text, i.e., they are defined exclusively by
            their values.

            @nosubgrouping */
        class tokens {

        public:

            // -----------------------------------------------------------------
            //                         tokens
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Destructor */
            ~tokens ( ) ;

            /** @brief Constructor */
            tokens ( ) ;

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         tokens
            /// @name                  Public Accessors
            /// @{

            /** @brief Adds a group of tenacitas::interpreter::symbol::value
                objects that should be recognized directly from the input text,
                and their common tenacitas::interpreter::symbol::type

                @param p_values is a string containg space separated @p
                symbol::value objects, like "= == ;"

                @param [in] p_symbol_type is a @p symbol::type object, defining
                the type of all the @p symbol::value objects coded in @p
                p_values */   
            void add ( const string & p_values,
                       const symbol::type & p_type ) ;

            /** @brief Adds a tenacitas::interpreter::symbol::value object that
                should be recognized directly from the input text, and their
                common tenacitas::interpreter::symbol::type 

                @param p_value is the tenacitas::interpreter::symbol::value that
                will be inserted into the collection

                @param [in] p_symbol_type is a @p symbol::type object, defining
                the type of all the @p symbol::value objects coded in @p
                p_values */   
            void add ( const symbol::value & p_value,
                       const symbol::type & p_type ) ;
            
            
            /** @brief Tries to retrieve a constant reference to a
                tenacitas::interpreter::symbol based on a
                tenacitas::interpreter::symbol::value

                @param [in] p_symbol_value is a
                tenacitas::interpreter::symbol::symbol::value used to find a @p
                symbol 

                @return a valid @p symbol reference, if found
                @return a null reference, otherwise */
            memory::ptr<const symbol>
            is_defined ( const symbol::value & p_symbol_value ) const ;


            /** @brief Copies all the tenacitas::interpreter::symbol::value
                objects that should be recognized directly from the input text,
                and their common tenacitas::interpreter::symbol::type from
                another @p tokens object into this @p tokens object

                @param [in] p_tokens is the @p tokens object from where the the
                tenacitas::interpreter::symbol::value objects, and theirs common
                tenacitas::interpreter::symbol::type will be copied from */ 
            void import ( const tokens & p_tokens ) ;
            
            /// @}                     ----------

        private:

            // -----------------------------------------------------------------
            //                         tokens
            /// @name                  Private Internal Types
            /// @{

            /** @brief Defines a collection of
                tenacitas::interpreter::symbol::value objects and their commom
                tenacitas::interpreter::symbol::type. 

                These values are directly recognized by a
                tenacitas::interpreter::scanner object.

                @nosubgrouping */
            class token {

            public:
                // --------------------------------------------------------------
                //                         token
                /// @name                  Public Internal Types
                /// @{

                /** @brief Allows to interact through the collection of
                    tenacitas::interpreter::symbol::value */ 
                typedef symbols_values::const_iterator const_iterator;

                /// @}                     ----------

                // --------------------------------------------------------------
                //                         token
                /// @name                  Public Constructors
                /// @{

                /** @brief Constructor

                    @param [in] p_values are the recognizeds
                    tenacitas::interpreter::symbol::value, like "= == ;"

                    @param [in] p_symbol_type is the
                    tenacitas::interpreter::symbol::type of the recognizeds
                    tenacitas::interpreter::symbol::value objects */
                token ( const string & p_symbol_values,
                        const symbol::type & p_symbol_type ) ;
            

                /** @brief Destructor */
                ~token ( ) ;

                /// @}                     ----------

                // ---------------------------------------------------------------
                //                         token
                /// @name                  Public Accessors
                /// @{

                /** @brief Returns a constant reference to the first element of
                    the collection */ 
                const_iterator begin ( ) const;

                /** @brief Returns a constant reference to the first element of
                    the collection */ 
                const_iterator end ( ) const;

                /** @brief Tries to find a
                    tenacitas::memory::ptr<const
                    tenacitas::interpreter::symbol::value> based on a
                    tenacitas::interpreter::symbol::value 

                    @param [in] p_symbol_value is a @p symbol::value used in the
                    search

                    @return a valid reference to a @p symbol::value, if found
                    @return a null reference, otherwise */ 
                memory::ptr<const symbol::value>
                is_defined ( const symbol::value & p_symbol_value ) const ;

                /** @brief Retrieves the tenacitas::interpreter::symbol::type of
                    registered the tenacitas::interpreter::symbol::value */ 
                memory::ptr<const symbol::type> get_type ( ) const;
	    
                /// @}                     ----------

            private:

                // --------------------------------------------------------------
                //                         token
                /// @name                  Private Attributes
                /// @{

                /** @brief The list of recognized values */ 
                memory::ptr<symbols_values> m_values;

                /** @brief The type of all the recognized values */ 
                memory::ptr<const symbol::type> m_type;
            
                /// @}                     ----------

            };


            /** @brief Type of the list of @p token objects */ 
            typedef list<memory::ptr<const token> > token_list;


            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         tokens
            /// @name                  Private Attributes
            /// @{

            /** @brief The list of @p token objects */ 
            token_list m_list;
	    
            /// @}                     ----------

        };

    }
}



#endif

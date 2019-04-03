
#ifndef __tenacitas_interpreter__type_recognizer__h__
#define __tenacitas_interpreter__type_recognizer__h__


// =============================================================================
/** @file
    @brief   */


// =============================================================================
/** @todo

 */


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
#include <tenacitas.number/counter.h>
#include <tenacitas.memory/ptr.h>
#include <tenacitas.number/fault.h>

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
        //                             Classes


        /** @brief  Objects of this class will try to match a format pattern to
            the input text, revealing the symbol value.

            For example, a @p number_recognizer class may recognize decimal or
            hexadecimal integers, as well as real numbers, or even complex
            numbers. A @p word_recognizer may recognize only a sequence of
            letters. A @p string_recognizer will try to recognize text between
            double-quotes.  

            @nosubgrouping */
        class type_recognizer  {

        public:
            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Destructor */
            virtual ~type_recognizer ( ) ;

            /// @}                     ----------


            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Protected Accessors
            /// @{

            /** @brief Retrieves the identifier of the recognizer */
            const recognizer_id & get_id ( ) const;
            

            /** @brief Retrieves the list of recognizable object used in the
                recognition process

                A @p recognizer defines, in its constructor, the
                tenacitas::interpreter::symbol::value or
                tenacitas::interpreter::symbol::type objects that it is capable
                of recognize

                This method retrieves this data, as a constant reference to the
                objects

                @return A tenacitas::interpreter::symbols_info constant
                reference, containing constant refereneces to the
                tenacitas::interpreter::symbol::value or
                tenacitas::interpreter::symbol::type objects used by its @p
                recognizer */
            memory::ptr<const symbols_types>
            get_recognizeds ( ) const;


            /** @brief Informs if a type of information,
                tenacitas::interpreter::symbol::value or
                tenacitas::interpreter::symbol::type is 
                recognized by this @p recognizer

                @param [in] p_symbol_info is the @p symbol_info we want to know
                if is recognized by this @p recognizer

                @return @p a valid constant reference to the @p symbol_info, if
                found 
                @return @p a null reference, if not */
            memory::ptr<const symbol::type>
            recognizes ( const symbol::type & p_symbol_type ) const;
            
            /// @}                     ----------


            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Public Processors
            /// @{

            /** @brief Tries to recognise a tenacitas::interpreter::symbol
                object from the input text

                @param [in] p_end marks the end of the input text

                @param [in-out] p_walker is the current position in the input
                text 
            */
            memory::ptr<const symbol>
            recognize ( string::const_iterator p_end,
                        string::const_iterator & p_walker ) const;

            /// @}                     ----------


            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Public Operators
            /// @{

            /** @brief Equal-to operator */
            bool operator == ( const type_recognizer & p_reco ) const;

            /** @brief Not-equal-to operator */
            bool operator != ( const type_recognizer & p_reco ) const;
            
            /// @}                     ----------


            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Public Attributes
            /// @{

            /** @brief Indicates that an empty string defining the @p
                symbol_info objects that this @p recognizer is able to
                recognize, was passed empty to the constructor */
            static const number::fault::code error_empty_info_string;

            /** @brief Indicates that a non registered
                tenacitas::interpreter::symbol::type is being used to define the
                @p symbol::type of a recognized  tenacitas::interpreter::symbol

                The valid @p symbol::type objects must be informed in the
                constructor, via the @p p_types parameter */
            static const number::fault::code error_type_not_registered;
            
            /// @}                     ----------            

        protected:

            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Protected Constructors
            /// @{

            /** @brief Constructor

                @param [in] p_reco_id is the identifier of this recognizer

                @param [in] p_types contains space separated strings, each
                defining a tenacitas::interpreter::symbol::type that this @p
                type_recognizer id capable of recognizing, like
                "decimal_integer real"
            */
            type_recognizer ( const recognizer_id & p_reco_id,
                              const string & p_symbols_types );
            
            /// @}                     ----------


            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Protected Processors
            /// @{

            /** @brief Tries to recognize a format pattern that corresponds to
                one of the tenacitas::interpreter::symbol::type that this @p
                type_recognizer is designed to recognize

                @param [in] p_end is the end of the input text

                @param [in-out] is the current referenced position in the input
                text

                @param [in-out] is a string that defines the
                tenacitas::interpreter::symbol::type of the recognized
                tenacitas::interpreter::symbol. This value <b> must </b> be one
                of the strings informed in the @p p_types parameter in the
                constructor 

                @return @p true if the @p symbol::type that this @p
                type_recognizer is designed to recognize was actually
                recognized

                @return @p false, if not */
            virtual bool try_to_recognize ( string::const_iterator p_end,
                                            string::const_iterator & p_walker,
                                            string & p_symbol_type ) const = 0;  
	    
            /// @}                     ----------
            
        private:

            // -----------------------------------------------------------------
            //                         type_recognizer
            /// @name                  Private Attributes
            /// @{

            /** @brief Identifier of the recognizer */
            const recognizer_id m_reco_id;

            /** @brief The collection of constant references to @p t_symbol_info
                objects that this @p recognizer recognizes */
            memory::ptr<symbols_types> m_symbols_types;
            
            /// @}                     ----------
            
        };





        /** @brief Collection of references to 
            tenacitas::interpreter::type_recognizer 
            @nosubgrouping */
        class type_recognizers {

        public:

            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Public Internal Types
            /// @{

            /** @brief Type of the collection of references to the constant
                references to tenacitas::interpreter::type_recognizer objects */
            typedef 
            list<memory::ptr<const type_recognizer> >::const_iterator
            const_iterator; 

            /// @}                     ----------
            
            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Public Constructors
            /// @{ 

            /** Constructor */
            type_recognizers ( ) ;

            /** @brief Destructor */
            virtual ~type_recognizers ( ) ;

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Public Accessors
            /// @{

            /** @brief Access to the first constant reference to a
                tenacitas::interpreter::type_recognizer */
            const_iterator begin ( ) const;

            /** @brief End of the collection */
            const_iterator end ( ) const;


            /** @brief Tries to find a
                tenacitas::interpreter::type_recognizer based on its
                tenacitas::interpreter::recognizer_id 

                @param [in] p_reco_id is the identifier of the @p
                recognizer we want to find

                @return a valid reference to a
                tenacitas::interpreter::type_recognizer, if found
                @return a null valid if not found */
            memory::ptr<const type_recognizer>
            find ( const recognizer_id & p_reco_id ) const;
            
            /** @brief Informs if a certain @p symbol::type is already being
                recognized by a tenacitas::interpreter::type_recognizer

                @param p_symbol_info is the @p symbol::type that we want to
                verify if is already being defined by a @p type_recognizer

                @return a valid reference to a @p symbol::type, if found
                @return a null reference, otherwise */
            memory::ptr<const symbol::type>
            find ( const symbol::type & p_symbol_type ) const ;  

            /** @brief Inserts a tenacitas::interpreter::type_recognizer
                reference to the collection

                @param p_reco is the @p type_recognizer reference we want to
                insert
            */  
            void add ( memory::ptr<const type_recognizer> p_reco );
            
            	    
            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Public Processors
            /// @{

            /** @brief Asks for each registered
                tenacitas::interpreter::type_recognizer to recognize a
                tenacitas::interpreter::symbol

                @param [in] p_end is the end of the input text

                @param [in-out] p_walker is the current referenced position in
                the input text

                @return a valid reference to @p symbol, if one was recognized
                @return a null  reference, if no @p symbol was recognized */
            memory::ptr<const symbol>
            recognize ( string::const_iterator p_end,
                        string::const_iterator & p_walker ) const;
            
            
            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Public Attributes
            /// @{

            /** @brief Indicates if a tenacitas::interpreter::type_recognizer
                was already registered in the collection */
            static const number::fault::code
            error_recognizer_already_registered; 

            /** @brief Indicates that a tenacitas::interpreter::type_recognizer,
                that is about to be inserted, recognizes a @p symbol::type that a
                previously inserted @p type_recognizer recognizes */
            static const number::fault::code error_info_already_recognized;

            /// @}                     ----------

        protected:

            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Protected Internal Types
            /// @{

            /** @brief Type of the collection of constant references to
                tenacitas::interpreter::type_recognizer objects */
            typedef list<memory::ptr<const type_recognizer> >
            recognizers_list;



            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Protected Accessors
            /// @{


            /// @}                     ----------

        private:

            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Private Processors
            /// @{

            /** @brief Policy that determines if a
                tenacitas::interpreter::type_recognizer can actually be added to
                the collection 

                @param [in] p_reco is the @p type_recognizer candidate to be
                added to the collection
            */
            bool may_add ( memory::ptr<const type_recognizer> p_reco ) const ;
	    
            /// @}                     ----------

            
            // -----------------------------------------------------------------
            //                         type_recognizers
            /// @name                  Private Attributes
            /// @{

            /** @brief The collection of constant references to
                tenacitas::interpreter::recognizer objects */
            recognizers_list m_type_recognizers;
            
            /// @}                     ----------

        };

    }
}



#endif

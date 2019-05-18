
#ifndef __tenacitas_interpreter__recognizers__h__
#define __tenacitas_interpreter__recognizers__h__


// =============================================================================
/** @file Avaiable tenacitas::interpreters::recognizers
    @brief   */

// =============================================================================
//                                 C++ Headers

#include <cctype>



// =============================================================================
//                                 3rds Headers

#include <string>



// =============================================================================
//                                 Our Headers
#include "type_recognizer.h"
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






/** @brief namespace of the organization */
namespace tenacitas {
    namespace interpreter {

        // =====================================================================
        //                             Pre-Declarations





        // =====================================================================
        //                             Typedefs

        /** @brief */





        // =====================================================================
        //                             Attributes

        /** @brief    */





        // =====================================================================
        //                             Functions

        /** @brief */





        // =====================================================================
        //                             Classes



        /** @brief @p word_recognizer is a
            tenacitas::interpreter::type_recognizer that recognizes 'word'
            symbols, which are conform to the format:   

            word: letter |
                  letter word

            letter: a | b ... | z | A | B ... | Z
            @nosubgrouping */

        class word_recognizer : public type_recognizer {
        public:
            // -----------------------------------------------------------------
            //                         word_recognizer
            /// @name                  Public Constructors
            /// @{

            word_recognizer ( );

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         number_recognizer
            /// @name                  Public Attributes
            /// @{

            static const symbol::type word;

            /// @}                     ----------


        protected:

            // -----------------------------------------------------------------
            //                         word_recognizer
            /// @name                  Protectec Processors
            /// @{

            /** @copydoc
                tenacitas::interpreter::type_recognizer::try_to_recognize */
            bool try_to_recognize ( string::const_iterator p_end,
                                    string::const_iterator & p_walker,
                                    string & p_str_symbol_type ) const ;

            /// @}                     ----------
            
        };



        /** @brief number_recognizer is a
            tenacitas::interpreter::type_recognizer that recognizes 'real' and
            'decimal_integer' symbols, which conform to the following format: 

            
            real: digit-sequence  decimal-separator digit-sequence
                
            decimal_integer: digit-sequence

            digit-sequence: digit  |
                            digit digit-sequence

            digit: '0' | '1'| '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

            @note @p decimal-separator is defined in the constructor

            @nosubgrouping */

        class number_recognizer : public type_recognizer {
        public:
            // -----------------------------------------------------------------
            //                         number_recognizer
            /// @name                  Public Constructors
            /// @{

            /** Constructor

                @param [in] p_decimal_separator defines the decimal separator */
            number_recognizer ( char p_decimal_separator = '.' );

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         number_recognizer
            /// @name                  Public Attributes
            /// @{

            static const symbol::type real;
            static const symbol::type decimal_integer;

            /// @}                     ----------
            

        protected:
            // -----------------------------------------------------------------
            //                         number_recognizer
            /// @name                  Protected Processors
            /// @{

            /** @copydoc
                tenacitas::interpreter::type_recognizer::try_to_recognize */
            bool try_to_recognize ( string::const_iterator p_end,
                                    string::const_iterator & p_walker,
                                    string & p_str_symbol_type) const;

            /// @}                     ----------

        private:
            // -----------------------------------------------------------------
            //                         number_recognizer
            /// @name                  Private Attributes
            /// @{

            /** @brief The decimal separator */
            char m_dec_sep;

            /// @}                     ----------
        };

        

    }
}



#endif

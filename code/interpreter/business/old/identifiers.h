#ifndef __tenacitas_interpreter__identifiers__h__
#define __tenacitas_interpreter__identifiers__h__


// =============================================================================
/**
   @file 
   File description
*/


// =============================================================================
//                                 C++ Headers
#include <string> 
#include <tenacitas.number/ints.h>

// =============================================================================
//                                 3rds Headers


// =============================================================================
//                                 Our Headers


// =============================================================================
//                                 Namespaces
using namespace std;


// =============================================================================
//                                 Macro Contants

/**
 */


// =============================================================================
//                                 Macro Commands

/**
 */


/** @brief Namespace of the organization */
namespace tenacitas {
    /** @brief Identifiers to allow the creation of a interpreter */
    namespace interpreter {


        // =====================================================================
        //                             Typedefs

        /**
         */

        /** @brief Rou;tine identifier */
        typedef string routine_id;        

        /** @brief Production identifier */
        typedef string production_id;        

        /** @brief Grammar identifier */
        typedef string grammar_id;

        /** @brief Item identifier */
        typedef number::i16t item_id;

        /** @brief Recognizer identifier */
        typedef string recognizer_id;        



        // =====================================================================
        //                             Pre-Declarations


        // =====================================================================
        //                             Attributes

        /**
         */


        // =====================================================================
        //                             Functions

        /**
         */


        // =====================================================================
        //                             Classes


    }
}



#endif

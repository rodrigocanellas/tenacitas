
#ifndef __tenacitas_interpreter__fault__h__
#define __tenacitas_interpreter__fault__h__


// =============================================================================
/** @file
    @brief   */



// =============================================================================
//                                 C++ Headers

#include <string>
#include <sstream>




// =============================================================================
//                                 3rds Headers





// =============================================================================
//                                 Our Headers

#include <tenacitas.number/ints.h>
#include <tenacitas.number/counter.h>
#include <tenacitas.number/fault.h>





// =============================================================================
//                                 Namespaces
using namespace std;





// =============================================================================
//                                 Macro Contants

/**  @brief */





// =============================================================================
//                                 Macro Commands

/** @brief Macro command to create a @p intepreter::fault object, allowing
    precise definition of file, function, code line, input text line and column
    where the error occurred 
    
    @param [in] p_class is the class that generated the fault
    @param [in] p_code is the code identifying the fault
    @param [in] p_input_line is the line in the input text where the error
    occured 
    @param [in] p_input_column is the line in the input text where the error
    occured 

    
    Please, see the class tenacitas::interpreter::fault below for more details
*/  
#define create_interpreter_fault(p_class, p_code,                       \
                                 p_input_line, p_input_column)          \
    tenacitas::interpreter::fault (#p_class, p_code, __FUNCTION__,      \
                                   __FILE__, __LINE__, p_input_line,    \
                                   p_input_column)








/** @brief Namespace of the organization */
namespace tenacitas {
    /** @brief Identifiers to allow the creation of a interpreter */
    namespace interpreter {

        // =====================================================================
        //                             Typedefs

        /** @brief A line in the text being analysed   */
        typedef number::positive_counter<number::u32t> line_number;

        /** @brief A column in a line in the text being analysed   */
        typedef number::u16t column_number;



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

        /** @brief Fault for the tenacitas::interpreter namespace 

            @nosubgrouping */
        class fault : public number::fault {

        public:
            // -----------------------------------------------------------------
            //                         fault
            /// @name                  Public Constructors
            /// @{ 

            /** @brief Creates a fault object
        
                Defines the class name, method, file and line where the
                fault was produced, and the fault code.
	 
                @note Please, prefer to use the @p create_fault macro
                to create a @p fault object
	 
                @param [in] p_class_name is the name of the class where the
                fault was generated
	 
                @param [in] p_code is the identifier of the fault generated
	 
                @param [in] p_method_name is the  name of the method where the
                fault was generated
	 
                @param [in] p_file_name is the name of the file where the
                fault was generated
	 
                @param [in] p_line_number is the line number where the fault
                was generated

                @param [in] p_input_text_line is the line in the input text
                where the fault occurred

                @param [in] p_input_text_column is the collumn in the @p
                p_input_text_line in the input text where the fault occurred */
            fault ( const string & p_class_name,
                    const fault::code & p_code,
                    const string & p_method_name,
                    const string & p_file_name,
                    const number::u32t & p_line_number,
                    const line_number & p_input_text_line,
                    const column_number &  p_input_text_column );

            fault ( const number::fault & e,
                    const line_number & p_input_text_line ,
                    const column_number &  p_input_text_column ) ; 
            
            /** @brief Destructor */
            virtual ~fault ( ) throw ( ) ;

            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         fault
            /// @name                  Public Accessors
            /// @{
	    
            /** @brief Retrieves the line number where the fault occurred */
            line_number get_line ( ) const ;

            /** @brief Retrieves the column number where the fault occurred */
            column_number get_column ( ) const ;

            /** @brief As <tt> std::exception </tt> requires */
            const char* what() const throw();
            
	    
            /// @}                     ----------

            // -----------------------------------------------------------------
            //                         fault
            /// @name                  Private Attributes
            /// @{
	    
            /** @brief Line number where the fault occurred */
            line_number m_input_text_line;

            /** @brief Column number where the fault occurred */
            column_number m_input_text_column;
	    
            /// @}                     ----------

        };


    }
}



#endif

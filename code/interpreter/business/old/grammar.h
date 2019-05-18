
#ifndef __tenacitas_interpreter__bus__grammar__h__
#define __tenacitas_interpreter__bus__grammar__h__


// =============================================================================
/** @file
 @brief */

/**
 @todo stop using tenacitas::identifier
 @todo stop using exceptions
 @todo do not use sub-classes for @p routine
 @todo simplify @p grammar interface, leting only methods used by the
 programmaer to create the grammar, eliminating methods used by the @p parser
 @todo step-by-step documentation/examples on how to create a new grammar,
 specialy on how to create @p type_recognizer classes
 */

// =============================================================================
// C++ Headers

#include <string>
#include <list>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <memory>


// =============================================================================
// 3rds Headers

// =============================================================================
// Our Headers
#include "type_recognizer.h"
#include "symbol.h"
#include "routine.h"
#include "token.h"
#include "identifiers.h"

// =============================================================================
// Namespaces
using namespace std;
using namespace tenacitas;

// =============================================================================
// Macro Contants

// =============================================================================
// Macro Commands


/** @brief namespace of the organization */
namespace tenacitas {
namespace interpreter {
namespace bus {

// =====================================================================
// Typedefs

// =====================================================================
// Pre-Declarations

// =====================================================================
// Attributes

// =====================================================================
// Functions

// =====================================================================
// Classes


/** @brief The @p grammar class represents a computer programming
 grammar, usually to define a programming language. However, it can
 be used to define any formal combination of symbols to be used in
 many situatons, i. e., any syntax.

 Grammars are usually defined in Backus–Naur form (BNF). You can read
 more about BNF in http: //en.wikipedia.org/wiki/Backus-Naur_form,
 among many other, and as an example, you can read an the BNF for the
 Pascal language in
 http: //www.cs.stevens.edu/~badri/cs616/pas_bnf.html.

 This implementation is quite different from the usual and more
 contemporary ones. It stores all the grammar in memory, and the
 comparison of an input text to the grammar, to check if the text is
 correct according to the grammar, normally called "parsing the
 text" or "syntax analisys", is done by class
 tenacitas::interpreter::parser.

 A @p grammar is defined by a collection of
 tenacitas::interpreter::production objects, each containing a
 collection of tenacitas::interpreter::item objects.

 A tenacitas::interpreter::item can have not only a successor
 tenacitas::interpreter::item, to link the chain of
 tenacitas::interpreter::item objects of the
 tenacitas::interpreter::production, but it can also have an
 alternative tenacitas::interpreter::item. This alternative @p
 item is used by the parser when trying to validate the input text
 against the grammar.

 Each tenacitas::interpreter::item object can reference one of
 these:

 - a tenacitas::interpreter::production, which means that the
 tenacitas::interpreter::item references a complex combination of
 tenacitas::interpreter::symbol, such as, in Pascal:
 <tt>
 unary-expression: unary-op unary-expression
 | primary-expression
 </tt>

 The <tt> unary-expression </tt> @p production has an @p item, say 0,
 which references the <tt> unary-op </tt> @p production; item 0 is
 followed by another @p item, say 1, which references the <tt>
 unary-expression </tt> @p production; @p item 0 has an alternative
 item, say 2, which references <tt> primary-expression </tt>
 @p production.

 When a tenacitas::interpreter::item references a
 tenacitas::interpreter::production, the
 tenacitas::interpreter::parser object recursively starts to analyse
 this production.

 - a tenacitas::interpreter::symbol::type, which means that it is
 possible to recognize a tenacitas::interpreter::symbol directly from
 the input text, such as, in Pascal:
 <tt>
 primary-expression: unsigned-integer
 word
 </tt>

 The @p production <tt> primary-expression </tt> has an @p item, say
 0, which references the @p type <tt> unsigned-integer </tt>, which
 is a combination of digits from 0 to 9, and no '-' sign. @p item 0
 has an alternative @p item, say 1, which
 references the @p symbol::type @p word, which is a combination of
 letters. Both <tt> unsigned-integer </tt> and @p word are defined by
 a format which can be recognized directly from the input text.

 When a tenacitas::interpreter::item references a
 tenacitas::interpreter::symbol::type, it is necessary to define a
 tenacitas::interpreter::type_recognizer aware of recognizing a
 tenacitas::interpreter::symbol::type from the input text.

 - a tenacitas::interpreter::symbol::value, a defined combination of
 characters, like '==' or 'const'.

 A tenacitas::interpreter::item object can also reference a
 tenacitas::interpreter::routine object, which is a function that the
 tenacitas::interpreter::parser object will execute when the
 tenacitas::interpreter::item is recognized.

 When one associates tenacitas::interpreter::routine objects to
 items, the tenacitas::interpreter::parser becomes an interpreter.

 @p grammar class is designed and implemented so that
 tenacitas::interpreter::routine objects can be associated to
 tenacitas::interpreter::item after the definition of the grammar.

 This means that one can implement a @ grammar for an @p expression,
 and have, for different situations, different
 tenacitas::interpreter::routine objects associated.

 So, if in one situation all we want is to print the @p expression,
 we can write tenacitas::interpreter::routine objects to simply
 print the elements of the expression.

 However, if we want to calculate the expression, we can write @p
 tenacitas::interpreter::routine to calculate the result of the
 expression.

 Finally, we can build new tenacitas::interpreter::grammar based on
 existing ones. Suppose we create a
 tenacitas::interpreter::expression class, sub-class of @p
 grammar. We can later create a tenacitas::interpreter::pascal, also
 sub-class of @p grammar, that incorporates the
 tenacitas::interpreter::expression.

 This way, we can create big grammars based on smaller, separetly
 tested ones.

 This design is largely inspired by the book "A construção de um
 Compilador", by Valdemar W. Setzer and Inês S. Homem de Melo,
 Editora Campus, ISBN 85-7001-130-X. Sorry, but this book was not
 translated to English.

 @nosubgrouping */
class grammar {

public:

    /// Public Internal Types

    /// full declaration and definition below
    class production;

    /// Public Constructors

    /** @brief For sure sub-classes will be created */
    virtual ~grammar();

    /// Public Accessors

    /** @brief Retrieves the identifier of the grammar */
    const grammar_id & get_id () const;

    /** @brief Retrieves the collection of
     * tenacitas::interpreter::type_recognizer objects */
    std::shared_ptr<const type_recognizers> get_type_recognizers () const;

    /** @brief Retrieves the collection of
     * tenacitas::interpreter::tokens objects */
    std::shared_ptr<const tokens> get_tokens() const;

    /** @brief Retrieves the first production of the @p grammar */
    std::shared_ptr<production> get_first_production () const;

    /// Public Helpers

    void report (std::ostream & o) const;

    /// Public Operators

    /** @brief A grammar is equal to another if theirs @p grammar_id
     * objects are equals */
    bool operator == (const grammar & p_grammar) const;

    /** @brief A grammar is equal to another if theirs @p grammar_id
     * objects are different */
    bool operator != (const grammar & p_grammar) const;

    /// Public Attributes

    static const uint16_t error_production_colision;

    static const uint16_t error_routine_colision;

    static const uint16_t error_production_exists;

    static const uint16_t error_routine_exists;

    static const uint16_t error_item_exists;

    static const uint16_t error_non_existing_production;

    static const uint16_t error_non_existing_item;

    static const uint16_t error_item_refs_type;

    static const uint16_t error_item_refs_value;

    static const uint16_t error_non_existing_recognizer;

    static const uint16_t error_item_refs_production;

    static const uint16_t error_item_has_successor ;

    static const uint16_t error_item_has_alternative;

    static const uint16_t error_recognizer_exists;

protected:

    /// Protected Internal Types

    /** @brief The productions of the grammar type
     * That is ok to be a simple <tt> std::list </tt> because all the
     * manipulation of the collection will be internal, i.e., the programmer
     * user of the @p interpreter project will not use it */
    typedef list<std::shared_ptr<production> > productions;


    /// Protected Constructors

    /** @brief A @p grammar has an identifier, useful when we
 incorporate an existing @p grammar into a new one

 @param [in] p_id is the tenacitas::interpreter::grammar_id */
    grammar(const grammar_id & p_id) ;

    /// Protected Accessors

    /** @brief Incorporates an existing grammar into this grammar

 @param [in] p_grammar is an existing grammar
 */
    template <typename t_grammar>
    void add_grammar() ;


    /** @brief Adds a tenacitas::interpreter::type_recognizer to
 recognize symbol types found in the input text

 @param [in] p_recognizer is new @p type_recognizer for the @p
 grammar
 */
    void add_type_recognizer (std::shared_ptr<const type_recognizer>
                              p_recognizer) ;


    /** @brief Adds a group of values to be recognized

 This method will create a
 tenacitas::interpreter::value_recognizer capable of
 recognizing the strings coded in @p p_values

 @param [in] p_values are the
 tenacitas::interpreter::symbol::value that this recognizer shall
 recognize from the input text, like like "= == ;"

 @param [in] p_symbol_type is the
 tenacitas::interpreter::symbol::type of the
 tenacitas::interpreter::symbol::value that this recognizer
 recognizes
 */
    void add_token (const string & p_values,
                    const symbol::type & p_type);


    /** @brief Adds a @p tenacitas::interpreter::production to the @p
 grammar.

 @param [in] p_prod_id is the identifier of the new
 tenacitas::interpreter::production
 */
    void add_production(const production_id & p_prod_id);


    /** @brief Adds a tenacitas::interpreter::routine to the @p grammar

 @param [in] p_rout is a new @p routine that will be executed
 whena @p item is recognized */
    void add_routine(std::shared_ptr<const routine> p_rout);


    /** Adds a tenacitas::interpreter::item to a previously added
 tenacitas::interpreter::production

 @param [in] p_prod_id is the identifier of the previously
 added @p production which will own the @p p_item_id

 @param [in] p_item_id is the identifier of the new @p item
 */
    void add_item(const production_id & p_prod_id,
                  const item_id & p_item_id);


    /** @brief Defines that a tenacitas::interpreter::item in a
 tenacitas::interpreter::production will reference another
 tenacitas::interpreter::production

 @param [in] p_prod_id_owner is the identifier of the
 @p production which holds the @p item which identifier is @p
 p_item_id

 @param [in] p_item_id is the identifier of the @p item that will
 reference the @p production which identifier is @p p_prod_id

 @param [in] p_prod_id is the identifier of the @p production
 that will be referenced by @p item @p p_item_id, which is held
 by the @p production @p p_prod_id_owner
 */
    void set_production(const production_id & p_prod_id_owner,
                        const item_id & p_item_id,
                        const production_id & p_prod_id) ;


    /** Defines that a tenacitas::interpreter::item in a
 tenacitas::interpreter::production will reference a
 tenacitas::interpreter::symbol::type

 @param [in] p_prod_id is the identifier of the @p production
 which holds the @p item which identifier is @p p_item_id

 @param [in] p_item_id is the identifier of the @p item that will
 reference the @p type which identifier is @p p_symbol_type

 @param [in] p_symbol_type is the identifier of the @p
 symbol::type that will be referenced by the @p item @p p_item,
 which is held by the @p production @p p_prod_id */
    void set_type(const production_id & p_prod_id,
                  const item_id & p_item_id,
                  const symbol::type & p_symbol_type);


    /** Defines that a tenacitas::interpreter::item in a
 tenacitas::interpreter::production will reference a
 tenacitas::interpreter::symbol::value

 @param [in] p_prod_id is the identifier of the @p production
 which holds the @p item which identifier is @p p_item_id

 @param [in] p_item_id is the identifier of the @p item that will
 reference the @p type which identifier is @p p_symbol_type

 @param [in] p_symbol_value is the identifier of the @p
 symbol::value that will be referenced by the @p item @p p_item,
 which is held by the @p production @p p_prod_id
 */
    void set_value(const production_id & p_prod_id,
                   const item_id & p_item_id,
                   const symbol::value & p_symbol_value);


    /** @brief Makes all the
 tenacitas::interpreter::grammar::production::item that
 references a tenacitas::interpreter::grammar::production which
 <tt> get_id () == p_routine->get_prod_id () </tt> to reference
 @p p_routine */
    void add_production_routine (std::shared_ptr<const production_routine>
                                 p_routine) ;


    /** @brief Makes all the
 tenacitas::interpreter::grammar::production::item that
 references a tenacitas::interpreter::grammar::symbol::type which
 <tt> get_symbol_type () == p_routine->get_type () </tt> to
 reference @p p_routine */
    void add_type_routine (std::shared_ptr<const symbol_type_routine>
                           p_routine) ;


    /** @brief Makes all the
 tenacitas::interpreter::grammar::production::item that
 references a tenacitas::interpreter::grammar::symbol::symbol
 which <tt> get_symbol_value () == p_routine->get_value () </tt>
 to reference @p p_routine */
    void add_value_routine (std::shared_ptr<const symbol_value_routine>
                            p_routine);


    /** @brief Defines the successor tenacitas::interpreter::item
 to a tenacitas::interpreter::item

 @param [in] p_prod_id is the identifier of the @p production
 which holds the @p item which identifier is @p p_item_id

 @param [in] p_item_id is the identifier of the @p item that will
 have its successor @p item defined

 @param [in] p_suc_id is the identifier of the @p item that will
 be successor of @p p_item_id
 */
    void set_suc(const production_id & p_prod_id,
                 const item_id & p_item_id,
                 const item_id & p_suc_id) ;


    /** @brief Defines the alternative tenacitas::interpreter::item
 of a tenacitas::interpreter::item

 @param [in] p_prod_id is the identifier of the @p production
 which holds the @p item which identifier is @p p_item_id

 @param [in] p_item_id is the identifier of the @p item that will
 have its alternative @p item defined

 @param [in] p_alt_id is the identifier of the @p item that will
 be alternative to @p p_item_id */
    void set_alt(const production_id & p_prod_id,
                 const item_id & p_item_id,
                 const item_id & p_alt_id) ;


    /** Defines that the altenative tenacitas::interpreter::item accepts
 any tenacitas::interpreter::symbol

 @param [in] p_prod_id is the identifier of the @p :production
 which holds the @p item which identifier is @p p_item_id

 @param [in] p_item_id is the identifier of the @p item that will
 accept any symbol as an alternative to the one it holds */
    void accepts_anything(const production_id & p_prod,
                          const item_id & p_item);

private:

    /// Private Internal Types

    /** @brief Type for the imported grammars */
    typedef list< grammar_id > imported_grammars;

    /// Private Accessors


    /** @brief Retrieves a reference to a
 tenacitas::interpreter::symbol::type based on a
 tenacitas::interpreter::symbol::type

 @param [in] p_symbol_type is the value of the
 tenacitas::interpreter::symbol::type we want to retrieve a
 reference to

 @return A valid reference to the searched
 tenacitas::interpreter::symbol::type
 @return A null reference, otherwise */
    std::shared_ptr<const symbol::type>
    find_type(const symbol::type & p_symbol_type) const;

    /** @brief Retrieves a reference to a
 tenacitas::interpreter::symbol::value based on a
 tenacitas::interpreter::symbol::value

 @param [in] p_symbol_type is the value of the
 tenacitas::interpreter::symbol::value we want to retrieve a
 reference to

 @return A valid reference to the searched
 tenacitas::interpreter::symbol::value
 @return A null reference, otherwise */
    std::shared_ptr<const symbol::value>
    find_value(const symbol::value & p_symbol_value) const;


    /** @brief Retrieves a reference to a
 tenacitas::interpreter::type_recognizer based on its
 tenacitas::interpreter::recognizer_id

 @param [in] p_reco_id is a @p recognizer_id for which one
 wants to find a @p type_recognizer

 @return a valid reference to a @p type_recognizer object, if
 found
 @return an invalid reference if not */
    std::shared_ptr<const type_recognizer>
    find_type_recognizer (const recognizer_id & p_reco_id) const;


    /** @brief Retrieves a reference to a
 tenacitas::interpreter::routine based on its
 tenacitas::interpreter::routine_id

 @param [in] p_rout_id is the tenacitas::interpreter::routine_id
 of the tenacitas::interpreter::routine one wants to retrieve

 @return a reference to the searched
 tenacitas::interpreter::routine
 @return a null reference, otherwise */
    std::shared_ptr<const routine>
    find_routine(const routine_id & p_rout_id) const;


    /** @brief Retrieves a reference to a
 tenacitas::interpreter::production based on its
 tenacitas::interpreter::production_id

 @param [in] p_prod_id is the
 tenacitas::interpreter::production_id of the
 tenacitas::interpreter::production one wants to retrieve

 @return A reference to the searched
 tenacitas::interpreter::production
 @return A null reference, otherwise */
    std::shared_ptr<production>
    find_production(const production_id & p_prod_id) const ;

    /// Private Processors

    /** Informs if a @p grammar was already imported

 @param [in] p_grammar is the identifier of a @p grammar

 @return @p true if the @p grammar @p id was already imported
 @return @p false otherwise */
    bool already_imported (std::shared_ptr<const grammar> p_grammar) const;


    /** @brief Tries to import the @p production objects from another @p
 grammar

 @param [in] p_grammar is the @p grammar from where we want to
 import @p production objects from

 @return @p true if it was possible to import the productions,
 i.e., if there was no @p production object in @p p_grammar where
 the @p production_id is equal to any existing @p production in
 this @p grammar

 @return @false, otherwise */
    bool import_productions (const grammar & p_grammar) ;

    /** @brief Tries to import the @p type_recognizers from another @p
 grammar

 @param [in] p_grammar is the @p grammar from where we want to
 import @p type_recognizer objects from */
    void import_type_recognizers (const grammar & p_grammar) ;

    /** @brief Tries to import the @p tokens_list from
 another @p grammar

 @param [in] p_grammar is the @p grammar from where we want to
 import @p tokens objects */
    void import_tokens (const grammar & p_grammar) ;

    /** @brief Tries to import the @p production_routine objects, all the
 @p value_routine objects, and all the @p type_routine objects
 from another @p grammar. If any of the @p routine from @p
 p_grammar has the same @p routine_id if the @p routine objects
 that exist in this grammar, the import fails

 @param [in] p_grammar is the @p grammar from where we want to
 import @p routine objects

 @return @true if it was possible to import all the @p
 production_routine objects, all the @p value_routine objects,
 and all the @p type_routine objects

 @retun @p false otheriswe */
    bool import_routines (const grammar & p_grammar) ;

    /** @brief Check if there is any @p production_routine in a @p
 grammar which @p routine_id matches one in this grammar

 @todo implement it*/
    bool check_production_routines (const grammar & p_grammar);

    /** @brief Check if there is any @p value_routine in a @p
 grammar which @p routine_id matches one in this grammar
 @todo implement it */
    bool check_value_routines (const grammar & p_grammar);

    /** @brief Check if there is any @p type_routine in a @p
 grammar which @p routine_id matches one in this grammar
 @todo implement it */
    bool check_type_routines (const grammar & p_grammar) ;

    /** @brief Imports all @p prodution_routine object from a @p grammar
 into this @p grammar
 @todo implement it */
    void import_production_routines (const grammar & p_grammar);

    /** @brief Imports all @p value_routine object from a @p grammar
 into this @p grammar
 @todo implement it */
    void import_value_routines (const grammar & p_grammar) ;

    /** @brief Imports all @p type_routine object from a @p grammar
 into this @p grammar
 @todo implement it */
    void import_type_routines (const grammar & p_grammar) ;


    /// Private Attributes

    /** @brief The identifier of the @p grammar */
    const grammar_id m_id;


    /** @brief Recognizers by type of this @p grammar */
    std::shared_ptr<type_recognizers> m_type_recognizers;

    /** @brief Values recognizeds directly from the input text */
    std::shared_ptr<tokens> m_tokens;

    /** @brief All the routines of the @p grammar */
    routines m_routines;

    /** @brief All the grammars already imported */
    imported_grammars m_imported_grammars;

    /** All the productions of the @p grammar */
    productions m_productions;

};






/** @brief A @p production is a group of tenacitas::interpreter::item,
 that defines sequences of valid symbols in a grammar

 Please refer to the tenacitas::interpreter::grammar for a complete
 documentation

 @nosubgrouping */
class grammar::production {
    // -----------------------------------------------------------------
    // grammar::production
    /// @name Friends
    /// @{

    friend class grammar;

    /// @} ----------

public:
    // -----------------------------------------------------------------
    // grammar::production
    /// @name Public Internal Types
    /// @{

    class item;

    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production
    /// @name Public Constructors
    /// @{

    /** @brief We expect no sub-classes */
    ~production();

    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production
    /// @name Public Accessors
    /// @{


    /** @brief Retrieves the @p production's identifier */
    const production_id & get_id() const;

    /** @brief retrieves a reference to the first
 tenacitas::interpreter::item of the @p production */
    std::shared_ptr<item> get_first() const;


    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production
    /// @name Public Helpers
    /// @{

    void report(ostringstream & s) const;

    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production
    /// @name Public Operators
    /// @{


    /** @brief A @p production is equal to another if theirs id's are
 equal */
    bool operator == (const production & p_prod) const;

    /** @brief A @p production is equal to another if theirs id's are
 different */
    bool operator != (const production & p_prod) const;

    /// @} ----------


private:

    // -----------------------------------------------------------------
    // grammar::production
    /// @name Private Internal Types
    /// @{

    /** @brief Collection of tenacitas::interpreter::item objects, that
 form a @p production */
    typedef list<std::shared_ptr<item> >itens;

    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production
    /// @name Private Constructors
    /// @{

    /** @brief Creates a @p production, defining its identifier */
    production(const production_id & p_id);

    /// @} ----------


    // -----------------------------------------------------------------
    // grammar::production
    /// @name Private Accessors
    /// @{


    /** @brief Given a tenacitas::interpreter::item_id, tries to
 retrieve a reference to an tenacitas::interpreter::item

 @param [in] p_item_id is the tenacitas::interpreter::item_id
 of the tenacitas::interpreter::item which one wants to retrieve
 a reference to

 @return a valid reference to a tenacitas::interpreter::item, if
 there is a tenacitas::interpreter::item which
 tenacitas::interpreter::item_id is @p p_item_id

 @return a null reference otherwise; */
    std::shared_ptr<item> find(const item_id & p_item_id) const;

    /** @brief Adds an tenacitas::interpreter::item to a @p production,
 returning a reference to it.

 @param [in] p_item_id is the tenacitas::interpreter::item_id of
 the new tenacitas::interpreter::item

 @return a reference to the new tenacitas::interpreter::item */
    std::shared_ptr<item> add(const item_id & p_item_id);


    void add_production_routine (std::shared_ptr<const production_routine>
                                 p_routine);

    void
    add_symbol_type_routine (std::shared_ptr<const symbol_type_routine>
                             p_routine);


    void
    add_symbol_value_routine (std::shared_ptr<const symbol_value_routine>
                              p_routine);



    /// @} ----------


    // -----------------------------------------------------------------
    // grammar::production
    /// @name Private Attributes
    /// @{

    /** @brief The identifier of the production */
    const production_id m_id;

    /** @brief The collection of tenacitas::interpreter::item of the @p
 production (*/
    itens m_itens;

    /// @} ----------

};





/** @brief A @p item is an element of a
 tenacitas::interpreter::production.

 Please refer to the tenacitas::interpreter::grammar for a complete
 documentation

 @nosubgrouping */

class grammar::production::item {
    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Friends
    /// @{

    friend class grammar;
    friend class grammar::production;

    /// @} ----------


public:

    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Public Constructors
    /// @{

    /** @brief Creates an item, defining its id, with no successor, no
 alternative item, no tenacitas::interpreter::routine, no
 tenacitas::interpreter::symbol::type, no
 tenacitas::interpreter::symbol::value, and defining this item as
 the last one in a @p production

 @param [in] p_id is the tenacitas::interpreter::item_id of this
 @p item

 @param [in] p_prod is the tenacitas::interpreter::production to
 which this item can belong to */
    item(const item_id & p_id,
         std::shared_ptr<const production> p_prod = 0);

    /** @brief We do not expect any inheritance */
    ~item();

    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Public Accessors
    /// @{

    /** @brief Retrieves the item's identifier */
    const item_id & get_id () const;

    /** @return true if this item references a
 tenacitas::interpreter::symbol::type
 @return false otherwise */
    bool points_to_type () const;

    /** @return true if this item references a
 tenacitas::interpreter::symbol::value
 @return false otherwise */
    bool points_to_value () const;

    /** @return true if this item references a
 tenacitas::interpreter::production
 @return false otherwise */
    bool points_to_production () const;

    /** @return a valid reference to a tenacitas::interpreter::routine
 referenced by this @p item, if it exists
 @return 0, if there is no tenacitas::interpreter::routine
 referenced by this @p item */
    std::shared_ptr<const routine> get_routine () const;

    /** @return a valid reference to a
 tenacitas::interpreter::symbol::type
 referenced by this @p item, if it exists
 @return 0, if there is no tenacitas::interpreter::symbol::type
 referenced by this @p item */
    std::shared_ptr<const symbol::type> get_symbol_type () const;

    /** @return a valid reference to a
 tenacitas::interpreter::symbol::value
 referenced by this @p item, if it exists
 @return 0, if there is no tenacitas::interpreter::symbol::value
 referenced by this @p item */
    std::shared_ptr<const symbol::value> get_symbol_value () const;

    /** @return a valid reference to a
 tenacitas::interpreter::production referenced by this @p item,
 if it exists
 @return 0, if there is no tenacitas::interpreter::production
 referenced by this @p item */
    std::shared_ptr<const production> get_production () const;

    /** @return the tenacitas::interpreter::production that owns this @p
 item */
    std::shared_ptr<const production> get_production_owner() const;

    /** @return a valid reference to a
 tenacitas::interpreter::item, which is the successor @p item of
 this @p item, if it exists
 @return 0, if this @p item has no successor @p item */
    std::shared_ptr<const item> get_suc () const;

    /** @return a valid reference to a
 tenacitas::interpreter::item, which is the alternative @p item
 of this @p item, if it exists
 @return 0, if this @p item has no alternative @p item */
    std::shared_ptr<const item> get_alt () const;

    /** @return @p true if this @p item accepts any
 tenacitas::interpreter::symbol_id as an alternative @p item
 @return @p false othewise */
    bool accepts_anything () const;

    /** @return @p true if this @p item is the last one in a @p
 tenacitas::interpreter::production
 @return @p false othewise */
    bool is_last () const;


    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Public Helpers
    /// @{

    void report(ostringstream & s) const;

    /// @} ----------


    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Public Operators
    /// @{


    /** @brief An @p item is equal to another if theirs id's are
 equal */
    bool operator == (const item & p_item) const;

    /** @brief An @p item is equal to another if theirs id's are
 different */
    bool operator != (const item & p_item) const;

    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Public Attributes
    /// @{

    /// @} ----------
private:

    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Private Accessors
    /// @{

    /** @brief Defines that this item references a @p
 production_routine */
    void set_production_routine (std::shared_ptr<const production_routine>
                                 p_routine) ;

    /** @brief Defines that this item references a @p
 symbol_type_routine */
    void
    set_symbol_type_routine (std::shared_ptr<const symbol_type_routine>
                             p_routine) ;

    /** @brief Defines that this item references a @p
 symbol_type_routine */
    void
    set_symbol_value_routine (std::shared_ptr<const symbol_value_routine>
                              p_routine) ;


    /** @brief Defines the tenacitas::interpreter::routine referenced by
 this @p item

 @param [in] p_rout is a reference to the
 tenacitas::interpreter::routine that will be referenced by the
 @p item */
    void set_routine(std::shared_ptr<const routine> p_rout);

    /** @brief Defines the tenacitas::interpreter::symbol::type
 referenced by this @p item

 @param [in] p_symbol_type is the
 tenacitas::interpreter::symbol::type reference to be referenced
 by this @p item */
    void
    set_symbol_type(std::shared_ptr<const symbol::type> p_symbol_type);

    /** @brief Defines the tenacitas::interpreter::symbol::value
 referenced by this @p item

 @param [in] p_symbol_value is the
 tenacitas::interpreter::symbol::value reference to be referenced
 by this @p item */
    void
    set_symbol_value (std::shared_ptr<const symbol::value> p_symbol_value);

    /** @brief Defines the tenacitas::interpreter::production referenced
 by this @p item

 @param [in] p_prod is the tenacitas::interpreter::production
 reference to be referenced by this @p item */
    void set_production(std::shared_ptr<const production> p_prod);

    /** @brief Defines the successor item

 @param [in] p_item is the successor @p item of the @p item */
    void set_suc(std::shared_ptr<const item> p_item);

    /** @brief Defines the item alternative item

 @param [in] p_item is the alternative @p item of the @p item */
    void set_alt(std::shared_ptr<const item> p_item) ;

    /** @brief Defines that this @p item accepts any @p
 tenacitas::interpreter::symbol_id as an alternative */
    void accepts_anything ();

    /// @} ----------

    // -----------------------------------------------------------------
    // grammar::production::item
    /// @name Private Attributes
    /// @{

    /** @brief tenacitas::interpreter::item_id of a "dummy" @p item,
 which is a special @p item used by the
 tenacitas::interpreter::parser to process the
 tenacitas::interpreter::grammar */
    // static
    static const item_id dummy_item_id;

    /** @brief tenacitas::interpreter::item_id of the @p item referenced
 by the @p item that accept any tenacitas::interpreter::symbol_id
 as an alternative */
    // static
    static const item_id accepts_any_symbol_id;


    /** @brief The "dummy" @p item is a static refence because we only
 need one per tenacitas::interpreter::production, as all the
 tenacitas::interpreter::item objects that reference the @p
 dummy_item (as successor or alternative) will reference the same
 object */
    static const std::shared_ptr<const item> dummy_item;

    /** @brief The "accept any symbol" @p item is a static refence
 because we only need one per
 tenacitas::interpreter::production, as all the
 tenacitas::interpreter::item objects that reference the @p
 accepts_any_symbol will reference the same object */
    static const std::shared_ptr<item> accepts_any_symbol;


    /** @brief To save some memory when (hopely not!) throwing
 exceptions */
    static const string m_class_name;


    /** @brief Identification of this item */
    const item_id m_id;


    /** @brief Production owner of the item */
    std::shared_ptr<const production> m_prod_owner;

    /** The tenacitas::interpreter::symbol::type pointed to by this item,
 if this @p item references a type in the
 tenacitas::interpreter::grammar */
    std::shared_ptr<const symbol::type> m_symbol_type;


    /** The tenacitas::interpreter::symbol::value pointed to by this
 item, if this @p item references a value in the
 tenacitas::interpreter::grammar */
    std::shared_ptr<const symbol::value> m_symbol_value;

    /** @brief the production pointed to by this item, if this item
 points to a non-terminal */
    std::shared_ptr<const production> m_prod;

    /** @brief The successor item of this item */
    std::shared_ptr<const item> m_suc;

    /** @brief The alternative item of this item */
    std::shared_ptr<const item> m_alt;

    /** @brief The semantic routine, if associated */
    std::shared_ptr<const routine> m_routine;

    /// @} ----------

};

// ############################################################################
// grammar

// ----------------------------------------------------------------------------
template <typename t_grammar>
void
grammar::
add_grammar () {

    t_grammar l_grammar;

    /// checking if @p p_grammar was already imported
    if (already_imported (l_grammar)) {
        goto l_out;
    }

    if (l_grammar->m_productions.size () < 2) {
        /// the grammar informed has no productions, so we just leave
        goto l_out;
    }

    /// importing the @p productions
    if (!import_productions (l_grammar)) {
        goto l_out;
    }

    /// importing the @p type_recognizers
    if (!import_type_recognizers (l_grammar)) {
        goto l_out;
    }

    /// importing the @p values_recognizers
    if (!import_tokens (l_grammar)) {
        goto l_out;
    }

    /// importing the @p routines
    if (!import_routines (l_grammar)) {
        goto l_out;
    }

    /// all ok


l_out:
    return;
}



}
}
}

// ====================================================================
// ====================================================================
// ====================================================================

// I M P L E M E N T A T I O N

// ====================================================================
// ====================================================================
// ====================================================================


using namespace tenacitas::interpreter;



#endif

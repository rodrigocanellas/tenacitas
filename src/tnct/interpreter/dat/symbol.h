#ifndef TNCT_INTERPRETER_DAT_SYMBOL_H
#define TNCT_INTERPRETER_DAT_SYMBOL_H

#include <iostream>

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

/// @brief Represents a symbol read from the text being interpreted
///
/// A symbol contains a lexema, which is the string read from the text being
/// analysed, and a type, like "integer", "variable_identifier" or
/// "reserved_word". The type of a symbol is determined by a
/// tenacitas::interpreter::bus::scanner object.
///
/// @tparam t_lexema_size is the maximum size of a lexema string
template <std::size_t t_lexema_size> class symbol_t final {

public:
  using lexema = lexema_t<t_lexema_size>;
  using lexema_const_reference = typename lexema::const_reference;

  explicit symbol_t(lexema_const_reference p_lexema_const_reference,
                    type p_type = dat::unknow_type)
      : m_lexema_const_reference{p_lexema_const_reference}, m_type{p_type} {}

  explicit symbol_t(std::string &&p_string, type p_type)
      : m_string{std::move(p_string)}, m_type{p_type} {}

  symbol_t() = delete;
  symbol_t(const symbol_t &) = default;
  symbol_t(symbol_t &&) = default;
  ~symbol_t() = default;

  symbol_t &operator=(const symbol_t &p_symbol) = default;
  symbol_t &operator=(symbol_t &&p_symbol) = default;

  bool is_token() const { return m_lexema_const_reference.has_value(); }

  std::optional<lexema_const_reference> get_lexema() const {
    return (m_lexema_const_reference ? m_lexema_const_reference : std::nullopt);
  }

  std::optional<std::reference_wrapper<const std::string>> get_value() const {
    if (m_string) {
      return {*m_string};
    }
    return std::nullopt;
  }

  type get_type() const { return m_type; };

  bool operator==(const symbol_t &p_symbol) const {
    if (m_type != p_symbol.m_type) {
      return false;
    }
    if (m_lexema_const_reference) {
      return (m_lexema_const_reference.get() ==
              p_symbol.m_lexema_const_iterator.get());
    }
    return (*m_string == *p_symbol.m_string);
  }

  bool operator!=(const symbol_t &p_symbol) const {
    return !(*this == p_symbol);
  };

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const symbol_t &p_symbol) {
    std::optional<const lexema &> _lexema{p_symbol.get_lexema()};

    p_out << '(';
    if (_lexema) {
      p_out << *_lexema;
    } else {
      p_out << "NO-LEXEMA";
    }
    p_out << ',' << p_symbol.get_type() << ')';
    return p_out;
  }

private:
  std::optional<lexema_const_reference> m_lexema_const_reference;

  std::optional<std::string> m_string;

  type m_type;
};

} // namespace tnct::interpreter::dat

// /// @brief A collection to constant references to an information of a
// ///    tenacitas::interpreter::symbol object
// ///    The type parameter @p t_symbol_info can be
// ///    tenacitas::interpreter::lexema or
// ///    tenacitas::interpreter::symbol::type
// template <typename t_symbol_info> class symbols_info {

// public:
//   /** @brief Type of information stored */
//   typedef t_symbol_info symbol_info;

//   /** @brief Reference to the information stored */
//   typedef
//       typename std::list<std::shared_ptr<const
//       t_symbol_info>>::const_iterator
//           const_iterator;

//   /** @brief Constructor

//       @param p_info is a string containg space separeted @p
//       symbol_info objects, , like "decimal_integer real", if @p
//       symbo_info is tenacitas::interpreter::symbol::type; or like "= ==
//       ;" if @p symbo_info is tenacitas::interpreter::lexema */
//   symbols_info(const std::string &p_values);

//   ~symbols_info();

//   /** @brief Tries to find a constant reference to an object of @p
//       t_symbol_info based on a @p t_symbol_info object

//       @param p_symbol_info is the @p t_symbol_info used as base to
//       find a reference to a @p t_symbol_info

//       @return a valid constante reference to a @p t_symbol_info, if
//       found

//       @return a null reference, if not */
//   std::shared_ptr<const t_symbol_info> find(const t_symbol_info &p_info)
//   const;

//   /** @brief Adds a @p t_symbol_info to the collection

//       @param [in] p_symbol_info is the @p t_symbol_info we want to
//       insert into the collection

//       @return a reference to the just insert @p p_symbol_info */
//   std::shared_ptr<const t_symbol_info> add(const t_symbol_info
//   &p_symbol_info);

//   /** @brief Returns a reference to the first <tt> std::shared_ptr<const
//       t_symbol_info> </tt> of the collection */
//   const_iterator begin() const;

//   /** @brief Returns a reference to the end of the of the
//       collection  */
//   const_iterator end() const;

// private:
//   /** @brief type of the collection of constant references to @p
//       t_symbol_info objects */
//   typedef std::list<std::shared_ptr<const t_symbol_info>> symbols_info_list;

//   /** @brief Decodes a string to a list of @p symbols_info_list object

//       @param p_info is a string containg space separeted @p
//       symbol_info objects, , like "decimal_integer real", if @p
//       symbo_info is tenacitas::interpreter::symbol::type; or like "= ==
//       ;" if @p symbo_info is tenacitas::interpreter::lexema */
//   void string2list(const std::string &p_info);

//   /** @brief The collection of constant references to @p t_symbol_info
//       objects  */
//   symbols_info_list m_symbols_info_list;
// };

// /** @brief Specialization of the tenacitas::interpreter::symbols_info
//     collection to store constant references to
//     tenacitas::interpreter::lexema objects */
// typedef symbols_info<lexema> symbols_values;

// /** @brief Specialization of the tenacitas::interpreter::symbols_info
//     collection to store constant references to
//     tenacitas::interpreter::symbol::type objects */
// typedef symbols_info<symbol::type> symbols_types;

// /// @brief Collection of references to tenacitas::interpreter::symbol
// ///    objects
// class symbols {

// public:
//   typedef std::list<std::shared_ptr<symbol>>::iterator iterator;
//   typedef std::list<std::shared_ptr<symbol>>::const_iterator const_iterator;
//   typedef std::list<std::shared_ptr<symbol>>::size_type size_type;

//   /** @brief Constructor */
//   symbols();

//   /** @brief  */
//   ~symbols();

//   /** @brief Adds a tenacitas::interpreter::symbol to the collection

//       @param p_symbol is the @p symbol to be added to the collection */
//   void add(std::shared_ptr<symbol> p_symbol);

//   /** @brief References the first <tt>
//       tenacitas::std::shared_ptr<tenacitas::interpreter::symbol> </tt> of
//       the collection */
//   iterator begin();

//   /** @brief The end of the collection */
//   iterator end();

//   /** @brief References the first <tt>
//       tenacitas::std::shared_ptr<tenacitas::interpreter::symbol> </tt> of
//       the collection */
//   const_iterator begin() const;

//   /** @brief The end of the collection */
//   const_iterator end() const;

//   /** @brief Erases symbols from the collection

//       @param [in] p_begin is a reference to the first @p symbol that
//       should be deleted

//       @param [in] p_end is a reference to the @p symbol imediately
//       after the last @p symbol to be deleted */
//   void erase(iterator p_begin, iterator p_end);

// private:
//   /** @brief The collection of <tt>
//       tenacitas::std::shared_ptr<tenacitas::interpreter::symbol </tt> */
//   std::list<std::shared_ptr<symbol>> m_list;
// };

// } // namespace tnct::interpreter::dat

// // ====================================================================
// // ====================================================================
// // ====================================================================

// //         I M P L E M E N T A T I O N

// // ====================================================================
// // ====================================================================
// // ====================================================================

// using namespace tnct::interpreter::dat;

// // ####################################################################
// //                                    symbols_info

// template <typename t_symbol_info>
// symbols_info<t_symbol_info>::symbols_info(const std::string &p_info)
//     : m_symbols_info_list() {

//   string2list(p_info);
// }

// template <typename t_symbol_info>
// symbols_info<t_symbol_info>::~symbols_info() {}

// template <typename t_symbol_info>
// std::shared_ptr<const t_symbol_info>
// symbols_info<t_symbol_info>::find(const t_symbol_info &p_info) const {

//   std::shared_ptr<const t_symbol_info> rc;

//   for (const_iterator i = begin(); i != end(); ++i) {
//     if (**i == p_info) {
//       rc = *i;
//       break;
//     }
//   }
//   return rc;
// }

// template <typename t_symbol_info>
// std::shared_ptr<const t_symbol_info>
// symbols_info<t_symbol_info>::add(const t_symbol_info &p_info) {

//   std::shared_ptr<const t_symbol_info> rc = find(p_info);

//   if (rc == 0) {
//     m_symbols_info_list.push_front(
//         std::shared_ptr<t_symbol_info>(new t_symbol_info(p_info)));

//     rc = *m_symbols_info_list.begin();
//   }
//   return rc;
// }

// template <typename t_symbol_info>
// void symbols_info<t_symbol_info>::string2list(const std::string &p_info) {

//   size_t p1 = 0;
//   size_t p2 = 0;
//   size_t size = p_info.size();

// lb_init: {
//   while (true) {
//     if (p2 >= size)
//       goto lb_last_info;

//     if (p_info[p2] == ' ')
//       goto lb_middle_info;

//     ++p2;
//   }
// }

// lb_middle_info: {
//   symbol_info l_symbol_info(std::string(&p_info[p1], &p_info[p2]));

//   /// adding it to the collection
//   add(l_symbol_info);

//   /// updating the positioners
//   ++p2;
//   p1 = p2;

//   goto lb_init;
// }

// lb_last_info: {

//   symbol_info l_symbol_info(std::string(&p_info[p1],
//   &p_info[p_info.size()]));

//   /// adding it to the collection
//   add(l_symbol_info);
// }
// }

// template <typename t_symbol_info>
// typename symbols_info<t_symbol_info>::const_iterator
// symbols_info<t_symbol_info>::begin() const {
//   return m_symbols_info_list.begin();
// }

// template <typename t_symbol_info>
// typename symbols_info<t_symbol_info>::const_iterator
// symbols_info<t_symbol_info>::end() const {
//   return m_symbols_info_list.end();
// }

#endif

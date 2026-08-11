// #ifndef TNCT_INTERPRETER_DAT_TYPES_H
// #define TNCT_INTERPRETER_DAT_TYPES_H

// #include <algorithm>
// #include <optional>
// #include <set>

// #include "tnct/interpreter/dat/type.h"

// namespace tnct::interpreter::dat {

// /// Collection tenacitas::interpreter::types objects
// ///
// /// @tparam t_type_size is the maximum size of a type string
// template <std::size_t t_lexema_size, std::size_t t_type_size>
// class types_t final {

// public:
//   using type = type_t<t_type_size>;

// private:
//   using container = std::set<type>;

// public:
//   using type_const_iterator = typename container::const_iterator;

//   types_t(std::initializer_list<type> p_types) : m_container{p_types} {}

//   types_t() = default;
//   types_t(const types_t &) = delete;
//   types_t(types_t &&) = default;
//   ~types_t();

//   types_t &operator=(const types_t &) = delete;
//   types_t &operator=(types_t &&) = default;

//   void add(const type &p_types) { m_container.push_back(p_types); }

//   void add(type &&p_type) { m_container.push_back(std::move(p_type)); }

//   type_const_iterator begin() const { return m_container.begin(); }

//   type_const_iterator end() const { return m_container.end(); }

//   std::optional<type_const_iterator> find(const type &p_type) const {
//     type_const_iterator _ite{
//         std::find(m_container.begin(), m_container.end(), p_type)};
//     return (_ite == m_container.end()) ? std::nullopt : _ite;
//   }

//   /// Tries to recognize a std::string as a lexema of this type
//   ///
//   /// \param p_walker is where the string begins
//   ///
//   /// \return if the string is recognized, it returns an iterator to its type
//   /// and the end of the string
//   std::optional<std::pair<type_const_iterator, std::string::const_iterator>>
//   recognize(std::string::const_iterator p_begin) const {

//     std::string::const_iterator _end;
//     auto criteria = [&](const type &p_type) {
//       std::optional<std::string::const_iterator> _recognized{
//           p_type.recognize(p_begin)};
//       if (_recognized) {
//         _end = *_recognized;
//         return true;
//       }
//       return false;
//     };

//     typename container::const_iterator _ite{
//         std::find_if(m_container.begin(), m_container.end(), criteria)};
//     return (_ite == m_container.end() ? std::nullopt : _ite);
//   }

// private:
//   container m_container;
// };

// } // namespace tnct::interpreter::dat

// #endif

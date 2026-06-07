#ifndef TNCT_INTERPRETER_DAT_TYPE_H
#define TNCT_INTERPRETER_DAT_TYPE_H

#include <cstdint>
#include <limits>

// #include <iostream>
// #include <string>

// #include "tnct/string/dat/fixed_size_string.h"

namespace tnct::interpreter::dat {

using type = std::uint16_t;

static constexpr type unknow_type{std::numeric_limits<type>::max()};
static constexpr type end_of_text{std::numeric_limits<type>::max() - 1};

// /// The type of the @p symbol, like "integer", "variable_identifier" or
// /// "reserved_word".
// ///
// /// @tparam t_type_size is the maximum size of a type string
// template <std::size_t t_type_size> class type_t final {

// public:
//   explicit type_t(std::string_view p_string) : m_string(p_string) {}

//   type_t() = delete;
//   type_t(const type_t &) = default;
//   type_t(type_t &&) = default;
//   ~type_t() = default;

//   type_t &operator=(const type_t &) = default;
//   type_t &operator=(type_t &&) = default;

//   operator std::string() const { return m_string; }

//   bool operator==(const type_t &p_type) const {
//     return m_string == p_type.m_string;
//   }

//   bool operator!=(const type_t &p_type) const {
//     return m_string != p_type.m_string;
//   }

//   bool operator<(const type_t &p_type) const {
//     return m_string < p_type.m_string;
//   }

//   friend std::ostream &operator<<(std::ostream &p_out, const type_t &p_type)
//   {
//     p_out << p_type.m_string;
//     return p_out;
//   }

//   std::size_t size() const { return m_string.size(); }

//   /// @brief Very special type_t,indicating that all the input text was
//   analysed static const type_t eot;

// private:
//   string::dat::fixed_size_string<t_type_size> m_string;
// };

// template <std::size_t t_type_size>
// const type_t<t_type_size> type_t<t_type_size>::eot{"!!eot!!"};

} // namespace tnct::interpreter::dat
#endif

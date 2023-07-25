#ifndef TENACITAS_LIB_TRAITS_ALG_POINTER_H
#define TENACITAS_LIB_TRAITS_ALG_POINTER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <memory>
#include <type_traits>

/// \brief master namespace
namespace tenacitas::lib::traits::alg {

/// \brief Determines is a type is not a shared pointer
template <typename t> struct is_shared_ptr : std::false_type {};

template <typename t>
struct is_shared_ptr<std::shared_ptr<t>> : std::true_type {};

/// \brief Alias for a \p is_shared_ptr value
template <typename t> constexpr bool is_shared_ptr_v = is_shared_ptr<t>::value;

/// \brief Determines is a type is not a unique pointer
template <typename t> struct is_unique_ptr : std::false_type {};

/// \brief Determines is a type is a unique pointer
template <typename t>
struct is_unique_ptr<std::unique_ptr<t>> : std::true_type {};

/// \brief Alias for a \p is_unique_ptr value
template <typename t> constexpr bool is_unique_ptr_v = is_unique_ptr<t>::value;

/// \brief Determines if a type is a shared or unique pointer
template <typename t>
using is_smart_ptr = std::conditional_t<
    is_shared_ptr_v<t>, std::true_type,
    std::conditional_t<is_unique_ptr_v<t>, std::true_type, std::false_type>>;

/// \brief Alias to determine if a type is shared or unique pointer
template <typename T> constexpr bool is_smart_ptr_v = is_smart_ptr<T>::value;

} // namespace tenacitas::lib::traits::alg
#endif // POINTER_H

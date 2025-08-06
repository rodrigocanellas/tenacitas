/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_MEMORY_TRAITS_IS_SMART_PTR_H
#define TENACITAS_SRC_MEMORY_TRAITS_IS_SMART_PTR_H

#include <memory>
#include <type_traits>

namespace tenacitas::src::menmory::traits
{

/// \brief Determines is a type is not a shared
/// pointer
template <typename t>
struct is_shared_ptr : std::false_type
{
};

template <typename t>
struct is_shared_ptr<std::shared_ptr<t>> : std::true_type
{
};

/// \brief Determines is a type is not a unique pointer
template <typename t>
struct is_unique_ptr : std::false_type
{
};

/// \brief Determines is a type is a unique pointer
template <typename t>
struct is_unique_ptr<std::unique_ptr<t>> : std::true_type
{
};

/// \brief Determines if a type is a shared or unique pointer
/// \note Version improved by ChatGPT
template <typename T>
struct is_smart_ptr : std::disjunction<is_shared_ptr<T>, is_unique_ptr<T>>
{
};

template <typename t>
inline constexpr bool is_smart_ptr_v = is_smart_ptr<t>::value;

} // namespace tenacitas::src::menmory::traits

#endif

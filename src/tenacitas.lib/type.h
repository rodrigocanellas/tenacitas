#ifndef TENACITAS_TYPE_H
#define TENACITAS_TYPE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <memory>
#include <type_traits>
#include <utility>

/// \brief master namespace
namespace tenacitas {

/// \brief some type traits and wrappers
namespace type {

/// \brief A shorter name for std::shared_ptr
template <typename t>
using sptr = std::shared_ptr<t>;

/// \brief A shorter name for std::make_shared
template <typename t_type, typename... t_params>
sptr<t_type> make_sptr(t_params &&... p_params) {
    return std::make_shared<t_type>(std::forward<t_params>(p_params)...);
}

/// \brief A shorter name for std::unique_ptr
template <typename t>
using uptr = std::unique_ptr<t>;

/// \brief A shorter name for std::make_unique
template <typename t_type, typename... t_params>
uptr<t_type> make_uptr(t_params &&... p_params) {
    return std::make_unique<t_type>(std::forward<t_params>(p_params)...);
}

/// \brief Type trait that identifies if a type is not a tuple
template <typename>
struct is_tuple {
    const static bool value {false};
};

/// \brief Type trait that identifies if a type is a tuple
template <typename... T>
struct is_tuple<std::tuple<T...>> {
    const static bool value {true};
};

} // namespace type

} // namespace tenacitas

#endif

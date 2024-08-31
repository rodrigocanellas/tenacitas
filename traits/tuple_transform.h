/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TRAVERSE_TUPLE_TRANSFORM_H
#define TENACITAS_LIB_TRAITS_TRAVERSE_TUPLE_TRANSFORM_H

#include <tuple>

#include <tenacitas.lib/traits/tuple_like.h>

namespace tenacitas::lib::traits {

// Base class for specialization
template <tuple_like t_uple, template <typename> typename t_converter>
struct tuple_transform;

/**
 * @brief Transforms each type of a tuple into another type
 *
 * @tparam ts are the types of the elements of the tuple
 * @tparam t_converter is the converter struct that will modify the type of each
 * type in \p ts
 *
 * @example
#include <memory>
#include <string>
#include <tuple>
#include <vector>

template <typename t_uple, template <typename> typename t_converter>
struct tuple_transform;

template <typename... ts, template <typename> typename t_converter>
struct tuple_transform<std::tuple<ts...>, t_converter> {
    using type = std::tuple<t_converter<ts>...>;
};

using my_tuple = std::tuple<int, double, char, std::vector<std::string>>;
using my_transformed_tuple =
    typename tuple_transform<my_tuple, std::shared_ptr>::type;

int main() {

    // is a tuple of std::shared_ptr<int>, std::shared_ptr<double>,
    // std::shared_ptr<char>, , std::shared_ptr<std::vector<std::string>>
    my_transformed_tuple my_transformed_tuple_;

    return 0;
}
*/
template <typename... ts, template <typename> typename t_converter>
struct tuple_transform<std::tuple<ts...>, t_converter> {
  using type = std::tuple<t_converter<ts>...>;
};

} // namespace tenacitas::lib::traits

#endif

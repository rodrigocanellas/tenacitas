/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TUPLE_TUPLE_TRANSFORM_H
#define TENACITAS_LIB_TUPLE_TUPLE_TRANSFORM_H

#include <tuple>

#include <tenacitas.lib/tuple/tuple_like.h>

namespace tenacitas::lib::tuple {

// Base class for specialization
template <tuple::tuple_like t_uple, template <typename> typename t_converter>
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

#include <tenacitas.lib/tuple/tuple_transform.h>

using namespace tenacitas::lib;

using my_tuple = std::tuple<int, double, char, std::vector<std::string>>;
using my_transformed_tuple =
    tuple::tuple_transform_t<std::shared_ptr, my_tuple>;

int main() {

    // is a tuple of std::shared_ptr<int>, std::shared_ptr<double>,
    // std::shared_ptr<char>, , std::shared_ptr<std::vector<std::string>>
    my_transformed_tuple my_transformed_tuple_;

    return 0;
}
*/
template <template <typename> typename t_converter, typename... ts>
struct tuple_transform<std::tuple<ts...>, t_converter> {
  using type = std::tuple<t_converter<ts>...>;
};

template <template <typename> typename t_converter, typename... ts>
using tuple_transform_t = typename tuple_transform<ts..., t_converter>::type;

} // namespace tenacitas::lib::tuple

#endif

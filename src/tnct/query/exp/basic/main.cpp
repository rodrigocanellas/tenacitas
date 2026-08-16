/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <string>
#include <tuple>

#include "tnct/container/cpt/index.h"
#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::query::trt {}

namespace tnct::query::cpt {

template <typename t>
concept chunked_container = requires(t p_t) {
  requires std::same_as<std::remove_cv_t<decltype(t::chunk_size)>, std::size_t>;
};

template <typename t>
concept data_field = requires(t p_t) {
  typename t::type;

  { p_t.name } -> std::same_as<std::string>;
};

template <typename t_tuple>
concept tuple_contains_only_fields =

    tuple::cpt::is_tuple<t_tuple> &&

    []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
      return (query::cpt::data_field<std::tuple_element_t<t_idx, t_tuple>> &&
              ...);
    }(std::make_index_sequence<std::tuple_size_v<t_tuple>>());

template <typename t>
concept data_set = requires(t p_t) {
  { p_t.fields };

  requires(query::cpt::tuple_contains_only_fields<decltype(p_t.fields)>);
};
} // namespace tnct::query::cpt

namespace tnct::query::dat {

template <typename t_type> struct field {
  using type = t_type;

  std::string name;
};

} // namespace tnct::query::dat

namespace tnct::query::bus {

template <tnct::container::cpt::index... t_indexes> struct query {

private:
  using indexes = std::tuple<t_indexes...>;

private:
  indexes m_indexes;
};

} // namespace tnct::query::bus

int main() {}

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_H
#define TNCT_CONTAINER_CPT_INDEX_H

#include <concepts>
#include <iterator>
#include <utility>

#include <tnct/tuple/cpt/is_tuple.h>

namespace tnct::container::cpt {

template <typename t_result, typename t_iterator>
concept emplace_result = std::same_as<t_result, t_iterator> ||
                         std::same_as<t_result, std::pair<t_iterator, bool>>;

template <typename t, typename t_key, typename t_object>
concept index =
    requires {
      typename t::key_type;
      typename t::mapped_type;
      typename t::value_type;
      typename t::iterator;
      typename t::const_iterator;
      typename t::key_compare;

      requires std::same_as<typename t::key_type, t_key>;

      requires std::same_as<typename t::value_type,
                            std::pair<const t_key, t_object>>;

      requires std::bidirectional_iterator<typename t::iterator>;
      requires std::bidirectional_iterator<typename t::const_iterator>;

      requires std::strict_weak_order<typename t::key_compare, t_key, t_key>;
    }

    &&

    requires(t p_t, const t p_const_t, t_key p_key, t_object &&p_ref,
             typename t::const_iterator p_const_ite) {
      { *p_t.begin() } -> std::same_as<typename t::value_type &>;

      { *p_const_t.begin() } -> std::same_as<const typename t::value_type &>;

      { p_t.emplace(p_key, p_ref) } -> emplace_result<typename t::iterator>;

      {
        p_t.equal_range(p_key)
      } -> std::same_as<std::pair<typename t::iterator, typename t::iterator>>;

      {
        p_const_t.equal_range(p_key)
      } -> std::same_as<
            std::pair<typename t::const_iterator, typename t::const_iterator>>;

      { p_t.erase(p_const_ite) } -> std::same_as<typename t::iterator>;

      { p_t.begin() } -> std::same_as<typename t::iterator>;
      { p_t.end() } -> std::same_as<typename t::iterator>;

      { p_const_t.begin() } -> std::same_as<typename t::const_iterator>;
      { p_const_t.end() } -> std::same_as<typename t::const_iterator>;
    };

} // namespace tnct::container::cpt

#endif

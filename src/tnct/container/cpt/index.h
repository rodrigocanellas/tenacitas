/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_H
#define TNCT_CONTAINER_CPT_INDEX_H

#include <concepts>
#include <utility>

namespace tnct::container::cpt {

template <typename t, typename t_key, typename t_mapped>
concept index =
    requires(t p_index, const t p_const_index, const t_key &p_key,
             t_mapped p_mapped, typename t::const_iterator p_const_ite) {
      typename t::key_type;
      typename t::mapped_type;
      typename t::value_type;
      typename t::iterator;
      typename t::const_iterator;

      requires std::same_as<typename t::key_type, t_key>;
      requires std::same_as<typename t::mapped_type, t_mapped>;

      { p_index.begin() } -> std::same_as<typename t::iterator>;

      { p_index.end() } -> std::same_as<typename t::iterator>;

      { p_const_index.begin() } -> std::same_as<typename t::const_iterator>;

      { p_const_index.end() } -> std::same_as<typename t::const_iterator>;

      {
        p_index.emplace(p_key, p_mapped)
      } -> std::same_as<std::pair<typename t::iterator, bool>>;

      {
        p_index.equal_range(p_key)
      } -> std::same_as<std::pair<typename t::iterator, typename t::iterator>>;

      {
        p_const_index.equal_range(p_key)
      } -> std::same_as<
            std::pair<typename t::const_iterator, typename t::const_iterator>>;

      { p_index.erase(p_const_ite) } -> std::same_as<typename t::iterator>;

      { p_index.find(p_key) } -> std::same_as<typename t::iterator>;

      { p_const_index.find(p_key) } -> std::same_as<typename t::const_iterator>;
    };

} // namespace tnct::container::cpt

#endif

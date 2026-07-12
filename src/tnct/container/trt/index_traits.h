/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_INDEX_TRAITS_H
#define TNCT_CONTAINER_TRT_INDEX_TRAITS_H

#include <map>

namespace tnct::container::trt {

class std_map_index_id {};
class std_multimap_index_id {};
struct no_index_id {};

template <typename t_index_id, typename t_key, typename t_value>
struct index_traits;

template <typename t_key, typename t_value>
struct index_traits<std_map_index_id, t_key, t_value> {
  using index = std::map<t_key, t_value>;
  using iterator = index::iterator;
  static constexpr bool unique = true;
};

template <typename t_key, typename t_ref> class std_multimap_index {
private:
  using impl = std::multimap<t_key, t_ref>;

public:
  using key_type = typename impl::key_type;
  using mapped_type = typename impl::mapped_type;
  using value_type = typename impl::value_type;
  using iterator = typename impl::iterator;
  using const_iterator = typename impl::const_iterator;

  std::pair<iterator, bool> emplace(const key_type &p_key,
                                    mapped_type p_mapped) {
    return {m_impl.emplace(p_key, std::move(p_mapped)), true};
  }

  std::pair<iterator, iterator> equal_range(key_type p_key) {
    return {m_impl.equal_range(p_key)};
  }

  std::pair<const_iterator, const_iterator> equal_range(key_type p_key) const {
    return {m_impl.equal_range(p_key)};
  }

  iterator erase(iterator p_ite) { return {m_impl.erase(p_ite)}; }

  iterator begin() { return m_impl.begin(); }

  iterator end() { return m_impl.end(); }

  const_iterator begin() const { return m_impl.begin(); }

  const_iterator end() const { return m_impl.end(); }

  iterator find(const key_type &p_key) { return {m_impl.find(p_key)}; }

  const_iterator find(const key_type &p_key) const {
    return {m_impl.find(p_key)};
  }

private:
  impl m_impl;
};

template <typename t_key, typename t_value>
struct index_traits<std_multimap_index_id, t_key, t_value> {
  using index = std_multimap_index<t_key, t_value>;
  using iterator = index::iterator;
  static constexpr bool unique = false;
};

struct no_index_type {};
struct no_iterator_type {};

template <typename t_key, typename t_value>
struct index_traits<no_index_id, t_key, t_value> {
  using index = no_index_type;
  using iterator = no_iterator_type;
  static constexpr bool unique = false;
};

} // namespace tnct::container::trt

#endif

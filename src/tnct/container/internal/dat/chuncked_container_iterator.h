/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_INTERNAL_DAT_CHUNKED_CONTAINER_ITERATOR_H
#define TNCT_CONTAINER_INTERNAL_DAT_CHUNKED_CONTAINER_ITERATOR_H

#include <iterator>
#include <type_traits>
#include <utility>

namespace tnct::container::internal::dat {

template <typename t_chunked_container, typename t_element>
class chunked_container_iterator {
private:
  static constexpr bool is_const_iterator = std::is_const_v<t_element>;

  using chunked_container = t_chunked_container;

  using owner_pointer =
      std::conditional_t<is_const_iterator, const chunked_container *,
                         chunked_container *>;

  using list_position = typename chunked_container::list_position;
  using array_position = typename chunked_container::array_position;

  using node = std::pair<list_position, array_position>;

public:
  using value_type = std::remove_cvref_t<t_element>;
  using difference_type = std::ptrdiff_t;
  using reference = t_element &;
  using pointer = t_element *;

  using iterator_category = std::bidirectional_iterator_tag;

#if __cplusplus >= 202002L
  using iterator_concept = std::bidirectional_iterator_tag;
#endif

  chunked_container_iterator() = default;

  chunked_container_iterator(const chunked_container_iterator &p_ite);

  chunked_container_iterator(chunked_container_iterator &&p_ite) noexcept;

  ~chunked_container_iterator() = default;

  chunked_container_iterator &
  operator=(const chunked_container_iterator &p_ite);

  chunked_container_iterator &
  operator=(chunked_container_iterator &&p_ite) noexcept;

  reference operator*() const;

  pointer operator->() const;

  chunked_container_iterator &operator++();

  chunked_container_iterator operator++(int);

  chunked_container_iterator &operator--();

  chunked_container_iterator operator--(int);

  friend bool operator==(const chunked_container_iterator &p_left,
                         const chunked_container_iterator &p_right) {
    if (p_left.m_owner != p_right.m_owner) {
      return false;
    }
    return p_left.m_node == p_right.m_node;
  }

private:
  chunked_container_iterator(node p_node, owner_pointer p_owner);

  list_position &list_pos();

  array_position &array_pos();

private:
  node m_node{};

  owner_pointer m_owner{nullptr};

  friend chunked_container;
};

} // namespace tnct::container::internal::dat

#include "tnct/container/internal/dat/chuncked_container_iterator.tpp"
#endif

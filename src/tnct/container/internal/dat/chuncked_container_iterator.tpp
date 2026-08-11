/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_INTERNAL_DAT_chuncked_container_ITERATOR_TPP
#define TNCT_CONTAINER_INTERNAL_DAT_chuncked_container_ITERATOR_TPP

#include "tnct/container/internal/dat/chuncked_container_iterator.h"

namespace tnct::container::internal::dat {

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element>::

    chunked_container_iterator(node p_node, owner_pointer p_owner)
    : m_node{p_node}, m_owner(p_owner) {}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container,
                           t_element>::chunked_container_iterator::

    chunked_container_iterator(const chunked_container_iterator &p_ite)
    : chunked_container_iterator(p_ite.m_node, p_ite.m_owner) {}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container,
                           t_element>::chunked_container_iterator::

    chunked_container_iterator(chunked_container_iterator &&p_ite) noexcept
    : m_node(std::move(p_ite.m_node)), m_owner{p_ite.m_owner} {}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element> &
chunked_container_iterator<t_chuncked_container, t_element>::
    chunked_container_iterator::operator=(
        const chunked_container_iterator &p_ite) {
  if (this != &p_ite) {
    m_node = p_ite.m_node;
    m_owner = p_ite.m_owner;
  }
  return *this;
}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element> &
chunked_container_iterator<t_chuncked_container, t_element>::operator=(
    chunked_container_iterator &&p_ite) noexcept {
  if (this != &p_ite) {
    m_node = std::move(p_ite.m_node);
    m_owner = std::move(p_ite.m_owner);
  }
  return *this;
}
template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element>::reference
chunked_container_iterator<t_chuncked_container, t_element>::

operator*() const {
  return m_owner->get_list_iterator(m_node.first)->at(m_node.second);
}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element>::pointer
chunked_container_iterator<t_chuncked_container, t_element>::operator->()
    const {
  return &m_owner->get_list_iterator(m_node.first)->at(m_node.second);
}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element> &
chunked_container_iterator<t_chuncked_container, t_element>::operator++() {
  if (array_pos() + 1 < m_owner->get_chunck_size()) {
    ++array_pos();
  } else {
    ++list_pos();
    array_pos() = 0;
  }
  return *this;
}
template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element>
chunked_container_iterator<t_chuncked_container, t_element>::

operator++(int) {
  auto _tmp = *this;
  ++(*this);
  return _tmp;
}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element> &
chunked_container_iterator<t_chuncked_container, t_element>::

operator--() {
  if (m_owner->is_list_end(list_pos())) {
    --list_pos();
    array_pos() = m_owner->get_chunck_size() - 1;
  } else if (array_pos() > 0) {
    --array_pos();
  } else {
    --list_pos();
    array_pos() = m_owner->get_chunck_size() - 1;
  }
  return *this;
}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element>
chunked_container_iterator<t_chuncked_container, t_element>::

operator--(int) {
  auto _tmp = *this;
  --(*this);
  return _tmp;
}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element>::list_position &
chunked_container_iterator<t_chuncked_container, t_element>::

    list_pos() {
  return m_node.first;
}

template <typename t_chuncked_container, typename t_element>
chunked_container_iterator<t_chuncked_container, t_element>::array_position &
chunked_container_iterator<t_chuncked_container, t_element>::

    array_pos() {
  return m_node.second;
}

} // namespace tnct::container::internal::dat

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_INTERNAL_DAT_CHUNKED_CONTAINER_TPP
#define TNCT_CONTAINER_INTERNAL_DAT_CHUNKED_CONTAINER_TPP

#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <optional>
#include <type_traits>
#include <utility>

#include "tnct/container/dat/chuncked_container.h"

namespace tnct::container::dat {

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::chunked_container(type p_default)
    : m_list{array{std::move(p_default)}}, m_list_current{0},
      m_array_current(1) {}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::chunked_container()
    : m_list{array{}}, m_list_current{0}, m_array_current(0) {}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::iterator
chunked_container<t_type, t_chunk_size>::begin() {
  return {{0, 0}, this};
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::iterator
chunked_container<t_type, t_chunk_size>::end() {
  return {{m_list_current, m_array_current}, this};
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::const_iterator
chunked_container<t_type, t_chunk_size>::begin() const {
  return {{0, 0}, this};
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::const_iterator
chunked_container<t_type, t_chunk_size>::end() const {
  return {{m_list_current, m_array_current}, this};
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::const_iterator
chunked_container<t_type, t_chunk_size>::cbegin() const {
  return begin();
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::const_iterator
chunked_container<t_type, t_chunk_size>::cend() const {
  return end();
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::iterator
chunked_container<t_type, t_chunk_size>::index2ite(std::size_t p_index) {
  const auto _list_index = p_index / t_chunk_size;
  const auto _array_index = p_index % t_chunk_size;

  if (_list_index >= m_list.size()) {
    return end();
  }

  return {{_list_index, static_cast<array_position>(_array_index)}, this};
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::const_iterator
chunked_container<t_type, t_chunk_size>::index2ite(std::size_t p_index) const {
  const auto _list_index = p_index / t_chunk_size;
  const auto _array_index = p_index % t_chunk_size;

  if (_list_index >= m_list.size()) {
    return cend();
  }

  return {{_list_index, static_cast<array_position>(_array_index)}, this};
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::const_list_iterator
chunked_container<t_type, t_chunk_size>::current_list_iterator() const {
  return std::next(m_list.cbegin(), m_list_current);
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::list_iterator
chunked_container<t_type, t_chunk_size>::current_list_iterator() {
  return std::next(m_list.begin(), m_list_current);
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::const_list_iterator
chunked_container<t_type, t_chunk_size>::get_list_iterator(
    list_position p_list_position) const {
  return std::next(m_list.cbegin(), p_list_position);
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
chunked_container<t_type, t_chunk_size>::list_iterator
chunked_container<t_type, t_chunk_size>::get_list_iterator(
    list_position p_list_position) {
  return std::next(m_list.begin(), p_list_position);
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
bool chunked_container<t_type, t_chunk_size>::is_list_end(
    list_position p_list_position) const {
  return p_list_position == m_list.size();
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
std::optional<typename chunked_container<t_type, t_chunk_size>::ref>
chunked_container<t_type, t_chunk_size>::operator[](std::size_t p_index) {
  iterator _ite{index2ite(p_index)};
  if (_ite == end()) {
    return std::nullopt;
  }
  return {*_ite};
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
std::optional<typename chunked_container<t_type, t_chunk_size>::const_ref>
chunked_container<t_type, t_chunk_size>::operator[](std::size_t p_index) const {
  const_iterator _ite{index2ite(p_index)};
  if (_ite == end()) {
    return std::nullopt;
  }
  return {*_ite};
};

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
void chunked_container<t_type, t_chunk_size>::remove(std::size_t p_index) {
  iterator _ite{index2ite(p_index)};
  if (_ite == end()) {
    return;
  }
  *_ite = std::nullopt;
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
void chunked_container<t_type, t_chunk_size>::remove(iterator p_ite) {
  if (p_ite.m_owner != this || p_ite == end()) {
    return;
  }
  *p_ite = std::nullopt;
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
typename chunked_container<t_type, t_chunk_size>::ref
chunked_container<t_type, t_chunk_size>::add(type &&p_object) {
  if (m_array_current == (t_chunk_size - 1)) {
    m_list.push_back(array{});
    ++m_list_current;
    m_array_current = 0;
  } /*else {
    ++m_array_current;
  }*/
  current_list_iterator()->at(m_array_current).emplace(std::move(p_object));
  ref _ref{current_list_iterator()->at(m_array_current)};
  ++m_array_current;
  return _ref;
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
void chunked_container<t_type, t_chunk_size>::erase(iterator p_ite) {
  if (p_ite.m_owner != this || p_ite == end()) {
    return;
  }
  *p_ite = std::nullopt;
}

template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_default_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)
void chunked_container<t_type, t_chunk_size>::erase(ref p_ref) {
  p_ref.get() = std::nullopt;
}

} // namespace tnct::container::dat

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_CHUNKED_CONTAINER_H
#define TNCT_CONTAINER_DAT_CHUNKED_CONTAINER_H

#include <array>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <limits>
#include <list>
#include <optional>
#include <type_traits>

#include "tnct/container/internal/dat/chuncked_container_iterator.h"

namespace tnct::container::dat {

/// A chuncked container
///
/// It guarantees that the address of each slot is not changed, even when
/// adding new elements or removing existing ones, allowing references to
/// objects to be reliable
///
/// \tparam t_type type that will be stored in the container
/// \tparam t_chunk_size number of t_class objects in each chunk
template <typename t_type, std::size_t t_chunk_size>
  requires(std::is_copy_constructible_v<std::remove_cvref_t<t_type>> &&
           std::is_move_constructible_v<std::remove_cvref_t<t_type>> &&
           t_chunk_size > 0)

struct chunked_container final {
  using type = std::remove_cvref_t<t_type>;
  using element = std::optional<type>;
  using ref = std::reference_wrapper<element>;
  using const_ref = std::reference_wrapper<const element>;
  static constexpr std::size_t chunck_size = t_chunk_size;

  friend class tnct::container::internal::dat::chunked_container_iterator<
      chunked_container, element>;
  friend class tnct::container::internal::dat::chunked_container_iterator<
      chunked_container, const element>;

  using iterator = tnct::container::internal::dat::chunked_container_iterator<
      chunked_container, element>;
  using const_iterator =
      tnct::container::internal::dat::chunked_container_iterator<
          chunked_container, const element>;

  chunked_container(type p_default = type{});

  chunked_container(const chunked_container &) = default;

  chunked_container(chunked_container &&) noexcept = default;

  ~chunked_container() = default;

  chunked_container &operator=(const chunked_container &) = default;

  chunked_container &operator=(chunked_container &&) noexcept = default;

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin() const;

  const_iterator cend() const;

  std::optional<ref> operator[](std::size_t p_index);

  std::optional<const_ref> operator[](std::size_t p_index) const;

  void remove(std::size_t p_index);

  void remove(iterator p_ite);

  ref add(type &&p_object);

  void erase(iterator p_ite);

  void erase(ref p_ref);

  constexpr std::size_t get_chunck_size() const { return t_chunk_size; }

private:
  using array = std::array<element, t_chunk_size>;

  using list = std::list<array>;

  using list_iterator = typename list::iterator;
  using const_list_iterator = typename list::const_iterator;

  using array_position = std::size_t;

  using list_position = std::size_t;

  iterator index2ite(std::size_t p_index);

  const_iterator index2ite(std::size_t p_index) const;

  const_list_iterator current_list_iterator() const;

  list_iterator current_list_iterator();

  const_list_iterator get_list_iterator(list_position p_list_position) const;

  list_iterator get_list_iterator(list_position p_list_position);

  bool is_list_end(list_position p_list_position) const;

private:
  list m_list;

  list_position m_list_current{std::numeric_limits<list_position>::max()};

  array_position m_array_current{std::numeric_limits<array_position>::max()};
};

} // namespace tnct::container::dat

#include "tnct/container/internal/dat/chuncked_container.tpp"

#endif

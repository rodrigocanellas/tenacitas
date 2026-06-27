/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_MULTI_INDEX_H
#define TNCT_CONTAINER_DAT_MULTI_INDEX_H

#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <tuple>
#include <utility>

#include "tnct/ostream/cpt/has_output_operator.h"
#include "tnct/pair/output.h"
#include "tnct/tuple/bus/traverse.h"

namespace tnct::container::cpt {
template <typename T>
concept less_than_comparable = requires(const T &a, const T &b) {
  { a < b } -> std::convertible_to<bool>;
};
} // namespace tnct::container::cpt

namespace tnct::container::dat {

/// Allows the creation of multiple indexes to a container
template <typename t_container, typename... t_key>
  requires((std::is_move_constructible_v<t_key>, ...) &&
           (cpt::less_than_comparable<t_key>, ...) &&
           (std::equality_comparable<t_key>, ...))

class multi_index_t final {

public:
  using container = t_container;
  using object = typename container::value_type;
  using iterator = typename container::iterator;
  using objects_const_iterator = typename container::const_iterator;
  using keys = std::tuple<t_key...>;

  multi_index_t() = delete;

  ~multi_index_t() = default;

  multi_index_t(std::function<std::optional<iterator>(object &&)> p_inserter,
                std::function<t_key(const object &)> &&...p_key_getters)
      : m_inserter{p_inserter},
        m_keys_getters{std::forward<std::function<t_key(const object &)>>(
            p_key_getters)...} {}

  void add(object &&p_object) {
    std::optional<iterator> _res{m_inserter(std::move(p_object))};

    if (!_res) {
      return;
    }

    auto _visitor = [&]<typename t_tuple, size_t t_index>(t_tuple &p_tuple) {
      using map = std::tuple_element_t<t_index, t_tuple>;

      map &_map{std::get<t_index>(p_tuple)};

      auto _key{std::get<t_index>(m_keys_getters)(*(_res.value()))};
      iterator _ite{_res.value()};
      _map.emplace(std::move(_key), std::move(_ite));

      return true;
    };

    tuple::bus::traverse<indexes, decltype(_visitor)>(m_indexes, _visitor);
  }

  template <std::size_t t_key_index>
  std::vector<iterator>
  get(const std::tuple_element_t<t_key_index, keys> &p_key) {
    using map = std::tuple_element_t<t_key_index, indexes>;
    using map_const_iterator = typename map::const_iterator;

    map &_map{std::get<t_key_index>(m_indexes)};

    std::pair<map_const_iterator, map_const_iterator> _range{
        _map.equal_range(p_key)};

    std::vector<iterator> _res;

    for (map_const_iterator _ite{_range.first}; _ite != _range.second; ++_ite) {
      if (_ite->second.has_value())
        _res.push_back(_ite->second.value());
    }

    std::sort(
        _res.begin(), _res.end(),
        [](const iterator &p_1, const iterator &p_2) { return *p_1 < *p_2; });
    return _res;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  multi_index_t &p_multi_index) {
    p_out << "\nindexes:\n";

    auto _visitor = [&]<typename t_tuple, size_t t_index>(t_tuple &p_tuple) {
      using map = std::tuple_element_t<t_index, t_tuple>;

      map &_map{std::get<t_index>(p_tuple)};

      for (const auto &_value : _map) {
        p_out << "\t{k = " << _value.first
              << ", obj = " << *(_value.second.value()) << "}";
      }
      p_out << '\n';
      return true;
    };
    tuple::bus::traverse<multi_index_t::indexes, decltype(_visitor)>(
        p_multi_index.m_indexes, _visitor);
    return p_out;
  }

private:
  using optional_iterator = std::optional<iterator>;
  using indexes = std::tuple<std::multimap<t_key, optional_iterator>...>;

  using keys_getters = std::tuple<std::function<t_key(const object &)>...>;

private:
  std::function<std::optional<iterator>(object &&)> m_inserter;
  keys_getters m_keys_getters;
  indexes m_indexes;
};

} // namespace tnct::container::dat

#endif

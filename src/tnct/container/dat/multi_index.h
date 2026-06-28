/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_MULTI_INDEX_H
#define TNCT_CONTAINER_DAT_MULTI_INDEX_H

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <ostream>
#include <tuple>
#include <utility>

#include "tnct/pair/output.h"
#include "tnct/tuple/bus/traverse.h"

/// \TODO this should be placed somewhere else
namespace tnct::container::cpt {
template <typename T>
concept less_than_comparable = requires(const T &a, const T &b) {
  { a < b } -> std::convertible_to<bool>;
};
} // namespace tnct::container::cpt

namespace tnct::container::dat {

/// Allows the creation of multiple indexes to a container
template <typename t_object, typename... t_keys>
  requires((std::is_move_constructible_v<t_keys>, ...) &&
           (cpt::less_than_comparable<t_keys>, ...) &&
           (std::equality_comparable<t_keys>, ...))

class multi_index_t final {

public:
  using object = t_object;

  using object_const_ref = std::reference_wrapper<const object>;
  using object_ref = std::reference_wrapper<object>;
  using keys = std::tuple<t_keys...>;

  /// Abstracts how to get a key value from an object for a key
  template <typename t_key>
  using key_getter = std::function<t_key(const object &)>;

  multi_index_t() = delete;

  ~multi_index_t() = default;

  multi_index_t(key_getter<t_keys>... p_key_getters)
      : m_keys_getters{std::forward<key_getter<t_keys>>(p_key_getters)...} {}

  void add(object &&p_object) {
    m_table.push_back({std::move(p_object)});

    table_iterator _table_ite{std::prev(m_table.end())};

    auto _visitor = [&]<typename t_tuple, size_t t_index>(t_tuple &p_indexes) {
      using key = std::tuple_element_t<t_index, keys>;

      key_getter _key_getter{std::get<t_index>(m_keys_getters)};
      index<key> &_index{std::get<t_index>(p_indexes)};

      const key _key{_key_getter(_table_ite->value())};

      index_iterator<key> _index_iterator{_index.emplace(_key, _table_ite)};

      return true;
    };

    tuple::bus::traverse<indexes, decltype(_visitor)>(m_indexes, _visitor);
  }

  template <std::size_t t_key_index>
  std::vector<object_const_ref>
  get(const std::tuple_element_t<t_key_index, keys> &p_key) {
    using key = std::tuple_element_t<t_key_index, keys>;
    index<key> &_index{std::get<t_key_index>(m_indexes)};

    std::pair<index_const_iterator<key>, index_const_iterator<key>> _range{
        _index.equal_range(p_key)};

    std::vector<object_const_ref> _res;

    for (index_const_iterator<key> _ite{_range.first}; _ite != _range.second;
         ++_ite) {
      if (_ite->second->has_value())
        _res.push_back(_ite->second->value());
    }

    std::sort(_res.begin(), _res.end(),
              [&](const object_const_ref &p_1, const object_const_ref &p_2) {
                return p_1.get() < p_2.get();
              });

    return _res;
  }

  template <std::size_t t_key_index>
  void erase(const std::tuple_element_t<t_key_index, keys> &p_key) {
    using key = std::tuple_element_t<t_key_index, keys>;
    index<key> &_index{std::get<t_key_index>(m_indexes)};

    std::pair<index_iterator<key>, index_iterator<key>> _range{
        _index.equal_range(p_key)};

    for (index_iterator<key> _ite{_range.first}; _ite != _range.second;
         ++_ite) {
      if (_ite->second->has_value()) {
        *(_ite->second) = std::nullopt;
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  multi_index_t &p_multi_index) {

    {
      std::cout << "############\nobjects:\n";
      std::for_each(p_multi_index.m_table.begin(), p_multi_index.m_table.end(),
                    [&](const std::optional<object> &p_object) {
                      if (p_object) {
                        std::cout << p_object.value() << " ";
                      } else {
                        std::cout << "DELETED ";
                      }
                    });
      std::cout << '\n';
    }
    {
      p_out << "############\nindexes:\n";
      auto _visitor = [&]<typename t_tuple, size_t t_index>(
                          t_tuple &p_indexes) {
        using key = std::tuple_element_t<t_index, multi_index_t::keys>;

        const multi_index_t::index<key> &_index{std::get<t_index>(p_indexes)};

        std::cout << "index " << t_index << " - ";
        for (const auto &_value : _index) {
          if (_value.second->has_value()) {
            p_out << "\t{k = " << _value.first
                  << ", obj = " << _value.second->value() << "}";
          } else {
            p_out << "\t{k = " << _value.first << ", obj = DELETED}";
          }
        }
        p_out << '\n';
        return true;
      };
      tuple::bus::traverse<multi_index_t::indexes, decltype(_visitor)>(
          p_multi_index.m_indexes, _visitor);
    }

    return p_out;
  }

private:
  using table = std::list<std::optional<object>>;

  using table_iterator = typename table::iterator;
  using table_const_iterator = typename table::const_iterator;

  /// An index for a key
  template <typename t_key> using index = std::multimap<t_key, table_iterator>;

  ///
  template <typename t_key>
  using index_const_iterator = typename index<t_key>::const_iterator;

  template <typename t_key>
  using index_iterator = typename index<t_key>::iterator;

  /// All indexes for all the keys
  using indexes = std::tuple<index<t_keys>...>;

  using keys_getters = std::tuple<std::function<t_keys(const object &)>...>;

private:
  table m_table;
  keys_getters m_keys_getters;
  indexes m_indexes;
  // objects_indexes m_objects_indexes;
};

} // namespace tnct::container::dat

#endif

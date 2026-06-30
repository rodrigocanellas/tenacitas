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

#include "tnct/generic/cpt/less_than_comparable.h"
#include "tnct/pair/output.h"
#include "tnct/tuple/bus/traverse.h"
#include "tnct/tuple/cpt/index_within_tuple.h"
#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/output.h"

namespace tnct::container::dat {

/// Allows the creation of multiple indexes to a container
template <tuple::cpt::is_tuple t_object, std::size_t... t_keys_pos>
  requires(std::is_unsigned_v<decltype(t_keys_pos)>, ...) &&
          (tuple::cpt::index_within_tuple<t_object, t_keys_pos>, ...) &&
          (generic::cpt::less_than_comparable<
               std::tuple_element_t<t_keys_pos, t_object>>,
           ...)
class multi_index_t final {

public:
  using object = std::optional<t_object>;

  using object_const_ref = std::reference_wrapper<const object>;
  using object_ref = std::reference_wrapper<object>;
  using keys_pos = std::tuple<decltype(t_keys_pos)...>;

  template <std::size_t t_key_pos>
  using key_t = std::tuple_element_t<t_key_pos, t_object>;

  multi_index_t() = default;
  multi_index_t(const multi_index_t &) = delete;
  multi_index_t(multi_index_t &&) = delete;

  ~multi_index_t() = default;

  multi_index_t &operator=(const multi_index_t &) = delete;
  multi_index_t &operator=(multi_index_t &&) = delete;

  void add(object &&p_object) {
    m_table.push_back({std::move(p_object)});

    object_ref _object_ref{*std::prev(m_table.end())};

    // table_iterator _table_ite{std::prev(m_table.end())};

    auto _visitor{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_key_pos>() {
      index<t_key_pos> &_index{std::get<t_key_pos>(m_indexes)};

      const key_t<t_key_pos> &_key{std::get<t_key_pos>(p_object.value())};

      // _index.emplace(_field, _table_ite);
      _index.emplace(_key, _object_ref);

      return true;
    }};

    tuple::bus::traverse<keys_pos, decltype(_visitor)>(_visitor);
  }

  template <std::size_t t_key_pos>
  std::vector<object_ref> get(const key_t<t_key_pos> &p_key) {

    index<t_key_pos> &_index{std::get<t_key_pos>(m_indexes)};

    std::pair<index_iterator<t_key_pos>, index_iterator<t_key_pos>> _range{
        _index.equal_range(p_key)};

    std::vector<object_ref> _res;

    for (index_iterator<t_key_pos> _ite{_range.first}; _ite != _range.second;
         ++_ite) {
      if (_ite->second.get().has_value())
        _res.push_back(_ite->second.get());
    }

    std::sort(_res.begin(), _res.end(),
              [&](const object_ref &p_1, const object_ref &p_2) {
                return p_1.get() < p_2.get();
              });

    return _res;
  }

  template <std::size_t t_key_pos> void erase(const key_t<t_key_pos> &p_key) {
    index<t_key_pos> &_index{std::get<t_key_pos>(m_indexes)};

    std::pair<index_iterator<t_key_pos>, index_iterator<t_key_pos>> _range{
        _index.equal_range(p_key)};

    for (index_iterator<t_key_pos> _ite{_range.first}; _ite != _range.second;
         ++_ite) {
      _ite->second.get() = std::nullopt;
    }
  }

  template <std::size_t t_field_pos>
  void update(object_ref &p_object,
              const std::tuple_element_t<t_field_pos, t_object> &p_value) {

    if (!p_object.get().has_value()) {
      return;
    }

    using field = std::tuple_element_t<t_field_pos, t_object>;

    bool _is_field_index{is_field_index<t_field_pos>()};

    const field _old_value{std::get<t_field_pos>(p_object.get().value())};

    std::get<t_field_pos>(p_object.get().value()) = p_value;

    if (_is_field_index) {
      index<t_field_pos> &_index{std::get<t_field_pos>(m_indexes)};

      auto _cmp{[&](const typename index<t_field_pos>::value_type
                        &p_index_value_type) {
        if (p_index_value_type.first == _old_value) {
          return true;
        }
        return false;
      }};

      index_iterator<t_field_pos> _ite{
          std::find_if(_index.begin(), _index.end(), _cmp)};

      if (_ite != _index.end()) {
        _index.erase(_ite);
        _index.emplace(p_value, p_object);
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  multi_index_t &p_multi_index) {

    {
      std::cout << "\nobjects:\n";
      std::for_each(p_multi_index.m_table.begin(), p_multi_index.m_table.end(),
                    [&](const object &p_object) {
                      if (p_object.has_value()) {
                        std::cout << p_object.value() << " ";
                      } else {
                        std::cout << "DELETED ";
                      }
                    });
      std::cout << '\n';
    }
    {
      p_out << "\nindexes:\n";
      auto _visitor = [&]<tuple::cpt::is_tuple t_tuple, size_t t_key_pos>(
                          t_tuple &p_indexes) {
        const multi_index_t::index<t_key_pos> &_index{
            std::get<t_key_pos>(p_indexes)};

        std::cout << "index " << t_key_pos << " - ";
        for (const auto &_value : _index) {
          if (_value.second.get().has_value()) {
            p_out << "\t{k = " << _value.first
                  << ", obj = " << _value.second.get().value() << "}";
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
  using table = std::list<object>;

  using table_iterator = typename table::iterator;
  using table_const_iterator = typename table::const_iterator;

  /// An index for a key
  template <std::size_t t_key_pos>
  using index = typename std::multimap<key_t<t_key_pos>, object_ref>;

  ///
  template <std::size_t t_key_pos>
  using index_const_iterator = typename index<t_key_pos>::const_iterator;

  template <std::size_t t_key_pos>
  using index_iterator = typename index<t_key_pos>::iterator;

  /// All indexes for all the keys
  using indexes = std::tuple<index<t_keys_pos>...>;

private:
  template <std::size_t t_field_pos> constexpr bool is_field_index() const {

    bool _is_field_index{false};

    auto _visitor{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_index>() {
      if (t_index == t_field_pos) {
        _is_field_index = true;
        return false;
      }
      return true;
    }};

    tuple::bus::traverse<keys_pos, decltype(_visitor)>(_visitor);

    return _is_field_index;
  }

private:
  table m_table;
  // keys_getters m_keys_getters;
  indexes m_indexes;
  // objects_indexes m_objects_indexes;
};

} // namespace tnct::container::dat

#endif

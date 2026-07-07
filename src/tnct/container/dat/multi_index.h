/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_MULTI_INDEX_H
#define TNCT_CONTAINER_DAT_MULTI_INDEX_H

#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <ostream>
#include <tuple>
#include <utility>

#include "tnct/container/cpt/field_definition.h"
#include "tnct/container/trt/index_traits.h"
#include "tnct/pair/output.h"
#include "tnct/tuple/bus/traverse.h"
#include "tnct/tuple/cpt/index_within_tuple.h"
#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/output.h"

namespace tnct::container::dat {

/// Allows the creation of multiple indexes to a container of (std::optional)
/// objects
///
/// \tparam t_object is some 'struct' or 'class'
/// \tparam t_fields_definitions defines the characteristics of each filed in
/// t_object, and if it will be used as an index
///
/// \details Each entry in the container is actually a std::optional<t_object>,
/// so it is up to the user to check if the reference to the
/// std::optional<t_object> that is retrieved is actually valid
///
/// std::optional<t_object> was used for two reasons:
/// 1 - the objects are not actually deleted, they are marked as std::nullopt to
/// optimize memory usage as well as to improve performance
/// 2 - in a multithread environment, say your code has a reference to the
/// std::optional<t_object> A, if another thread also has a reference, your code
/// can check if A has not become std::nullopt by another thread
///
/// \example tnct/container/exp/multi_index/main.cpp
template <typename t_object,
          cpt::field_definition<t_object>... t_fields_definitions>
  requires(
      (sizeof...(t_fields_definitions) > 0) &&
      (std::is_same_v<typename t_fields_definitions::object_type, t_object> &&
       ...))

class multi_index_t final {

public:
  struct record;

  using optional = std::optional<t_object>;

  using record_ref = std::reference_wrapper<record>;

  using fields_definitions = std::tuple<t_fields_definitions...>;

  template <std::size_t t_field_pos>
  using field_t = typename std::tuple_element_t<t_field_pos,
                                                fields_definitions>::field_type;

public:
  struct record final {
    friend class multi_index_t;

    record() = delete;

    record(const record &) = delete;
    record &operator=(const record &) = delete;

    record(record &&) = default;
    record &operator=(record &&) = default;

    void *operator new(std::size_t) = delete;
    void *operator new[](std::size_t) = delete;

    const optional &get_optional() const { return m_optional; }

    constexpr bool operator<(const record &p_record) const {
      return (m_optional.has_value()
                  ? m_optional.value() < p_record.m_optional.value()
                  : false);
    }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const record &p_record) {

      if (!p_record.m_optional.has_value()) {
        return p_out;
      }

      auto _visit = [&]<tuple::cpt::is_tuple t_tuple, std::size_t t_index>(
                        const t_tuple &p_tuple) {
        if constexpr (multi_index_t::is_iterator<t_index>()) {
          p_out << std::get<t_index>(p_tuple)->first << ' ';
        }
        return true;
      };

      p_out << "[ " << p_record.m_optional.value() << " ( ";

      tuple::bus::traverse<index_iterators, decltype(_visit)>(
          p_record.m_index_iterators, _visit);

      p_out << ")]";
      return p_out;
    }

  private:
    using index_iterators = std::tuple<typename trt::index_traits<
        typename t_fields_definitions::index_id,
        typename t_fields_definitions::field_type, record_ref>::iterator...>;

  private:
    record(t_object &&p_object) : m_optional{std::move(p_object)} {}

    void set_indexes(index_iterators &&p_index_iterators) {
      m_index_iterators = std::move(p_index_iterators);
    }

    optional &get_internal_optional() { return m_optional; }

    index_iterators &get_index_iterators() { return m_index_iterators; }

  private:
    optional m_optional;
    index_iterators m_index_iterators;
  };

  multi_index_t() = default;
  multi_index_t(const multi_index_t &) = delete;
  multi_index_t(multi_index_t &&) = delete;

  ~multi_index_t() = default;

  multi_index_t &operator=(const multi_index_t &) = delete;
  multi_index_t &operator=(multi_index_t &&) = delete;

  void add(t_object &&p_object) {

    m_table.push_back(std::move(p_object));

    record_ref _record_ref{*std::prev(m_table.end())};

    typename record::index_iterators _index_iterators{create_index_iterators()};

    bool _error{false};
    auto _visitor{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_field_pos>() {
      if constexpr (is_index<t_field_pos>()) {
        using index = index_t<t_field_pos>;
        using index_iterator = index_iterator_t<t_field_pos>;
        using field = field_t<t_field_pos>;
        using field_getter = field_getter_t<t_field_pos>;

        index &_index{std::get<t_field_pos>(m_indexes)};

        const field _field{
            field_getter{}(_record_ref.get().get_optional().value())};

        index_iterator _ite{_index.find(_field)};
        if ((_ite == _index.end())) {
          std::pair<index_iterator, bool> _res{
              _index.emplace(_field, _record_ref)};
          if (_res.second) {
            std::get<t_field_pos>(_index_iterators) = _res.first;
          } else {
            _error = true;
            return false;
          }
        } else if (!_ite->second.get().get_optional().has_value()) {
          _ite->second = _record_ref;
          std::get<t_field_pos>(_index_iterators) = _ite;
        } else if (_ite->second.get().get_optional().value() !=
                   _record_ref.get().get_optional().value()) {
          std::pair<index_iterator, bool> _res{
              _index.emplace(_field, _record_ref)};
          if (_res.second) {
            std::get<t_field_pos>(_index_iterators) = _res.first;
          } else {
            _error = true;
            return false;
          }
        }
      }
      return true;
    }};

    tuple::bus::traverse<indexes, decltype(_visitor)>(_visitor);

    if (!_error) {
      _record_ref.get().set_indexes(std::move(_index_iterators));
    } else {
      erase(_index_iterators);
      m_table.erase(std::prev(m_table.end()));
    }
  }

  template <std::size_t t_field_pos>
    requires(tuple::cpt::index_within_tuple<fields_definitions, t_field_pos>)
  std::vector<record_ref> get(const field_t<t_field_pos> &p_field) {
    std::vector<record_ref> _res;
    if constexpr (is_index<t_field_pos>()) {
      _res = get_index<t_field_pos>(p_field);
    } else {
      _res = get_field<t_field_pos>(p_field);
    }
    if (!_res.empty()) {
      std::sort(_res.begin(), _res.end(),
                [&](const record_ref &p_1, const record_ref &p_2) {
                  return p_1.get() < p_2.get();
                });
    }

    return std::move(_res);
  }

  template <std::size_t t_field_pos>
    requires(tuple::cpt::index_within_tuple<fields_definitions, t_field_pos>)
  void erase(const field_t<t_field_pos> &p_field) {

    if constexpr (is_index<t_field_pos>()) {
      erase_index<t_field_pos>(p_field);
    } else {
      update_field<t_field_pos>(p_field);
    }
  }

  template <std::size_t t_field_pos>
    requires(tuple::cpt::index_within_tuple<fields_definitions, t_field_pos>)
  void update(record_ref p_record_ref, const field_t<t_field_pos> &p_field) {

    if (!p_record_ref.get().get_optional().has_value()) {
      return;
    }

    if constexpr (is_index<t_field_pos>()) {
      update_index<t_field_pos>(p_record_ref, p_field);
    } else {
      field_setter_t<t_field_pos>{}(
          p_record_ref.get().get_internal_optional().value(), p_field);
    }
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  multi_index_t &p_multi_index) {

    {
      std::cout << "\nobjects:\n";
      std::for_each(p_multi_index.m_table.begin(), p_multi_index.m_table.end(),
                    [&](const record &p_record) {
                      if (p_record.get_optional().has_value()) {
                        p_out << p_record << " ";
                      } else {
                        std::cout << "DELETED";
                      }
                      p_out << '\n';
                    });
      std::cout << '\n';
    }
    {
      p_out << "\nindexes:\n";
      auto _visitor = [&]<tuple::cpt::is_tuple t_tuple, size_t t_index_pos>(
                          t_tuple &p_indexes) {
        if constexpr (is_index<t_index_pos>()) {
          const multi_index_t::index_t<t_index_pos> &_index{
              std::get<t_index_pos>(p_indexes)};

          std::cout << "index " << t_index_pos << " - ";
          for (const auto &_value : _index) {
            if (_value.second.get().get_optional().has_value()) {
              p_out << "\t{k = " << _value.first
                    << ", obj = " << _value.second.get().get_optional().value()
                    << "}";
            } else {
              p_out << "\t{k = " << _value.first << ", obj = DELETED}";
            }
          }
          p_out << '\n';
        }
        return true;
      };
      tuple::bus::traverse<multi_index_t::indexes, decltype(_visitor)>(
          p_multi_index.m_indexes, _visitor);
    }

    return p_out;
  }

private:
  /// \todo this will be replaced with something like
  /// std::list<std::array<object,N>>
  using table = std::list<record>;

  using indexes = std::tuple<typename trt::index_traits<
      typename t_fields_definitions::index_id,
      typename t_fields_definitions::field_type, record_ref>::index...>;

  template <std::size_t t_field_pos>
  using index_t = std::tuple_element_t<t_field_pos, indexes>;

  template <std::size_t t_field_pos>
  using index_iterator_t =
      std::tuple_element_t<t_field_pos, typename record::index_iterators>;

  template <std::size_t t_field_pos>
  using field_getter_t =
      typename std::tuple_element_t<t_field_pos,
                                    fields_definitions>::field_getter;

  template <std::size_t t_field_pos>
  using field_setter_t =
      typename std::tuple_element_t<t_field_pos,
                                    fields_definitions>::field_setter;

private:
  template <std::size_t t_field_pos> static constexpr bool is_index() {
    return !std::is_same_v<std::tuple_element_t<t_field_pos, indexes>,
                           trt::no_index_type>;
  }

  template <std::size_t t_field_pos> static constexpr bool is_iterator() {
    return !std::is_same_v<
        std::tuple_element_t<t_field_pos, typename record::index_iterators>,
        trt::no_iterator_type>;
  }

  template <std::size_t t_field_pos>
  std::optional<index_iterator_t<t_field_pos>>
  exists(const field_t<t_field_pos> &p_key) {
    using index_iterator = index_iterator_t<t_field_pos>;
    using index = index_t<t_field_pos>;

    index &_index{std::get<t_field_pos>(m_indexes)};

    if (index_iterator _ite{_index.find(p_key)}; _ite == _index.end()) {
      return {_ite};
    }
    return std::nullopt;
  }

  template <std::size_t t_field_pos>
  void update_index(record_ref p_record_ref,
                    const field_t<t_field_pos> &p_field) {
    optional &_optional{p_record_ref.get().get_internal_optional()};

    field_setter_t<t_field_pos> _field_setter;

    t_object _copy{_optional.value()};
    _field_setter(_copy, p_field);

    auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>(
                    t_tuple &p_index_iterators) {
      if constexpr (is_index<t_pos>()) {
        using index = std::tuple_element_t<t_pos, indexes>;
        using index_iterator =
            std::tuple_element_t<t_pos, typename record::index_iterators>;
        using field_getter = field_getter_t<t_pos>;
        using field = field_t<t_pos>;

        field_getter _field_getter;

        if (const field _field{_field_getter(_optional.value())};
            _field != _field_getter(_copy)) {
          index_iterator _index_iterator{std::get<t_pos>(p_index_iterators)};
          index &_index{std::get<t_pos>(m_indexes)};
          _field_setter(_optional.value(), p_field);
          _index.erase(_index_iterator);
          std::pair<index_iterator, bool> _res{
              _index.emplace(_field_getter(_optional.value()), p_record_ref)};
          if (_res.second) {
            std::get<t_pos>(p_index_iterators) = _res.first;
          }
        }
      }

      return true;
    }};

    tuple::bus::traverse<typename record::index_iterators, decltype(_visit)>(
        p_record_ref.get().get_index_iterators(), _visit);
  }

  template <std::size_t t_field_pos>
  void erase_index(const field_t<t_field_pos> &p_field) {

    if constexpr (is_index<t_field_pos>()) {
      using index = index_t<t_field_pos>;

      index &_index{std::get<t_field_pos>(m_indexes)};
      using index_iterator = index_iterator_t<t_field_pos>;

      std::pair<index_iterator, index_iterator> _range{
          _index.equal_range(p_field)};

      for (index_iterator _ite{_range.first}; _ite != _range.second; ++_ite) {
        _ite->second.get().get_internal_optional().reset();
      }
    }
  }

  template <std::size_t t_field_pos>
  void update_field(const field_t<t_field_pos> &p_field) {

    for (record &_record : m_table) {
      if (const optional & _optional{_record.get_optional()};
          _optional.has_value() &&
          (p_field == field_getter_t<t_field_pos>{}(_optional.value()))) {
        _record.get_internal_optional().reset();
      }
    }
  }

  template <std::size_t t_field_pos>
  std::vector<record_ref> get_index(const field_t<t_field_pos> &p_field) {
    using index = index_t<t_field_pos>;
    using index_iterator = index_iterator_t<t_field_pos>;

    std::vector<record_ref> _res;

    index &_index{std::get<t_field_pos>(m_indexes)};

    std::pair<index_iterator, index_iterator> _range{
        _index.equal_range(p_field)};

    for (index_iterator _ite{_range.first}; _ite != _range.second; ++_ite) {
      if (_ite->second.get().get_optional().has_value())
        _res.push_back(record_ref{_ite->second.get()});
    }
    return std::move(_res);
  }

  template <std::size_t t_field_pos>
  std::vector<record_ref> get_field(const field_t<t_field_pos> &p_field) {
    std::vector<record_ref> _res;
    for (record &_record : m_table) {
      if (const optional & _optional{_record.get_optional()};
          _optional.has_value() &&
          (p_field == field_getter_t<t_field_pos>{}(_optional.value()))) {
        _res.push_back(record_ref{_record});
      }
    }
    return std::move(_res);
  }

  void erase(typename record::index_iterators &p_index_iterators) {

    auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
      if constexpr (is_index<t_pos>()) {

        using index = index_t<t_pos>;
        using index_iterator = index_iterator_t<t_pos>;

        index &_index{std::get<t_pos>(m_indexes)};
        index_iterator _index_iterator{std::get<t_pos>(p_index_iterators)};

        if (_index_iterator != _index.end()) {
          _index.erase(_index_iterator);
        }
      }
      return true;
    }};

    tuple::bus::traverse<typename record::index_iterators, decltype(_visit)>(
        _visit);
  }

  typename record::index_iterators create_index_iterators() {

    typename record::index_iterators _index_iterators;

    auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
      if constexpr (is_index<t_pos>()) {
        using index = index_t<t_pos>;

        index &_index{std::get<t_pos>(m_indexes)};
        std::get<t_pos>(_index_iterators) = _index.end();
      }

      return true;
    }};

    tuple::bus::traverse<typename record::index_iterators, decltype(_visit)>(
        _visit);

    return _index_iterators;
  }

private:
  table m_table;
  indexes m_indexes;
};

} // namespace tnct::container::dat

#endif

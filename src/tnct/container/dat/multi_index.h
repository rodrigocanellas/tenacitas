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

#include "tnct/container/cpt/index_definition.h"
#include "tnct/container/trt/index_type.h"
#include "tnct/container/trt/object_indexed.h"
#include "tnct/pair/output.h"
#include "tnct/tuple/bus/traverse.h"
#include "tnct/tuple/cpt/index_within_tuple.h"
#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/output.h"

namespace tnct::container::dat {

/// Allows the creation of multiple indexes to a container of (std::optional)
/// objects
///
/// \tparam t_object is a std::tuple with the data record to be indexed
/// \tparam t_indexes_definitions defines the characteristics of each index used
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
/// can check if A is not std::nullopt
///
/// \example tnct/container/exp/multi_index/main.cpp
template <typename t_object,
          cpt::index_definition<t_object>... t_indexes_definitions>
  requires(
      (sizeof...(t_indexes_definitions) > 0) &&
      (std::is_same_v<typename t_indexes_definitions::object_type, t_object> &&
       ...))

class multi_index_t final {

public:
  using object = trt::object_indexed<t_object>::type;

  using optional = trt::object_indexed<t_object>::optional;

  struct record;

  using record_ref = std::reference_wrapper<record>;

private:
  using indexes =
      std::tuple<trt::index_type_t<typename t_indexes_definitions::index_id,
                                   typename t_indexes_definitions::key_type,
                                   record_ref>...>;
  template <std::size_t t_index_pos>
  using index_t = std::tuple_element_t<t_index_pos, indexes>;

  template <std::size_t t_index_pos>
  using key_t = typename index_t<t_index_pos>::key_type;

  template <std::size_t t_index_pos>
  using index_iterator_t = typename index_t<t_index_pos>::iterator;

  template <std::size_t t_index_pos>
  using index_const_iterator_t = typename index_t<t_index_pos>::const_iterator;

public:
  using index_iterators = std::tuple<typename trt::index_type_t<
      typename t_indexes_definitions::index_id,
      typename t_indexes_definitions::key_type, record_ref>::iterator...>;

  struct record final {
    friend class multi_index_t;

    record() = delete;

    record(object &&p_object) : m_optional{std::move(p_object)} {}

    record(const record &) = default;
    record(record &) = default;
    record &operator=(const record &) = default;
    record &operator=(record &) = default;
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
        p_out << std::get<t_index>(p_tuple)->first << ' ';
        return true;
      };

      p_out << "[ " << p_record.m_optional.value() << " ( ";

      tuple::bus::traverse<index_iterators, decltype(_visit)>(
          p_record.m_index_iterators, _visit);

      p_out << ")]";
      return p_out;
    }

  private:
    void set_indexes(index_iterators &&p_index_iterators) {
      m_index_iterators = std::move(p_index_iterators);
    }

    optional &get_internal_optional() { return m_optional; }

    index_iterators &get_index_iterators() { return m_index_iterators; }

    optional m_optional;
    index_iterators m_index_iterators;
  };

  multi_index_t() = default;
  multi_index_t(const multi_index_t &) = delete;
  multi_index_t(multi_index_t &&) = delete;

  ~multi_index_t() = default;

  multi_index_t &operator=(const multi_index_t &) = delete;
  multi_index_t &operator=(multi_index_t &&) = delete;

  void add(object &&p_object) {
    m_table.push_back({std::move(p_object)});

    record_ref _record_ref{*std::prev(m_table.end())};

    index_iterators _index_iterators;

    auto _visitor{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_index_pos>() {
      index_t<t_index_pos> &_index{std::get<t_index_pos>(m_indexes)};
      const key_t<t_index_pos> _key{key_getter_t<t_index_pos>()(
          _record_ref.get().get_optional().value())};
      index_iterator_t<t_index_pos> _ite{_index.insert({_key, _record_ref})};

      std::get<t_index_pos>(_index_iterators) = _ite;

      return true;
    }};

    tuple::bus::traverse<indexes, decltype(_visitor)>(_visitor);

    _record_ref.get().set_indexes(std::move(_index_iterators));
  }

  template <std::size_t t_index_pos>
    requires(tuple::cpt::index_within_tuple<indexes, t_index_pos>)
  std::vector<record_ref> get(const key_t<t_index_pos> &p_key) {

    using index = index_t<t_index_pos>;
    using index_iterator = index_iterator_t<t_index_pos>;

    index &_index{std::get<t_index_pos>(m_indexes)};

    std::pair<index_iterator, index_iterator> _range{_index.equal_range(p_key)};

    std::vector<record_ref> _res;

    for (index_iterator _ite{_range.first}; _ite != _range.second; ++_ite) {
      if (_ite->second.get().get_optional().has_value())
        _res.push_back(_ite->second.get());
    }

    std::sort(_res.begin(), _res.end(),
              [&](const record_ref &p_1, const record_ref &p_2) {
                return p_1.get() < p_2.get();
              });

    return _res;
  }

  template <std::size_t t_index_pos>
    requires(tuple::cpt::index_within_tuple<indexes, t_index_pos>)
  void erase(const key_t<t_index_pos> &p_key) {

    using index = index_t<t_index_pos>;

    index &_index{std::get<t_index_pos>(m_indexes)};
    using index_iterator = index_iterator_t<t_index_pos>;

    std::pair<index_iterator, index_iterator> _range{_index.equal_range(p_key)};

    for (index_iterator _ite{_range.first}; _ite != _range.second; ++_ite) {
      _ite->second.get().get_internal_optional().reset();
    }
  }

  void update(record_ref p_record_ref, std::function<void(object &)> p_update) {
    if (!p_record_ref.get().get_optional().has_value()) {
      return;
    }

    optional &_optional{p_record_ref.get().get_internal_optional()};

    t_object _copy{_optional.value()};
    p_update(_copy);

    auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>(
                    t_tuple &p_index_iterators) {
      using index = std::tuple_element_t<t_pos, indexes>;
      using index_iterator = std::tuple_element_t<t_pos, index_iterators>;
      using key_getter = key_getter_t<t_pos>;
      using key = key_t<t_pos>;

      key_getter _key_getter;

      if (const key _key{_key_getter(_optional.value())};
          _key != _key_getter(_copy)) {
        index_iterator _index_iterator{std::get<t_pos>(p_index_iterators)};
        index &_index{std::get<t_pos>(m_indexes)};
        p_update(_optional.value());
        _index.erase(_index_iterator);
        index_iterator _new{
            _index.insert({_key_getter(_optional.value()), p_record_ref})};
        std::get<t_pos>(p_index_iterators) = _new;
      }

      return true;
    }};

    tuple::bus::traverse<index_iterators, decltype(_visit)>(
        p_record_ref.get().get_index_iterators(), _visit);
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

  using indexes_definitions = std::tuple<t_indexes_definitions...>;

  template <std::size_t t_index_pos>
  using key_getter_t =
      typename std::tuple_element_t<t_index_pos,
                                    indexes_definitions>::key_getter;

private:
  table m_table;
  indexes m_indexes;
};

} // namespace tnct::container::dat

#endif

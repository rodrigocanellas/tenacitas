/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_MULTI_INDEX_H
#define TNCT_CONTAINER_DAT_MULTI_INDEX_H

#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <mutex>
#include <optional>
#include <ostream>
#include <tuple>
#include <utility>

#include "tnct/container/cpt/field_definition.h"
#include "tnct/container/trt/fields_definitions_are_compatible.h"
#include "tnct/container/trt/no_index_trait.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/pair/output.h"
#include "tnct/tuple/bus/traverse.h"
#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/output.h"

namespace tnct::container::dat {

/// Allows the creation of multiple indexes to a container of (std::optional)
/// objects
///
/// \tparam t_logger is used to log messages
///
/// \tparam t_fields_definitions defines the characteristics of each field in
/// t_object
///
/// \details Each entry in the container is actually a std::optional<t_object>,
/// so it is up to the user to check if the reference to the
/// std::optional<t_object> that is retrieved is actually valid
///
/// \details You can use the tnct::container::trt::attribute_definition,
/// tnct::container::trt::index_definition and
/// tnct::container::trt::calculated_index_definition to define the fields used
/// in \p multi_index_t
///
/// std::optional<t_object> was used for two reasons:
/// 1 - the objects are not actually deleted, they are marked as std::nullopt to
/// optimize memory usage as well as to improve performance
/// 2 - in a multithread environment, say your code has a reference to the
/// std::optional<t_object> A, if another thread also has a reference, your code
/// can check if A has not become std::nullopt by another thread
///
/// \example tnct/container/exp/multi_index/main.cpp
template <log::cpt::logger t_loggerr,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
class multi_index_t final {

public:
  using logger = t_loggerr;

  using fields_definitions = std::tuple<t_fields_definitions...>;

  /// Object that will be multi indexed
  using object =
      typename std::tuple_element_t<0, fields_definitions>::object_type;

  /// An \p std::optional is actually multi indexed to allow check if the \p
  /// object is valid
  using optional = std::optional<object>;

  /// This class gives acess to the \p std::optional<record>
  struct record;

  /// Actually it is a reference to the \p record
  using record_ref = std::reference_wrapper<record>;

  /// Type of the \p t_field_pos-th field
  template <std::size_t t_field_pos>
  using field_t = typename std::tuple_element_t<t_field_pos,
                                                fields_definitions>::field_type;

public:
  /// Allows controlled access to the \p std::optional that contains (or not)
  /// the object being multi indexed
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
          p_out << std::get<t_index>(p_tuple).value()->first << ' ';
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
    using index_iterators =
        std::tuple<std::optional<typename container::trt::index_traits<
            typename t_fields_definitions::index_traits_id,
            typename t_fields_definitions::field_type,
            record_ref>::iterator>...>;

    template <std::size_t t_field_pos>
    using index_iterator_t = std::tuple_element_t<t_field_pos, index_iterators>;

  private:
    record(object &&p_object);

    void set_indexes(index_iterators &&p_index_iterators) {
      m_index_iterators = std::move(p_index_iterators);
    }

    template <std::size_t t_index_pos>
    void set_index_iterator(index_iterator_t<t_index_pos> p_iterator) {
      std::get<t_index_pos>(m_index_iterators) = p_iterator;
    }

    optional &get_internal_optional() { return m_optional; }

    index_iterators &get_index_iterators() { return m_index_iterators; }

    void reset_index_iterators();

  private:
    optional m_optional;
    index_iterators m_index_iterators;
  };

  // end of \p record class

  multi_index_t(std::reference_wrapper<logger> p_logger) : m_logger(p_logger) {}

  multi_index_t(const multi_index_t &) = delete;
  multi_index_t(multi_index_t &&) = delete;

  ~multi_index_t() = default;

  multi_index_t &operator=(const multi_index_t &) = delete;
  multi_index_t &operator=(multi_index_t &&) = delete;

  /// Adds a \p object and returns a std::optional reference with (or not) a
  /// reference to the record
  std::optional<record_ref> add(object &&p_object);

  /// Retrieves a collection of references to \p record
  ///
  /// \tparam t_field_pos is the field used to retrieve the references to \p
  /// record
  ///
  /// \param p_field is the value of the field to be used as comparision
  ///
  /// \return collection of references to \p record
  ///
  /// \note if there is an index associated to t_field_pos, it will be used to
  /// retrive the references to \p record
  ///
  /// \note if the index associated to t_field_pos is an index that does not
  /// allow repetition, the number of references to \p record in the return is
  /// at most 1
  template <std::size_t t_field_pos>
  std::vector<record_ref> get(const field_t<t_field_pos> &p_field);

  /// Erase all the references to \p record that have a certain field value
  ///
  /// \tparam t_field_pos is the field used to retrieve the references to \p
  /// record
  ///
  /// \param p_field is the value of the field to be used as comparision
  template <std::size_t t_field_pos>
  void erase(const field_t<t_field_pos> &p_field);

  /// Updates a certain field of a record
  ///
  /// \tparam t_field_pos is the field used to retrieve the references to \p
  /// record
  ///
  /// \param p_record_ref is a reference to the \p record being modified
  ///
  /// \param p_field is the new value of the field
  ///
  /// \note if there is an index associated to t_field_pos, the entry in the
  /// index will be updated
  template <std::size_t t_field_pos>
  bool update(record_ref p_record_ref, const field_t<t_field_pos> &p_field);

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
  using table_iterator = typename table::iterator;

private:
  using indexes = std::tuple<typename container::trt::index_traits<
      typename t_fields_definitions::index_traits_id,
      typename t_fields_definitions::field_type, record_ref>::index...>;

  template <std::size_t t_field_pos>
  using index_t = std::tuple_element_t<t_field_pos, indexes>;

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
        typename std::tuple_element_t<
            t_field_pos, typename record::index_iterators>::value_type,
        trt::no_iterator_type>;
  }

  template <std::size_t t_field_pos>
  static constexpr bool is_calculated_index() {
    return is_index<t_field_pos> &&
           std::tuple_element_t<t_field_pos, fields_definitions>::is_calculated;
  }

  template <std::size_t t_field_pos>
  bool update_index(record_ref p_record_ref,
                    const field_t<t_field_pos> &p_field);

  bool update_calculated_indexes(record_ref p_record_ref);

  template <std::size_t t_field_pos>
  void erase_by_index(const field_t<t_field_pos> &p_field);

  template <std::size_t t_field_pos>
  void erase_by_field(const field_t<t_field_pos> &p_field);

  template <std::size_t t_field_pos>
  std::vector<record_ref> get_by_index(const field_t<t_field_pos> &p_field);

  template <std::size_t t_field_pos>
  std::vector<record_ref> get_by_field(const field_t<t_field_pos> &p_field);

  void erase_indexes(typename record::index_iterators &p_index_iterators);

  void erase_record(record_ref p_record_ref);

private:
  std::reference_wrapper<logger> m_logger;
  table m_table;
  indexes m_indexes;
  std::mutex m_mutex;
};

// #############################################################################
// implementation
// #############################################################################
template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
std::optional<
    typename multi_index_t<t_logger, t_fields_definitions...>::record_ref>
multi_index_t<t_logger, t_fields_definitions...>::

    add(typename multi_index_t<t_logger, t_fields_definitions...>::object
            &&p_object) {

  std::lock_guard<std::mutex> _lock{m_mutex};

  m_table.push_back({std::move(p_object)});

  record_ref _record_ref{*std::prev(m_table.end())};

  bool _error{false};
  auto _visitor{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_field_pos>() {
    if constexpr (is_index<t_field_pos>()) {
      using index = index_t<t_field_pos>;
      using index_iterator =
          typename record::template index_iterator_t<t_field_pos>;
      using field = field_t<t_field_pos>;
      using field_getter = field_getter_t<t_field_pos>;

      index &_index{std::get<t_field_pos>(m_indexes)};

      const field _field{
          field_getter{}(_record_ref.get().get_optional().value())};

      std::pair<typename index_iterator::value_type, bool> _res{
          _index.emplace(_field, _record_ref)};
      if (_res.second) {
        // index creation ok
        _record_ref.get().template set_index_iterator<t_field_pos>(
            {_res.first});
      } else {
        // index creation not ok
        _error = true;
        return false;
      }
    }
    return true;
  }};

  tuple::bus::traverse<indexes, decltype(_visitor)>(_visitor);

  if (_error) {
    erase_record(_record_ref);
    return std::nullopt;
  }
  return {_record_ref};
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
std::vector<
    typename multi_index_t<t_logger, t_fields_definitions...>::record_ref>
multi_index_t<t_logger, t_fields_definitions...>::

    get(const field_t<t_field_pos> &p_field) {

  std::vector<record_ref> _res;
  if constexpr (is_index<t_field_pos>()) {
    _res = get_by_index<t_field_pos>(p_field);
  } else {
    _res = get_by_field<t_field_pos>(p_field);
  }
  if (!_res.empty()) {
    std::sort(_res.begin(), _res.end(),
              [&](const record_ref &p_1, const record_ref &p_2) {
                return p_1.get() < p_2.get();
              });
  }

  return _res;
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
void multi_index_t<t_logger, t_fields_definitions...>::

    erase(const field_t<t_field_pos> &p_field) {
  std::lock_guard<std::mutex> _lock{m_mutex};
  if constexpr (is_index<t_field_pos>()) {
    erase_by_index<t_field_pos>(p_field);
  } else {
    erase_by_field<t_field_pos>(p_field);
  }
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
bool multi_index_t<t_logger, t_fields_definitions...>::

    update(record_ref p_record_ref, const field_t<t_field_pos> &p_field) {

  if (!p_record_ref.get().get_optional().has_value()) {
    return false;
  }

  std::lock_guard<std::mutex> _lock{m_mutex};

  bool _ok{true};
  if constexpr (is_index<t_field_pos>()) {
    _ok = update_index<t_field_pos>(p_record_ref, p_field);
  } else {
    field_setter_t<t_field_pos>{}(
        p_record_ref.get().get_internal_optional().value(), p_field);
  }

  return (!_ok ? _ok : update_calculated_indexes(p_record_ref));
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
bool multi_index_t<t_logger, t_fields_definitions...>::

    update_index(record_ref p_record_ref, const field_t<t_field_pos> &p_field) {

  record &_record = p_record_ref.get();

  typename record::index_iterators &_index_iterators =
      _record.get_index_iterators();

  index_t<t_field_pos> &_index = std::get<t_field_pos>(m_indexes);

  if (std::get<t_field_pos>(_index_iterators).value()->first != p_field) {
    auto [_new_ite, _inserted] = _index.emplace(p_field, p_record_ref);

    if (!_inserted) {
      return false;
    }

    object &_object = _record.get_internal_optional().value();

    _index.erase(std::get<t_field_pos>(_index_iterators).value());

    std::get<t_field_pos>(_index_iterators) = _new_ite;

    field_setter_t<t_field_pos>{}(_object, p_field);
  }

  return true;
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
bool multi_index_t<t_logger, t_fields_definitions...>::

    update_calculated_indexes(record_ref p_record_ref) {
  bool _ok{true};
  auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
    if constexpr (is_calculated_index<t_pos>()) {
      using field_getter = field_getter_t<t_pos>;
      field_getter _field_getter;
      if (!update_index<t_pos>(
              p_record_ref.get(),
              _field_getter(
                  p_record_ref.get().get_internal_optional().value()))) {
        _ok = false;
        return false;
      }
    }
    return true;
  }

  };
  tuple::bus::traverse<indexes, decltype(_visit)>(_visit);

  return _ok;
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
void multi_index_t<t_logger, t_fields_definitions...>::

    erase_by_index(const field_t<t_field_pos> &p_field) {
  if constexpr (is_index<t_field_pos>()) {
    using index = index_t<t_field_pos>;

    index &_index{std::get<t_field_pos>(m_indexes)};
    using index_iterator =
        typename record::template index_iterator_t<t_field_pos>;

    std::pair<typename index_iterator::value_type, index_iterator> _range{
        _index.equal_range(p_field)};

    if (_range.first == _range.second) {
      return;
    }
    typename index_iterator::value_type _ite{_range.first};
    while (true) {
      typename index_iterator::value_type _aux = _ite;
      ++_ite;
      erase_record(_aux->second);

      if (_ite == _range.second) {
        break;
      }
    }
  }
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
void multi_index_t<t_logger, t_fields_definitions...>::

    erase_by_field(const field_t<t_field_pos> &p_field) {
  using field_getter = field_getter_t<t_field_pos>;
  for (record &_record : m_table) {
    if (optional & _optional{_record.get_internal_optional()};
        _optional.has_value() &&
        (p_field == field_getter{}(_optional.value()))) {
      erase_record(_record);
    }
  }
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
void multi_index_t<t_logger, t_fields_definitions...>::

    erase_record(record_ref p_record_ref) {
  erase_indexes(p_record_ref.get().get_index_iterators());
  p_record_ref.get().reset_index_iterators();
  p_record_ref.get().get_internal_optional().reset();
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
void multi_index_t<t_logger, t_fields_definitions...>::

    erase_indexes(typename record::index_iterators &p_index_iterators) {
  auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
    if constexpr (is_index<t_pos>()) {

      using index = index_t<t_pos>;
      using index_iterator = typename record::template index_iterator_t<t_pos>;

      index &_index{std::get<t_pos>(m_indexes)};
      index_iterator _index_iterator{std::get<t_pos>(p_index_iterators)};

      if (_index_iterator.has_value()) {
        _index.erase(_index_iterator.value());
      }
    }
    return true;
  }};

  tuple::bus::traverse<typename record::index_iterators, decltype(_visit)>(
      _visit);
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
std::vector<
    typename multi_index_t<t_logger, t_fields_definitions...>::record_ref>
multi_index_t<t_logger, t_fields_definitions...>::

    get_by_field(const field_t<t_field_pos> &p_field) {
  std::vector<record_ref> _res;
  for (record &_record : m_table) {
    if (const optional & _optional{_record.get_optional()};
        _optional.has_value() &&
        (p_field == field_getter_t<t_field_pos>{}(_optional.value()))) {
      _res.push_back(record_ref{_record});
    }
  }
  return _res;
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
std::vector<
    typename multi_index_t<t_logger, t_fields_definitions...>::record_ref>
multi_index_t<t_logger, t_fields_definitions...>::

    get_by_index(const field_t<t_field_pos> &p_field) {
  using index = index_t<t_field_pos>;
  using index_iterator =
      typename record::template index_iterator_t<t_field_pos>;

  std::vector<record_ref> _res;

  index &_index{std::get<t_field_pos>(m_indexes)};

  std::pair<typename index_iterator::value_type, index_iterator> _range{
      _index.equal_range(p_field)};

  for (typename index_iterator::value_type _ite{_range.first};
       _ite != _range.second; ++_ite) {
    if (_ite->second.get().get_optional().has_value())
      _res.push_back(record_ref{_ite->second.get()});
  }
  return _res;
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
multi_index_t<t_logger, t_fields_definitions...>::record::record(
    typename multi_index_t<t_logger, t_fields_definitions...>::object
        &&p_object)
    : m_optional{std::move(p_object)} {
  reset_index_iterators();
}

template <log::cpt::logger t_logger,
          cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
void multi_index_t<t_logger,
                   t_fields_definitions...>::record::reset_index_iterators() {
  auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
    if constexpr (is_index<t_pos>()) {
      // using index = index_t<t_pos>;

      // index &_index{std::get<t_pos>(m_indexes.get())};
      std::get<t_pos>(m_index_iterators) = std::nullopt;
      ;
    }
    return true;
  }};

  tuple::bus::traverse<index_iterators, decltype(_visit)>(_visit);
}

} // namespace tnct::container::dat

#endif

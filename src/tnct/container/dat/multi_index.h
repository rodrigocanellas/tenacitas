/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_MULTI_INDEX_H
#define TNCT_CONTAINER_DAT_MULTI_INDEX_H

#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <ostream>
#include <tuple>
#include <utility>

#include "tnct/container/cpt/field_definition.h"
#include "tnct/container/cpt/index_definition.h"
#include "tnct/container/dat/chuncked_container.h"
#include "tnct/container/trt/fields_definitions_are_compatible.h"
#include "tnct/container/trt/index_id_has_index_definition.h"
#include "tnct/container/trt/no_index_definition.h"
#include "tnct/tuple/bus/traverse.h"
#include "tnct/tuple/cpt/is_tuple.h"

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
/// \details You can use the tnct::container::trt::attribute_field_definition,
/// tnct::container::trt::index_field_definition and
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
template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
class multi_index final {

public:
  using fields_definitions = std::tuple<t_fields_definitions...>;

  /// Object that will be multi indexed
  using object =
      typename std::tuple_element_t<0, fields_definitions>::object_type;

  /// This class gives acess to a const object
  struct record;

  /// Actually it is a reference to the \p record
  using rec_opt_ref = std::reference_wrapper<std::optional<record>>;

  // static_assert(
  //     container::trt::index_id_has_index_definition_v<rec_opt_ref,
  //                                                     t_fields_definitions...>,
  //     "At least one 'index_id' in one of the 'cpt::field_definition' does not
  //     " "have a 'trt::index_definition' associated or is wrongly defined");

  /// Type of the \p t_field_pos-th field
  template <std::size_t t_field_pos>
  using field_type =
      typename std::tuple_element_t<t_field_pos,
                                    fields_definitions>::field_type;

  /// Allows controlled access to the \p std::optional that contains (or not)
  /// the object being multi indexed
  struct record final {
    friend multi_index;

    record() = default;

    record(const record &) = default;
    record &operator=(const record &) = default;

    record(record &&) = default;
    record &operator=(record &&) = default;

    void *operator new(std::size_t) = delete;
    void *operator new[](std::size_t) = delete;

    const object &get_object() const { return m_object; }

    constexpr bool operator<(const record &p_record) const {
      return m_object < p_record.m_object;
    }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const record &p_record) {

      auto _visit = [&]<tuple::cpt::is_tuple t_tuple, std::size_t t_index>(
                        const t_tuple &p_tuple) {
        if constexpr (record::template is_iterator<t_index>()) {
          p_out << std::get<t_index>(p_tuple).value()->first << ' ';
        }
        return true;
      };

      p_out << "[ " << p_record.m_object << " ( ";

      tuple::bus::traverse<optional_indexes_iterators, decltype(_visit)>(
          p_record.m_optional_index_iterators, _visit);

      p_out << ")]";
      return p_out;
    }

  private:
    using optional_indexes_iterators =
        std::tuple<std::optional<typename container::trt::index_definition<
            typename t_fields_definitions::index_id,
            typename t_fields_definitions::field_type,
            rec_opt_ref>::index_type::iterator>...>;

    template <std::size_t t_field_pos>
    using optional_index_iterator =
        std::tuple_element_t<t_field_pos, optional_indexes_iterators>;

  private:
    record(object &&p_object);

    void set_optional_indexes_iterators(
        optional_indexes_iterators &&p_index_iterators) {
      m_optional_index_iterators = std::move(p_index_iterators);
    }

    template <std::size_t t_index_pos>
    void set_optional_index_iterator(
        optional_index_iterator<t_index_pos> p_iterator) {
      std::get<t_index_pos>(m_optional_index_iterators) = p_iterator;
    }

    object &get_mutable_object() { return m_object; }

    optional_indexes_iterators &get_optional_indexes_iterators() {
      return m_optional_index_iterators;
    }

    void reset_optional_indexes_iterators();

    template <std::size_t t_field_pos> static constexpr bool is_iterator() {
      return !std::is_same_v<trt::no_index_id,
                             typename std::tuple_element_t<
                                 t_field_pos, fields_definitions>::index_id>;
    }

    template <std::size_t t_index_pos>
    bool get_field(const field_type<t_index_pos> &p_field) const {
      if (std::get<t_index_pos>(m_optional_index_iterators).has_value()) {
        return std::get<t_index_pos>(m_optional_index_iterators)
                   .value()
                   ->first == p_field;
      }
      return false;
    }

  private:
    object m_object{};
    optional_indexes_iterators m_optional_index_iterators{};
  };

  // end of \p record class

  multi_index() = default;

  multi_index(const multi_index &) = delete;
  multi_index(multi_index &&) = delete;

  ~multi_index() = default;

  multi_index &operator=(const multi_index &) = delete;
  multi_index &operator=(multi_index &&) = delete;

  void *operator new(std::size_t) = delete;
  void operator delete(void *) = delete;

  /// Adds a \p object and returns a std::optional reference with (or not) a
  /// reference to the record
  std::optional<rec_opt_ref> add(object &&p_object);

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
  std::vector<rec_opt_ref> get(const field_type<t_field_pos> &p_field);

  /// Erase all the references to \p record that have a certain field value
  ///
  /// \tparam t_field_pos is the field used to retrieve the references to \p
  /// record
  ///
  /// \param p_field is the value of the field to be used as comparision
  template <std::size_t t_field_pos>
  void erase(const field_type<t_field_pos> &p_field);

  /// Updates a certain field of a record
  ///
  /// \tparam t_field_pos is the field used to retrieve the references to \p
  /// record
  ///
  /// \param p_rec_opt_ref is a reference to the \p record being
  /// modified
  ///
  /// \param p_field is the new value of the field
  ///
  /// \note if there is an index associated to t_field_pos, the entry in the
  /// index will be updated
  template <std::size_t t_field_pos>
  bool update(rec_opt_ref p_rec_opt_ref,
              const field_type<t_field_pos> &p_field);

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const multi_index &p_multi_index) {

    {
      std::cout << "\nobjects:\n";
      for (table_const_iterator _ite = p_multi_index.m_table.begin();
           _ite != p_multi_index.m_table.end(); ++_ite) {
        if (_ite->has_value()) {
          p_out << _ite->value() << " ";
        } else {
          std::cout << "DELETED";
        }
        p_out << '\n';
      }
    }
    {
      p_out << "\nindexes:\n";
      auto _visitor = [&]<tuple::cpt::is_tuple t_tuple, size_t t_index_pos>(
                          const t_tuple &p_indexes) {
        if constexpr (is_field_an_index<t_index_pos>()) {
          using index = std::tuple_element_t<t_index_pos, multi_index::indexes>;

          const index &_index{std::get<t_index_pos>(p_indexes)};

          std::cout << "index " << t_index_pos << " - ";
          for (const typename index::value_type &_value : _index) {
            if (_value.second.get().has_value()) {
              p_out << "\t{k = " << _value.first
                    << ", obj = " << _value.second.get().value().get_object()
                    << "}";
            } else {
              p_out << "\t{k = " << _value.first << ", obj = DELETED}";
            }
          }
          p_out << '\n';
        }
        return true;
      };
      tuple::bus::traverse<multi_index::indexes, decltype(_visitor)>(
          p_multi_index.m_indexes, _visitor);
    }

    return p_out;
  }

private:
  /// \todo this will be replaced with something like
  /// std::list<std::array<object,N>>
  //  using table = std::list<record>;
  using table = container::dat::chunked_container<record, 100>;
  using table_iterator = typename table::iterator;
  using table_const_iterator = typename table::const_iterator;
  using table_element = typename table::element;
  using table_type = typename table::type;

private:
  using indexes = std::tuple<typename container::trt::index_definition<
      typename t_fields_definitions::index_id,
      typename t_fields_definitions::field_type, rec_opt_ref>::index_type...>;

  template <std::size_t t_field_pos>
  using index = std::tuple_element_t<t_field_pos, indexes>;

  template <std::size_t t_field_pos>
  using index_iterator = typename index<t_field_pos>::iterator;

  template <std::size_t t_field_pos>
  using index_const_iterator = typename index<t_field_pos>::const_iterator;

  template <std::size_t t_field_pos>
  using field_getter =
      typename std::tuple_element_t<t_field_pos,
                                    fields_definitions>::field_getter;

  template <std::size_t t_field_pos>
  using field_setter =
      typename std::tuple_element_t<t_field_pos,
                                    fields_definitions>::field_setter;

private:
  template <std::size_t t_field_pos> static constexpr bool is_field_an_index() {
    return !std::is_same_v<trt::no_index_id,
                           typename std::tuple_element_t<
                               t_field_pos, fields_definitions>::index_id>;
  }

  template <std::size_t t_field_pos>
  static constexpr bool is_field_a_calculated_index() {
    return is_field_an_index<t_field_pos> &&
           std::tuple_element_t<t_field_pos, fields_definitions>::is_calculated;
  }

  template <std::size_t t_field_pos>
  bool update_index(rec_opt_ref p_rec_opt_ref,
                    const field_type<t_field_pos> &p_field);

  bool update_calculated_indexes(rec_opt_ref p_rec_opt_ref);

  template <std::size_t t_field_pos>
  void erase_by_index(const field_type<t_field_pos> &p_field);

  template <std::size_t t_field_pos>
  void erase_by_field(const field_type<t_field_pos> &p_field);

  template <std::size_t t_field_pos>
  std::vector<rec_opt_ref> get_by_index(const field_type<t_field_pos> &p_field);

  template <std::size_t t_field_pos>
  std::vector<rec_opt_ref>
  get_by_attribute(const field_type<t_field_pos> &p_field);

  void erase_indexes(
      typename record::optional_indexes_iterators &p_optional_indexs_iterators);

  void erase_record(std::optional<record> &p_optional);

private:
  table m_table;
  indexes m_indexes;
  std::mutex m_mutex;
};

// #############################################################################
// implementation
// #############################################################################
template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
std::optional<typename multi_index<t_fields_definitions...>::rec_opt_ref>
multi_index<t_fields_definitions...>::

    add(typename multi_index<t_fields_definitions...>::object &&p_object) {

  std::lock_guard<std::mutex> _lock{m_mutex};

  record _record{std::move(p_object)};

  rec_opt_ref _rec_opt_ref{m_table.add(std::move(_record))};

  bool _error{false};
  auto _visitor{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_field_pos>() {
    if constexpr (is_field_an_index<t_field_pos>()) {

      using optional_index_iterator =
          typename record::template optional_index_iterator<t_field_pos>;
      using field = field_type<t_field_pos>;

      field_getter<t_field_pos> _field_getter;

      index<t_field_pos> &_index{std::get<t_field_pos>(m_indexes)};

      const field _field{
          _field_getter(_rec_opt_ref.get().value().get_object())};

      std::pair<typename optional_index_iterator::value_type, bool> _res{
          _index.emplace(_field, _rec_opt_ref)};
      if (_res.second) {
        // index creation ok
        _rec_opt_ref.get()
            .value()
            .template set_optional_index_iterator<t_field_pos>({_res.first});
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
    //    erase_record(_rec_opt_ref);
    m_table.unadd();
    return std::nullopt;
  }
  return {_rec_opt_ref};
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
std::vector<typename multi_index<t_fields_definitions...>::rec_opt_ref>
multi_index<t_fields_definitions...>::

    get(const field_type<t_field_pos> &p_field) {

  std::vector<rec_opt_ref> _res;
  if constexpr (is_field_an_index<t_field_pos>()) {
    _res = get_by_index<t_field_pos>(p_field);
  } else {
    _res = get_by_attribute<t_field_pos>(p_field);
  }
  if (!_res.empty()) {
    std::sort(_res.begin(), _res.end(),
              [&](const rec_opt_ref &p_1, const rec_opt_ref &p_2) {
                return p_1.get().value() < p_2.get().value();
              });
  }

  return _res;
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
void multi_index<t_fields_definitions...>::

    erase(const field_type<t_field_pos> &p_field) {

  std::lock_guard<std::mutex> _lock{m_mutex};
  if constexpr (is_field_an_index<t_field_pos>()) {
    erase_by_index<t_field_pos>(p_field);
  } else {
    erase_by_field<t_field_pos>(p_field);
  }
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
bool multi_index<t_fields_definitions...>::

    update(rec_opt_ref p_rec_opt_ref, const field_type<t_field_pos> &p_field) {

  if (!p_rec_opt_ref.get().has_value()) {
    return false;
  }

  std::lock_guard<std::mutex> _lock{m_mutex};

  bool _ok{true};
  if constexpr (is_field_an_index<t_field_pos>()) {
    _ok = update_index<t_field_pos>(p_rec_opt_ref, p_field);
  } else {
    field_setter<t_field_pos>{}(
        p_rec_opt_ref.get().value().get_mutable_object(), p_field);
  }

  return (!_ok ? _ok : update_calculated_indexes(p_rec_opt_ref));
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
bool multi_index<t_fields_definitions...>::

    update_index(rec_opt_ref p_rec_opt_ref,
                 const field_type<t_field_pos> &p_field) {

  if (!p_rec_opt_ref.get().has_value()) {
    return false;
  }
  record &_record = p_rec_opt_ref.get().value();

  typename record::optional_indexes_iterators &_optional_indexes_iterators =
      _record.get_optional_indexes_iterators();

  index<t_field_pos> &_index = std::get<t_field_pos>(m_indexes);

  if (std::get<t_field_pos>(_optional_indexes_iterators).value()->first !=
      p_field) {
    auto [_new_ite, _inserted] = _index.emplace(p_field, p_rec_opt_ref);

    if (!_inserted) {
      return false;
    }

    object &_object = _record.get_mutable_object();

    _index.erase(std::get<t_field_pos>(_optional_indexes_iterators).value());

    std::get<t_field_pos>(_optional_indexes_iterators) = _new_ite;

    field_setter<t_field_pos>{}(_object, p_field);
  }

  return true;
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
bool multi_index<t_fields_definitions...>::

    update_calculated_indexes(rec_opt_ref p_rec_opt_ref) {

  if (!p_rec_opt_ref.get().has_value()) {
    return false;
  }

  bool _ok{true};
  auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
    if constexpr (is_field_a_calculated_index<t_pos>()) {
      using field_getter = field_getter<t_pos>;
      field_getter _field_getter;
      object &_object{p_rec_opt_ref.get().value().get_mutable_object()};
      if (!update_index<t_pos>(p_rec_opt_ref, _field_getter(_object))) {
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

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
void multi_index<t_fields_definitions...>::

    erase_by_index(const field_type<t_field_pos> &p_field) {

  index<t_field_pos> &_index{std::get<t_field_pos>(m_indexes)};
  using index_iterator = index_iterator<t_field_pos>;

  std::pair<index_iterator, index_iterator> _range{_index.equal_range(p_field)};

  if (_range.first == _range.second) {
    return;
  }
  index_iterator _ite{_range.first};
  while (true) {
    index_iterator _aux = _ite;
    ++_ite;
    erase_record(_aux->second.get());

    if (_ite == _range.second) {
      break;
    }
  }
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
void multi_index<t_fields_definitions...>::

    erase_by_field(const field_type<t_field_pos> &p_field) {

  using field_getter = field_getter<t_field_pos>;

  for (table_element &_table_element : m_table) {
    if (_table_element.has_value()) {
      table_type &_record{_table_element.value()};
      if (p_field == field_getter{}(_record.get_object())) {
        erase_record(_table_element);
      }
    }
  }
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
void multi_index<t_fields_definitions...>::

    erase_record(std::optional<record> &p_optional) {

  erase_indexes(p_optional.value().get_optional_indexes_iterators());
  p_optional.value().reset_optional_indexes_iterators();
  p_optional.reset();
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
void multi_index<t_fields_definitions...>::

    erase_indexes(typename record::optional_indexes_iterators
                      &p_optional_indexs_iterators) {

  auto _visit{[&]<tuple::cpt::is_tuple t_index_iterators, std::size_t t_pos>() {
    if constexpr (is_field_an_index<t_pos>()) {

      using index = index<t_pos>;
      using optional_index_iterator =
          typename record::template optional_index_iterator<t_pos>;

      index &_index{std::get<t_pos>(m_indexes)};
      optional_index_iterator _optional_index_iterator{
          std::get<t_pos>(p_optional_indexs_iterators)};

      if (_optional_index_iterator.has_value()) {
        _index.erase(_optional_index_iterator.value());
      }
    }
    return true;
  }};

  tuple::bus::traverse<typename record::optional_indexes_iterators,
                       decltype(_visit)>(_visit);
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
std::vector<typename multi_index<t_fields_definitions...>::rec_opt_ref>
multi_index<t_fields_definitions...>::

    get_by_attribute(const field_type<t_field_pos> &p_field) {

  std::vector<rec_opt_ref> _res;
  for (table_element &_table_element : m_table) {
    if (_table_element.has_value()) {
      field_getter<t_field_pos> _field_getter;
      record &_record{_table_element.value()};
      if (p_field == _field_getter(_record.get_object())) {
        _res.push_back(rec_opt_ref{_table_element});
      }
    }
  }
  return _res;
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
template <std::size_t t_field_pos>
std::vector<typename multi_index<t_fields_definitions...>::rec_opt_ref>
multi_index<t_fields_definitions...>::

    get_by_index(const field_type<t_field_pos> &p_field) {

  using index_iterator = index_iterator<t_field_pos>;

  std::vector<rec_opt_ref> _result;

  index<t_field_pos> &_index{std::get<t_field_pos>(m_indexes)};

  std::pair<index_iterator, index_iterator> _range{_index.equal_range(p_field)};

  for (index_iterator _ite{_range.first}; _ite != _range.second; ++_ite) {
    rec_opt_ref &_rec_opt_ref{_ite->second};
    if (_rec_opt_ref.get().has_value()) {
      _result.push_back(_rec_opt_ref);
    }
  }
  return _result;
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
multi_index<t_fields_definitions...>::record::record(
    typename multi_index<t_fields_definitions...>::object &&p_object)
    : m_object{std::move(p_object)} {
  reset_optional_indexes_iterators();
}

template <cpt::field_definition... t_fields_definitions>
  requires(trt::fields_definitions_are_compatible_v<t_fields_definitions...>)
void multi_index<
    t_fields_definitions...>::record::reset_optional_indexes_iterators() {
  auto _visit{[&]<tuple::cpt::is_tuple t_tuple, std::size_t t_pos>() {
    if constexpr (is_field_an_index<t_pos>()) {
      // using index = index_t<t_pos>;

      // index &_index{std::get<t_pos>(m_indexes.get())};
      std::get<t_pos>(m_optional_index_iterators) = std::nullopt;
    }
    return true;
  }};

  tuple::bus::traverse<optional_indexes_iterators, decltype(_visit)>(_visit);
}

} // namespace tnct::container::dat

#endif

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_CHUNKED_CONTAINER_TEST_H
#define TNCT_CONTAINER_TST_CHUNKED_CONTAINER_TEST_H

#include <concepts>
#include <cstddef>
#include <iterator>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "tnct/container/dat/chuncked_container.h"
#include "tnct/program/bus/options.h"

namespace tnct::container::tst {

namespace chunked_container_test {

using container = tnct::container::dat::chunked_container<int, 4>;
using small_container = tnct::container::dat::chunked_container<int, 2>;

static_assert(std::bidirectional_iterator<typename container::iterator>);
static_assert(std::bidirectional_iterator<typename container::const_iterator>);
static_assert(std::same_as<std::iter_value_t<typename container::iterator>,
                           typename container::element>);
static_assert(std::same_as<std::iter_reference_t<typename container::iterator>,
                           typename container::element &>);
static_assert(
    std::same_as<std::iter_reference_t<typename container::const_iterator>,
                 const typename container::element &>);

inline bool has_value(container &p_container, std::size_t p_index,
                      int p_expected) {
  auto _slot{p_container[p_index]};

  return _slot.has_value() && _slot->get().has_value() &&
         (_slot->get().value() == p_expected);
}

inline bool is_empty(container &p_container, std::size_t p_index) {
  auto _slot{p_container[p_index]};
  return _slot.has_value() && !_slot->get().has_value();
}

inline bool has_value(small_container &p_container, std::size_t p_index,
                      int p_expected) {
  auto _slot{p_container[p_index]};

  return _slot.has_value() && _slot->get().has_value() &&
         (_slot->get().value() == p_expected);
}

inline bool is_empty(small_container &p_container, std::size_t p_index) {
  auto _slot{p_container[p_index]};
  return _slot.has_value() && !_slot->get().has_value();
}

inline std::vector<std::optional<int>> forward_values(container &p_container) {
  std::vector<std::optional<int>> _values;

  for (auto _ite = p_container.begin(); _ite != p_container.end(); ++_ite) {
    _values.push_back(*_ite);
  }

  return _values;
}

inline std::vector<std::optional<int>> backward_values(container &p_container) {
  std::vector<std::optional<int>> _values;

  auto _ite{p_container.end()};
  while (_ite != p_container.begin()) {
    --_ite;
    _values.push_back(*_ite);
  }

  return _values;
}

struct non_default {
  non_default() = delete;
  explicit non_default(int p_value) : value{p_value} {}

  non_default(const non_default &) = default;
  non_default(non_default &&) = default;
  non_default &operator=(const non_default &) = default;
  non_default &operator=(non_default &&) = default;

  int value;
};

} // namespace chunked_container_test

struct chunked_container_001 {
  static std::string desc() {
    return "chunked_container: constructor creates one chunk, initializes slot "
           "0 and leaves remaining slots empty";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{10};

    return (_container.get_chunck_size() == 4) && has_value(_container, 0, 10) &&
           is_empty(_container, 1) && is_empty(_container, 2) &&
           is_empty(_container, 3) && !_container[4].has_value();
  }
};

struct chunked_container_002 {
  static std::string desc() {
    return "chunked_container: add fills consecutive slots in the current chunk "
           "and returns references to those slots";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};

    auto _r1{_container.add(11)};
    auto _r2{_container.add(22)};
    auto _r3{_container.add(33)};

    return has_value(_container, 0, 0) && has_value(_container, 1, 11) &&
           has_value(_container, 2, 22) && has_value(_container, 3, 33) &&
           _r1.get().has_value() && (_r1.get().value() == 11) &&
           _r2.get().has_value() && (_r2.get().value() == 22) &&
           _r3.get().has_value() && (_r3.get().value() == 33);
  }
};

struct chunked_container_003 {
  static std::string desc() {
    return "chunked_container: add allocates a new chunk when the current chunk "
           "becomes full";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};
    _container.add(1);
    _container.add(2);
    _container.add(3);
    _container.add(4);

    return has_value(_container, 3, 3) && has_value(_container, 4, 4) &&
           is_empty(_container, 5) && is_empty(_container, 6) &&
           is_empty(_container, 7) && !_container[8].has_value();
  }
};

struct chunked_container_004 {
  static std::string desc() {
    return "chunked_container: slot and contained-object addresses remain stable "
           "when new chunks are added";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{100};
    auto _r1{_container.add(101)};

    auto *_slot_address{&_r1.get()};
    auto *_value_address{&_r1.get().value()};

    for (int _value = 102; _value < 150; ++_value) {
      _container.add(std::move(_value));
    }

    auto _slot{_container[1]};
    if (!_slot.has_value() || !_slot->get().has_value()) {
      return false;
    }

    return (&_slot->get() == _slot_address) &&
           (&_slot->get().value() == _value_address) &&
           (_slot->get().value() == 101);
  }
};

struct chunked_container_005 {
  static std::string desc() {
    return "chunked_container: forward iterator visits every slot of every "
           "allocated chunk, including nullopt slots";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};
    _container.add(1);
    _container.add(2);
    _container.add(3);
    _container.add(4);

    const std::vector<std::optional<int>> _expected{
        0, 1, 2, 3, 4, std::nullopt, std::nullopt, std::nullopt};

    return forward_values(_container) == _expected;
  }
};

struct chunked_container_006 {
  static std::string desc() {
    return "chunked_container: bidirectional iterator traverses all allocated "
           "slots backwards from end";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};
    _container.add(1);
    _container.add(2);
    _container.add(3);
    _container.add(4);

    const std::vector<std::optional<int>> _expected{
        std::nullopt, std::nullopt, std::nullopt, 4, 3, 2, 1, 0};

    return backward_values(_container) == _expected;
  }
};

struct chunked_container_007 {
  static std::string desc() {
    return "chunked_container: prefix and postfix increment have standard "
           "iterator semantics";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{10};
    _container.add(20);
    _container.add(30);

    auto _ite{_container.begin()};
    auto _old{_ite++};

    if (!(*_old).has_value() || ((*_old).value() != 10) ||
        !(*_ite).has_value() || ((*_ite).value() != 20)) {
      return false;
    }

    auto &_prefix{++_ite};

    return (&_prefix == &_ite) && (*_ite).has_value() &&
           ((*_ite).value() == 30);
  }
};

struct chunked_container_008 {
  static std::string desc() {
    return "chunked_container: prefix and postfix decrement have standard "
           "iterator semantics";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{10};
    _container.add(20);

    auto _ite{_container.end()};
    auto _old{_ite--};

    if (_old != _container.end() || _ite == _container.end() ||
        (*_ite).has_value()) {
      return false;
    }

    auto &_prefix{--_ite};

    return (&_prefix == &_ite) && !(*_ite).has_value();
  }
};

struct chunked_container_009 {
  static std::string desc() {
    return "chunked_container: iterator dereference and operator-> access the "
           "same slot";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{7};
    auto _ite{_container.begin()};

    if (_ite.operator->() != &(*_ite)) {
      return false;
    }

    _ite->value() = 9;

    return has_value(_container, 0, 9);
  }
};

struct chunked_container_010 {
  static std::string desc() {
    return "chunked_container: iterator equality includes the owning container";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container_a{1};
    container _container_b{1};

    auto _a1{_container_a.begin()};
    auto _a2{_container_a.begin()};
    auto _b{_container_b.begin()};

    return (_a1 == _a2) && (_a1 != _b) &&
           (_container_a.end() != _container_b.end());
  }
};

struct chunked_container_011 {
  static std::string desc() {
    return "chunked_container: operator[] maps allocated slots and rejects "
           "indexes beyond allocated chunks";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};

    if (!_container[3].has_value() || _container[4].has_value()) {
      return false;
    }

    _container.add(1);
    _container.add(2);
    _container.add(3);
    _container.add(4);

    return _container[7].has_value() && !_container[8].has_value();
  }
};

struct chunked_container_012 {
  static std::string desc() {
    return "chunked_container: const begin/end/cbegin/cend and const operator[] "
           "provide read-only access";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{5};
    _container.add(6);

    const container &_const_container{_container};

    auto _begin{_const_container.begin()};
    auto _cbegin{_const_container.cbegin()};
    auto _end{_const_container.end()};
    auto _cend{_const_container.cend()};

    if ((_begin != _cbegin) || (_end != _cend) || !_begin->has_value() ||
        (_begin->value() != 5)) {
      return false;
    }

    auto _slot{_const_container[1]};

    return _slot.has_value() && _slot->get().has_value() &&
           (_slot->get().value() == 6) && !_const_container[4].has_value();
  }
};

struct chunked_container_013 {
  static std::string desc() {
    return "chunked_container: remove by index clears an engaged slot without "
           "changing its address";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};
    auto _ref{_container.add(10)};
    auto *_address{&_ref.get()};

    _container.remove(1);

    auto _slot{_container[1]};

    return _slot.has_value() && !_slot->get().has_value() &&
           (&_slot->get() == _address);
  }
};

struct chunked_container_014 {
  static std::string desc() {
    return "chunked_container: remove by invalid index or already-empty slot is "
           "a no-op";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{10};
    _container.add(20);

    _container.remove(3);
    _container.remove(1000);

    return has_value(_container, 0, 10) && has_value(_container, 1, 20) &&
           is_empty(_container, 3);
  }
};

struct chunked_container_015 {
  static std::string desc() {
    return "chunked_container: remove by iterator clears the iterator slot";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{10};
    _container.add(20);

    auto _ite{_container.begin()};
    ++_ite;
    _container.remove(_ite);

    return has_value(_container, 0, 10) && is_empty(_container, 1) &&
           !(*_ite).has_value();
  }
};

struct chunked_container_016 {
  static std::string desc() {
    return "chunked_container: remove ignores end and iterators owned by another "
           "container";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container_a{10};
    container _container_b{20};

    _container_a.remove(_container_a.end());
    _container_a.remove(_container_b.begin());

    return has_value(_container_a, 0, 10) && has_value(_container_b, 0, 20);
  }
};

struct chunked_container_017 {
  static std::string desc() {
    return "chunked_container: erase by iterator clears the iterator slot and "
           "ignores foreign iterators";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container_a{10};
    container _container_b{20};

    auto _ite{_container_a.begin()};
    _container_a.erase(_container_b.begin());

    if (!has_value(_container_a, 0, 10) || !has_value(_container_b, 0, 20)) {
      return false;
    }

    _container_a.erase(_ite);

    return is_empty(_container_a, 0);
  }
};

struct chunked_container_018 {
  static std::string desc() {
    return "chunked_container: erase by ref clears the referenced slot";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};
    auto _ref{_container.add(10)};

    _container.erase(_ref);

    return !_ref.get().has_value() && is_empty(_container, 1);
  }
};

struct chunked_container_019 {
  static std::string desc() {
    return "chunked_container: add does not reuse a removed slot";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};
    _container.add(1);
    _container.remove(1);
    auto _ref{_container.add(2)};

    return is_empty(_container, 1) && has_value(_container, 2, 2) &&
           (&_ref.get() == &_container[2]->get());
  }
};

struct chunked_container_020 {
  static std::string desc() {
    return "chunked_container: mutable operator[] can update an engaged slot "
           "without changing its address";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{10};
    auto _slot{_container[0]};

    if (!_slot.has_value() || !_slot->get().has_value()) {
      return false;
    }

    auto *_address{&_slot->get()};
    _slot->get().value() = 50;

    auto _again{_container[0]};

    return _again.has_value() && (_again->get().value() == 50) &&
           (&_again->get() == _address);
  }
};

struct chunked_container_021 {
  static std::string desc() {
    return "chunked_container: copy construction preserves slot state and makes "
           "an independent container";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _source{10};
    _source.add(20);
    _source.add(30);
    _source.remove(1);

    container _copy{_source};

    if (!has_value(_copy, 0, 10) || !is_empty(_copy, 1) ||
        !has_value(_copy, 2, 30)) {
      return false;
    }

    _copy[0]->get().value() = 99;

    return has_value(_source, 0, 10) && has_value(_copy, 0, 99);
  }
};

struct chunked_container_022 {
  static std::string desc() {
    return "chunked_container: copy assignment preserves slot state and makes "
           "an independent container";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _source{10};
    _source.add(20);

    container _copy{0};
    _copy = _source;
    _copy.remove(1);

    return has_value(_source, 0, 10) && has_value(_source, 1, 20) &&
           has_value(_copy, 0, 10) && is_empty(_copy, 1);
  }
};

struct chunked_container_023 {
  static std::string desc() {
    return "chunked_container: move construction and move assignment preserve "
           "the destination contents";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _source_a{10};
    _source_a.add(20);
    container _moved{std::move(_source_a)};

    if (!has_value(_moved, 0, 10) || !has_value(_moved, 1, 20)) {
      return false;
    }

    container _source_b{30};
    _source_b.add(40);
    _moved = std::move(_source_b);

    return has_value(_moved, 0, 30) && has_value(_moved, 1, 40);
  }
};

struct chunked_container_024 {
  static std::string desc() {
    return "chunked_container: chunk_size equal to one allocates one new chunk "
           "for each added object";
  }

  bool operator()(const program::bus::options &) {
    using one_container = tnct::container::dat::chunked_container<int, 1>;

    one_container _container{10};
    _container.add(20);
    _container.add(30);

    auto _slot0{_container[0]};
    auto _slot1{_container[1]};
    auto _slot2{_container[2]};

    return (_container.get_chunck_size() == 1) && _slot0.has_value() &&
           _slot0->get().has_value() && (_slot0->get().value() == 10) &&
           _slot1.has_value() && _slot1->get().has_value() &&
           (_slot1->get().value() == 20) && _slot2.has_value() &&
           _slot2->get().has_value() && (_slot2->get().value() == 30) &&
           !_container[3].has_value();
  }
};

struct chunked_container_025 {
  static std::string desc() {
    return "chunked_container: explicitly initialized container supports a "
           "copy/move type without default constructor";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;
    using non_default_container =
        tnct::container::dat::chunked_container<non_default, 2>;

    non_default_container _container{non_default{10}};
    auto _ref{_container.add(non_default{20})};

    auto _slot0{_container[0]};
    auto _slot1{_container[1]};

    return _slot0.has_value() && _slot0->get().has_value() &&
           (_slot0->get()->value == 10) && _slot1.has_value() &&
           _slot1->get().has_value() && (_slot1->get()->value == 20) &&
           _ref.get().has_value() && (_ref.get()->value == 20);
  }
};

struct chunked_container_026 {
  static std::string desc() {
    return "chunked_container: a reference to a removed slot remains valid and "
           "continues referring to the same empty slot after later additions";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};
    auto _ref{_container.add(10)};
    auto *_address{&_ref.get()};

    _container.remove(1);

    for (int _value = 20; _value < 50; ++_value) {
      _container.add(std::move(_value));
    }

    auto _slot{_container[1]};

    return !_ref.get().has_value() && _slot.has_value() &&
           !_slot->get().has_value() && (&_slot->get() == _address);
  }
};

struct chunked_container_027 {
  static std::string desc() {
    return "chunked_container: index mapping remains correct across several "
           "chunk boundaries";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{0};

    for (int _value = 1; _value <= 12; ++_value) {
      _container.add(std::move(_value));
    }

    for (std::size_t _index = 0; _index <= 12; ++_index) {
      auto _slot{_container[_index]};
      if (!_slot.has_value() || !_slot->get().has_value() ||
          (_slot->get().value() != static_cast<int>(_index))) {
        return false;
      }
    }

    return is_empty(_container, 13) && is_empty(_container, 14) &&
           is_empty(_container, 15) && !_container[16].has_value();
  }
};

struct chunked_container_028 {
  static std::string desc() {
    return "chunked_container: iterators to existing slots remain usable after "
           "new chunks are added";
  }

  bool operator()(const program::bus::options &) {
    using namespace chunked_container_test;

    container _container{10};
    _container.add(20);

    auto _ite{_container.begin()};
    ++_ite;
    auto *_slot_address{&(*_ite)};

    for (int _value = 30; _value < 80; ++_value) {
      _container.add(std::move(_value));
    }

    return (*_ite).has_value() && ((*_ite).value() == 20) &&
           (&(*_ite) == _slot_address);
  }
};

} // namespace tnct::container::tst

#endif

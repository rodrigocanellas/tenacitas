/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_SUPPLIER_TST_CPT_TEST_H
#define TNCT_SUPPLIER_TST_CPT_TEST_H

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "tnct/generic/cpt/meta_class.h"
#include "tnct/program/bus/options.h"
#include "tnct/supplier/cpt/const_reference_supplier.h"
#include "tnct/supplier/cpt/const_shared_ptr.h"
#include "tnct/supplier/cpt/internal/supplier.h"
#include "tnct/supplier/cpt/object_supplier.h"
#include "tnct/supplier/cpt/reference_supplier.h"
#include "tnct/supplier/cpt/shared_ptr.h"
#include "tnct/supplier/cpt/unique_ptr.h"

namespace tnct::supplier::tst {

namespace detail {

struct object {
  int value{0};
};

struct other_object {
  int value{0};
};

struct valid_supplier_base {
  valid_supplier_base() = default;
  valid_supplier_base(const valid_supplier_base &) = delete;
  valid_supplier_base(valid_supplier_base &&) = delete;

  valid_supplier_base &operator=(const valid_supplier_base &) = delete;
  valid_supplier_base &operator=(valid_supplier_base &&) = delete;

  void *operator new(std::size_t) = delete;
};

struct valid_internal_supplier : valid_supplier_base {};

struct copyable_supplier {
  void *operator new(std::size_t) = delete;
};

struct movable_supplier {
  movable_supplier() = default;
  movable_supplier(const movable_supplier &) = delete;
  movable_supplier(movable_supplier &&) = default;

  movable_supplier &operator=(const movable_supplier &) = delete;
  movable_supplier &operator=(movable_supplier &&) = default;

  void *operator new(std::size_t) = delete;
};

struct heap_supplier {
  heap_supplier() = default;
  heap_supplier(const heap_supplier &) = delete;
  heap_supplier(heap_supplier &&) = delete;

  heap_supplier &operator=(const heap_supplier &) = delete;
  heap_supplier &operator=(heap_supplier &&) = delete;
};

struct valid_object_supplier : valid_supplier_base {
  std::optional<object> get(generic::cpt::meta_class<object>) {
    return object{};
  }

  std::optional<object> get(generic::cpt::meta_class<object>, int,
                            const std::string &) {
    return object{};
  }
};

struct object_supplier_wrong_return : valid_supplier_base {
  object get(generic::cpt::meta_class<object>) { return {}; }
};

struct object_supplier_wrong_object : valid_supplier_base {
  std::optional<other_object> get(generic::cpt::meta_class<object>) {
    return other_object{};
  }
};

struct object_supplier_wrong_parameters : valid_supplier_base {
  std::optional<object> get(generic::cpt::meta_class<object>) {
    return object{};
  }
};

struct copyable_object_supplier {
  void *operator new(std::size_t) = delete;
  std::optional<object> get(generic::cpt::meta_class<object>) {
    return object{};
  }
};

struct valid_reference_supplier : valid_supplier_base {
  std::optional<std::reference_wrapper<object>>
  get(generic::cpt::meta_class<object>, int) {
    return std::ref(m_object);
  }

private:
  object m_object;
};

struct reference_supplier_wrong_constness : valid_supplier_base {
  std::optional<std::reference_wrapper<const object>>
  get(generic::cpt::meta_class<object>, int) {
    return std::cref(m_object);
  }

private:
  object m_object;
};

struct reference_supplier_wrong_return : valid_supplier_base {
  object &get(generic::cpt::meta_class<object>, int) { return m_object; }

private:
  object m_object;
};

struct valid_const_reference_supplier : valid_supplier_base {
  std::optional<std::reference_wrapper<const object>>
  get(generic::cpt::meta_class<object>, int) const {
    return std::cref(m_object);
  }

private:
  object m_object;
};

struct const_reference_supplier_non_const_get : valid_supplier_base {
  std::optional<std::reference_wrapper<const object>>
  get(generic::cpt::meta_class<object>, int) {
    return std::cref(m_object);
  }

private:
  object m_object;
};

struct const_reference_supplier_wrong_constness : valid_supplier_base {
  std::optional<std::reference_wrapper<object>>
  get(generic::cpt::meta_class<object>, int) const {
    return std::ref(m_object);
  }

private:
  mutable object m_object;
};

struct valid_shared_ptr_supplier : valid_supplier_base {
  std::shared_ptr<object> get(generic::cpt::meta_class<object>, int) {
    return std::make_shared<object>();
  }
};

struct shared_ptr_supplier_wrong_constness : valid_supplier_base {
  std::shared_ptr<const object> get(generic::cpt::meta_class<object>, int) {
    return std::make_shared<object>();
  }
};

struct shared_ptr_supplier_wrong_return : valid_supplier_base {
  std::unique_ptr<object> get(generic::cpt::meta_class<object>, int) {
    return std::make_unique<object>();
  }
};

struct valid_const_shared_ptr_supplier : valid_supplier_base {
  std::shared_ptr<const object>
  get(generic::cpt::meta_class<object>, int) const {
    return std::make_shared<object>();
  }
};

struct const_shared_ptr_supplier_non_const_get : valid_supplier_base {
  std::shared_ptr<const object> get(generic::cpt::meta_class<object>, int) {
    return std::make_shared<object>();
  }
};

struct const_shared_ptr_supplier_wrong_constness : valid_supplier_base {
  std::shared_ptr<object> get(generic::cpt::meta_class<object>, int) const {
    return std::make_shared<object>();
  }
};

struct valid_unique_ptr_supplier : valid_supplier_base {
  std::unique_ptr<object> get(generic::cpt::meta_class<object>, int) {
    return std::make_unique<object>();
  }
};

struct unique_ptr_supplier_wrong_constness : valid_supplier_base {
  std::unique_ptr<const object> get(generic::cpt::meta_class<object>, int) {
    return std::make_unique<object>();
  }
};

struct unique_ptr_supplier_wrong_return : valid_supplier_base {
  std::shared_ptr<object> get(generic::cpt::meta_class<object>, int) {
    return std::make_shared<object>();
  }
};

} // namespace detail

struct cpt_test_001 {
  static std::string desc() {
    return "internal::supplier accepts a non-copyable, non-movable and "
           "non-heap-allocatable type";
  }

  bool operator()(const program::bus::options &) {
    using detail::valid_internal_supplier;

    static_assert(cpt::internal::supplier<valid_internal_supplier>);
    static_assert(cpt::internal::supplier<const valid_internal_supplier &>);
    return true;
  }
};

struct cpt_test_002 {
  static std::string desc() {
    return "internal::supplier rejects copyable, movable or heap-allocatable "
           "types";
  }

  bool operator()(const program::bus::options &) {
    static_assert(!cpt::internal::supplier<detail::copyable_supplier>);
    static_assert(!cpt::internal::supplier<detail::movable_supplier>);
    static_assert(!cpt::internal::supplier<detail::heap_supplier>);
    static_assert(
        !cpt::internal::supplier<const detail::heap_supplier &>);
    return true;
  }
};

struct cpt_test_003 {
  static std::string desc() {
    return "object_supplier accepts the exact optional<object> return type";
  }

  bool operator()(const program::bus::options &) {
    using detail::object;
    using detail::valid_object_supplier;

    static_assert(cpt::object_supplier<valid_object_supplier, object>);
    static_assert(cpt::object_supplier<valid_object_supplier, object, int,
                                       const std::string &>);
    return true;
  }
};

struct cpt_test_004 {
  static std::string desc() {
    return "object_supplier rejects wrong return, object, parameters and base "
           "supplier semantics";
  }

  bool operator()(const program::bus::options &) {
    using detail::object;

    static_assert(
        !cpt::object_supplier<detail::object_supplier_wrong_return, object>);
    static_assert(
        !cpt::object_supplier<detail::object_supplier_wrong_object, object>);
    static_assert(
        !cpt::object_supplier<detail::object_supplier_wrong_parameters, object,
                              int>);
    static_assert(
        !cpt::object_supplier<detail::copyable_object_supplier, object>);
    return true;
  }
};

struct cpt_test_005 {
  static std::string desc() {
    return "reference_supplier accepts optional<reference_wrapper<object>>";
  }

  bool operator()(const program::bus::options &) {
    static_assert(cpt::reference_supplier<detail::valid_reference_supplier,
                                          detail::object, int>);
    return true;
  }
};

struct cpt_test_006 {
  static std::string desc() {
    return "reference_supplier rejects const references and non-wrapper "
           "returns";
  }

  bool operator()(const program::bus::options &) {
    static_assert(!cpt::reference_supplier<
                  detail::reference_supplier_wrong_constness, detail::object,
                  int>);
    static_assert(
        !cpt::reference_supplier<detail::reference_supplier_wrong_return,
                                 detail::object, int>);
    return true;
  }
};

struct cpt_test_007 {
  static std::string desc() {
    return "const_reference_supplier accepts a const get returning a const "
           "reference wrapper";
  }

  bool operator()(const program::bus::options &) {
    static_assert(cpt::const_reference_supplier<
                  detail::valid_const_reference_supplier, detail::object, int>);
    return true;
  }
};

struct cpt_test_008 {
  static std::string desc() {
    return "const_reference_supplier rejects non-const get and mutable "
           "reference wrapper";
  }

  bool operator()(const program::bus::options &) {
    static_assert(!cpt::const_reference_supplier<
                  detail::const_reference_supplier_non_const_get,
                  detail::object, int>);
    static_assert(!cpt::const_reference_supplier<
                  detail::const_reference_supplier_wrong_constness,
                  detail::object, int>);
    return true;
  }
};

struct cpt_test_009 {
  static std::string desc() {
    return "shared_ptr_supplier accepts the exact shared_ptr<object> return";
  }

  bool operator()(const program::bus::options &) {
    static_assert(cpt::shared_ptr_supplier<detail::valid_shared_ptr_supplier,
                                          detail::object, int>);
    return true;
  }
};

struct cpt_test_010 {
  static std::string desc() {
    return "shared_ptr_supplier rejects shared_ptr<const object> and other "
           "smart pointers";
  }

  bool operator()(const program::bus::options &) {
    static_assert(!cpt::shared_ptr_supplier<
                  detail::shared_ptr_supplier_wrong_constness, detail::object,
                  int>);
    static_assert(!cpt::shared_ptr_supplier<
                  detail::shared_ptr_supplier_wrong_return, detail::object,
                  int>);
    return true;
  }
};

struct cpt_test_011 {
  static std::string desc() {
    return "const_shared_ptr_supplier accepts a const get returning "
           "shared_ptr<const object>";
  }

  bool operator()(const program::bus::options &) {
    static_assert(cpt::const_shared_ptr_supplier<
                  detail::valid_const_shared_ptr_supplier, detail::object,
                  int>);
    return true;
  }
};

struct cpt_test_012 {
  static std::string desc() {
    return "const_shared_ptr_supplier rejects non-const get and "
           "shared_ptr<object>";
  }

  bool operator()(const program::bus::options &) {
    static_assert(!cpt::const_shared_ptr_supplier<
                  detail::const_shared_ptr_supplier_non_const_get,
                  detail::object, int>);
    static_assert(!cpt::const_shared_ptr_supplier<
                  detail::const_shared_ptr_supplier_wrong_constness,
                  detail::object, int>);
    return true;
  }
};

struct cpt_test_013 {
  static std::string desc() {
    return "unique_ptr_supplier accepts the exact unique_ptr<object> return";
  }

  bool operator()(const program::bus::options &) {
    static_assert(cpt::unique_ptr_supplier<detail::valid_unique_ptr_supplier,
                                          detail::object, int>);
    return true;
  }
};

struct cpt_test_014 {
  static std::string desc() {
    return "unique_ptr_supplier rejects unique_ptr<const object> and other "
           "smart pointers";
  }

  bool operator()(const program::bus::options &) {
    static_assert(!cpt::unique_ptr_supplier<
                  detail::unique_ptr_supplier_wrong_constness, detail::object,
                  int>);
    static_assert(!cpt::unique_ptr_supplier<
                  detail::unique_ptr_supplier_wrong_return, detail::object,
                  int>);
    return true;
  }
};

} // namespace tnct::supplier::tst

#endif

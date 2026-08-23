/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory
///
/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_MULTI_INDEX_TEST_H
#define TNCT_CONTAINER_TST_MULTI_INDEX_TEST_H

#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "tnct/container/dat/multi_index.h"
#include "tnct/container/trt/field_definition.h"
#include "tnct/container/trt/std_map_definition.h"
#include "tnct/container/trt/std_multimap_definition.h"
#include "tnct/program/bus/options.h"

namespace tnct::container::tst {

namespace multi_index_test {

struct object {
  object() = default;

  object(int p_id, float p_score, std::string_view p_name)
      : m_id{p_id}, m_score{p_score}, m_name{p_name} {}

  object(const object &) = default;
  object(object &&) = default;
  object &operator=(const object &) = default;
  object &operator=(object &&) = default;

  int get_id() const { return m_id; }
  void set_id(int p_id) { m_id = p_id; }

  float get_score() const { return m_score; }
  void set_score(float p_score) { m_score = p_score; }

  std::string get_name() const { return m_name; }
  void set_name(std::string p_name) { m_name = std::move(p_name); }

  bool operator==(const object &p_object) const {
    return std::tie(m_id, m_score, m_name) ==
           std::tie(p_object.m_id, p_object.m_score, p_object.m_name);
  }

  bool operator<(const object &p_object) const {
    return std::tie(m_id, m_score, m_name) <
           std::tie(p_object.m_id, p_object.m_score, p_object.m_name);
  }

  friend std::ostream &operator<<(std::ostream &p_out, const object &p_object) {
    p_out << "{id = " << p_object.m_id << ", score = " << p_object.m_score
          << ", name = " << p_object.m_name << "}";
    return p_out;
  }

private:
  int m_id{0};
  float m_score{0.0F};
  std::string m_name;
};

using tnct::container::trt::attribute_field_definition;
using tnct::container::trt::calculated_index_definition;
using tnct::container::trt::index_field_definition;
using tnct::container::trt::std_map_index_id;
using tnct::container::trt::std_multimap_index_id;

using id_field = index_field_definition<
    object, int,
    decltype([](const object &p_object) -> int { return p_object.get_id(); }),
    decltype([](object &p_object, int p_id) -> void { p_object.set_id(p_id); }),
    std_map_index_id>;

using score_field = index_field_definition<
    object, float,
    decltype([](const object &p_object) -> float {
      return p_object.get_score();
    }),
    decltype([](object &p_object, float p_score) -> void {
      p_object.set_score(p_score);
    }),
    std_multimap_index_id>;

using name_field = attribute_field_definition<
    object, std::string,
    decltype([](const object &p_object) -> std::string {
      return p_object.get_name();
    }),
    decltype([](object &p_object, std::string p_name) -> void {
      p_object.set_name(std::move(p_name));
    })>;

using name_unique_field = index_field_definition<
    object, std::string,
    decltype([](const object &p_object) -> std::string {
      return p_object.get_name();
    }),
    decltype([](object &p_object, std::string p_name) -> void {
      p_object.set_name(std::move(p_name));
    }),
    std_map_index_id>;

using id_name_calculated_field = calculated_index_definition<
    object, std::string,
    decltype([](const object &p_object) -> std::string {
      return std::to_string(p_object.get_id()) + ":" + p_object.get_name();
    }),
    std_multimap_index_id>;

using index =
    tnct::container::dat::multi_index<id_field, score_field, name_field>;

using record_ref = typename index::rec_opt_ref;

using rollback_index = tnct::container::dat::multi_index<
    id_field, name_unique_field, score_field>;

using rollback_record_ref = typename rollback_index::rec_opt_ref;

using calculated_index =
    tnct::container::dat::multi_index<id_field, score_field, name_field,
                                     id_name_calculated_field>;

using calculated_record_ref = typename calculated_index::rec_opt_ref;

template <typename t_record_ref>
inline bool has_object_in(const std::vector<t_record_ref> &p_records, int p_id,
                          float p_score, std::string_view p_name) {
  for (const t_record_ref &r : p_records) {
    if (!r.get().has_value()) {
      continue;
    }

    const object &obj{r.get().value().get_object()};
    if ((obj.get_id() == p_id) && (obj.get_score() == p_score) &&
        (obj.get_name() == p_name)) {
      return true;
    }
  }

  return false;
}

inline bool one_live_by_id(index &p_index, int p_id, float p_score,
                           std::string_view p_name) {
  const std::vector<record_ref> _records{p_index.get<0>(p_id)};
  return (_records.size() == 1) &&
         has_object_in(_records, p_id, p_score, p_name);
}

} // namespace multi_index_test

struct multi_index_001 {
  static std::string desc() {
    return "multi_index: duplicate unique key is rejected without changing "
           "other lookups";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    auto _dup{_index.add(object{1, 99.0F, "duplicated"})};

    return _r1.has_value() && !_dup.has_value() &&
           one_live_by_id(_index, 1, 10.0F, "one") &&
           _index.get<1>(99.0F).empty() &&
           _index.get<2>(std::string{"duplicated"}).empty();
  }
};

struct multi_index_002 {
  static std::string desc() {
    return "multi_index: erase by unique index removes the record from every "
           "lookup and allows key reuse";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    auto _r2{_index.add(object{2, 20.0F, "two"})};

    if (!_r1 || !_r2) {
      return false;
    }

    _index.erase<0>(1);

    if (_r1->get().has_value() || !_index.get<0>(1).empty() ||
        !_index.get<1>(10.0F).empty() ||
        !_index.get<2>(std::string{"one"}).empty() ||
        !one_live_by_id(_index, 2, 20.0F, "two")) {
      return false;
    }

    auto _r3{_index.add(object{1, 30.0F, "reused"})};

    return _r3.has_value() &&
           one_live_by_id(_index, 1, 30.0F, "reused");
  }
};

struct multi_index_003 {
  static std::string desc() {
    return "multi_index: erase by non-unique index removes all matching "
           "records";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    auto _r2{_index.add(object{2, 10.0F, "two"})};
    auto _r3{_index.add(object{3, 30.0F, "three"})};

    if (!_r1 || !_r2 || !_r3) {
      return false;
    }

    _index.erase<1>(10.0F);

    return !_r1->get().has_value() && !_r2->get().has_value() &&
           _r3->get().has_value() && _index.get<0>(1).empty() &&
           _index.get<0>(2).empty() && _index.get<1>(10.0F).empty() &&
           one_live_by_id(_index, 3, 30.0F, "three");
  }
};

struct multi_index_004 {
  static std::string desc() {
    return "multi_index: update of a unique indexed field moves the index "
           "entry";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;
    auto _r1{_index.add(object{1, 10.0F, "one"})};

    if (!_r1 || !_index.update<0>(_r1.value(), 2)) {
      return false;
    }

    return _index.get<0>(1).empty() &&
           one_live_by_id(_index, 2, 10.0F, "one");
  }
};

struct multi_index_005 {
  static std::string desc() {
    return "multi_index: failed update of a unique indexed field leaves the "
           "record and indexes unchanged";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    auto _r2{_index.add(object{2, 20.0F, "two"})};

    if (!_r1 || !_r2) {
      return false;
    }

    if (_index.update<0>(_r2.value(), 1)) {
      return false;
    }

    return one_live_by_id(_index, 1, 10.0F, "one") &&
           one_live_by_id(_index, 2, 20.0F, "two");
  }
};

struct multi_index_006 {
  static std::string desc() {
    return "multi_index: update of a non-unique indexed field moves only the "
           "target record";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    auto _r2{_index.add(object{2, 10.0F, "two"})};

    if (!_r1 || !_r2 || !_index.update<1>(_r1.value(), 20.0F)) {
      return false;
    }

    const std::vector<record_ref> _score_10{_index.get<1>(10.0F)};
    const std::vector<record_ref> _score_20{_index.get<1>(20.0F)};

    return (_score_10.size() == 1) &&
           has_object_in(_score_10, 2, 10.0F, "two") &&
           (_score_20.size() == 1) &&
           has_object_in(_score_20, 1, 20.0F, "one");
  }
};

struct multi_index_007 {
  static std::string desc() {
    return "multi_index: erase by non-indexed field removes all matching "
           "records and their indexes";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "same"})};
    auto _r2{_index.add(object{2, 20.0F, "same"})};
    auto _r3{_index.add(object{3, 30.0F, "other"})};

    if (!_r1 || !_r2 || !_r3) {
      return false;
    }

    _index.erase<2>(std::string{"same"});

    return !_r1->get().has_value() && !_r2->get().has_value() &&
           _r3->get().has_value() && _index.get<0>(1).empty() &&
           _index.get<0>(2).empty() && _index.get<1>(10.0F).empty() &&
           _index.get<1>(20.0F).empty() &&
           one_live_by_id(_index, 3, 30.0F, "other");
  }
};

struct multi_index_008 {
  static std::string desc() {
    return "multi_index: add rollback removes an earlier unique index entry "
           "when a later unique index fails";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    rollback_index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    auto _r2{_index.add(object{2, 20.0F, "two"})};

    if (!_r1 || !_r2) {
      return false;
    }

    // id=3 is inserted in field 0. The later unique name index rejects "one".
    auto _failed{_index.add(object{3, 30.0F, "one"})};

    if (_failed.has_value()) {
      return false;
    }

    // This is the important regression check. If rollback left the id=3 map
    // entry behind, this add will incorrectly fail even though get<0>(3)
    // filters the stale reference and looks empty.
    auto _reused{_index.add(object{3, 40.0F, "three"})};

    if (!_reused) {
      return false;
    }

    const std::vector<rollback_record_ref> _id_1{_index.get<0>(1)};
    const std::vector<rollback_record_ref> _id_3{_index.get<0>(3)};

    return (_id_1.size() == 1) &&
           has_object_in(_id_1, 1, 10.0F, "one") &&
           (_id_3.size() == 1) &&
           has_object_in(_id_3, 3, 40.0F, "three") &&
           _index.get<1>(std::string{"three"}).size() == 1;
  }
};

struct multi_index_009 {
  static std::string desc() {
    return "multi_index: erase with a non-existing indexed key is a no-op";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    auto _r2{_index.add(object{2, 20.0F, "two"})};

    if (!_r1 || !_r2) {
      return false;
    }

    _index.erase<0>(999);

    return one_live_by_id(_index, 1, 10.0F, "one") &&
           one_live_by_id(_index, 2, 20.0F, "two");
  }
};

struct multi_index_010 {
  static std::string desc() {
    return "multi_index: erasing the same unique key twice is safe and the key "
           "can be reused";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    if (!_r1) {
      return false;
    }

    _index.erase<0>(1);
    _index.erase<0>(1);

    auto _r2{_index.add(object{1, 20.0F, "two"})};

    return _r2.has_value() &&
           one_live_by_id(_index, 1, 20.0F, "two");
  }
};

struct multi_index_011 {
  static std::string desc() {
    return "multi_index: update of an erased record is rejected";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    if (!_r1) {
      return false;
    }

    _index.erase<0>(1);

    return !_index.update<0>(_r1.value(), 2) &&
           !_r1->get().has_value() && _index.get<0>(2).empty();
  }
};

struct multi_index_012 {
  static std::string desc() {
    return "multi_index: update of a non-indexed field changes attribute lookup "
           "without changing unrelated indexes";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    if (!_r1 ||
        !_index.update<2>(_r1.value(), std::string{"uno"})) {
      return false;
    }

    const std::vector<record_ref> _name_records{
        _index.get<2>(std::string{"uno"})};

    return _index.get<2>(std::string{"one"}).empty() &&
           (_name_records.size() == 1) &&
           has_object_in(_name_records, 1, 10.0F, "uno") &&
           one_live_by_id(_index, 1, 10.0F, "uno") &&
           (_index.get<1>(10.0F).size() == 1);
  }
};

struct multi_index_013 {
  static std::string desc() {
    return "multi_index: updating one record among duplicated multimap keys "
           "does not move the other record";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    index _index;

    auto _r1{_index.add(object{1, 7.0F, "one"})};
    auto _r2{_index.add(object{2, 7.0F, "two"})};

    if (!_r1 || !_r2 || !_index.update<1>(_r1.value(), 8.0F)) {
      return false;
    }

    const std::vector<record_ref> _score_7{_index.get<1>(7.0F)};
    const std::vector<record_ref> _score_8{_index.get<1>(8.0F)};

    return (_score_7.size() == 1) &&
           has_object_in(_score_7, 2, 7.0F, "two") &&
           (_score_8.size() == 1) &&
           has_object_in(_score_8, 1, 8.0F, "one") &&
           one_live_by_id(_index, 1, 8.0F, "one") &&
           one_live_by_id(_index, 2, 7.0F, "two");
  }
};

struct multi_index_014 {
  static std::string desc() {
    return "multi_index: calculated index is refreshed after a non-indexed "
           "field update";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    calculated_index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    if (!_r1 || _index.get<3>(std::string{"1:one"}).size() != 1) {
      return false;
    }

    if (!_index.update<2>(_r1.value(), std::string{"uno"})) {
      return false;
    }

    const std::vector<calculated_record_ref> _records{
        _index.get<3>(std::string{"1:uno"})};

    return _index.get<3>(std::string{"1:one"}).empty() &&
           (_records.size() == 1) &&
           has_object_in(_records, 1, 10.0F, "uno");
  }
};

struct multi_index_015 {
  static std::string desc() {
    return "multi_index: calculated index is refreshed after an indexed field "
           "update";
  }

  bool operator()(const program::bus::options &) {
    using namespace multi_index_test;

    calculated_index _index;

    auto _r1{_index.add(object{1, 10.0F, "one"})};
    if (!_r1 || _index.get<3>(std::string{"1:one"}).size() != 1) {
      return false;
    }

    if (!_index.update<0>(_r1.value(), 2)) {
      return false;
    }

    const std::vector<calculated_record_ref> _records{
        _index.get<3>(std::string{"2:one"})};

    return _index.get<0>(1).empty() &&
           (_index.get<0>(2).size() == 1) &&
           _index.get<3>(std::string{"1:one"}).empty() &&
           (_records.size() == 1) &&
           has_object_in(_records, 2, 10.0F, "one");
  }
};

} // namespace tnct::container::tst

#endif

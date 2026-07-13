/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_MULTI_INDEX_TEST_H
#define TNCT_CONTAINER_TST_MULTI_INDEX_TEST_H

#include <functional>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "tnct/container/dat/multi_index.h"
#include "tnct/container/trt/field_definition.h"
#include "tnct/container/trt/std_map_index_trait.h"
#include "tnct/container/trt/std_multimap_index_trait.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using tnct::format::bus::fmt;

namespace tnct::container::tst {

namespace std_multimap_index_trait_id {

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

  friend std::ostream &operator<<(std::ostream &p_out, const object &p_object) {
    p_out << "{id = " << p_object.m_id << ", score = " << p_object.m_score
          << ", name = " << p_object.m_name << "}";
    return p_out;
  }

  bool operator==(const object &p_object) const {
    return (m_id == p_object.m_id) && (m_score == p_object.m_score) &&
           (m_name == p_object.m_name);
  }

  bool operator!=(const object &p_object) const { return !(*this == p_object); }

  bool operator<(const object &p_object) const {
    return std::tie(m_id, m_score, m_name) <
           std::tie(p_object.m_id, p_object.m_score, p_object.m_name);
  }

private:
  int m_id{0};
  float m_score{0.0F};
  std::string m_name;
};

using tnct::container::trt::attribute_definition;
using tnct::container::trt::calculated_index_definition;
using tnct::container::trt::index_definition;
using tnct::container::trt::std_map_index_trait_id;
using tnct::container::trt::std_multimap_index_trait_id;

using id_field = index_definition<
    object, int,
    decltype([](const object &p_object) -> int { return p_object.get_id(); }),
    decltype([](object &p_object, int p_id) -> void { p_object.set_id(p_id); }),
    std_map_index_trait_id>;

using score_field =
    index_definition<object, float,
                     decltype([](const object &p_object) -> float {
                       return p_object.get_score();
                     }),
                     decltype([](object &p_object, float p_score) -> void {
                       p_object.set_score(p_score);
                     }),
                     std_multimap_index_trait_id>;

using name_field = attribute_definition<
    object, std::string, decltype([](const object &p_object) -> std::string {
      return p_object.get_name();
    }),
    decltype([](object &p_object, std::string p_name) -> void {
      p_object.set_name(std::move(p_name));
    })>;

using id_name_calculated_field = calculated_index_definition<
    object, std::string, decltype([](const object &p_object) -> std::string {
      return std::to_string(p_object.get_id()) + ":" + p_object.get_name();
    }),
    std_multimap_index_trait_id>;

using name_calculated_field = calculated_index_definition<
    object, std::string, decltype([](const object &p_object) -> std::string {
      return p_object.get_name();
    }),
    std_multimap_index_trait_id>;

using logger = tnct::log::cerr;
using index = tnct::container::dat::multi_index_t<logger, id_field, score_field,
                                                  name_field>;

using record_ref = typename index::record_ref;

inline index make_index(logger &p_logger) { return index{p_logger}; }

inline bool has_object(const std::vector<record_ref> &p_records, int p_id,
                       float p_score, std::string_view p_name) {
  for (const record_ref &r : p_records) {
    if (!r.get().get_optional().has_value()) {
      continue;
    }

    const object &obj{r.get().get_optional().value()};

    if ((obj.get_id() == p_id) && (obj.get_score() == p_score) &&
        (obj.get_name() == p_name)) {
      return true;
    }
  }

  return false;
}

inline bool one_live_by_id(index &p_index, int p_id, float p_score,
                           std::string_view p_name) {
  std::vector<record_ref> records{p_index.get<0>(p_id)};

  return (records.size() == 1) && has_object(records, p_id, p_score, p_name);
}

template <typename t_record_ref>
inline bool has_object_in(const std::vector<t_record_ref> &p_records, int p_id,
                          float p_score, std::string_view p_name) {
  for (const t_record_ref &r : p_records) {
    if (!r.get().get_optional().has_value()) {
      continue;
    }

    const object &obj{r.get().get_optional().value()};

    if ((obj.get_id() == p_id) && (obj.get_score() == p_score) &&
        (obj.get_name() == p_name)) {
      return true;
    }
  }

  return false;
}

using rollback_index =
    tnct::container::dat::multi_index_t<logger, score_field, id_field,
                                        name_field>;

using rollback_record_ref = typename rollback_index::record_ref;

using calculated_index =
    tnct::container::dat::multi_index_t<logger, id_field, score_field,
                                        name_field, id_name_calculated_field>;

using calculated_record_ref = typename calculated_index::record_ref;

using calculated_name_index =
    tnct::container::dat::multi_index_t<logger, id_field, score_field,
                                        name_field, name_calculated_field>;

using calculated_name_record_ref = typename calculated_name_index::record_ref;

} // namespace std_multimap_index_trait_id

struct multi_index_001 {
  static std::string desc() {
    return "multi_index: add rejects duplicated key in unique index and leaves "
           "other indexes clean";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});
    if (!r1.has_value()) {
      return false;
    }

    auto r2 = idx.add(object{2, 10.0F, "two"});
    if (!r2.has_value()) {
      return false;
    }

    auto dup = idx.add(object{1, 99.0F, "duplicated-id"});
    if (dup.has_value()) {
      return false;
    }

    if (!one_live_by_id(idx, 1, 10.0F, "one")) {
      return false;
    }

    if (!one_live_by_id(idx, 2, 10.0F, "two")) {
      return false;
    }

    // A failed add must not have polluted the non-unique score index.
    if (!idx.get<1>(99.0F).empty()) {
      return false;
    }

    return idx.get<1>(10.0F).size() == 2;
  }
};

struct multi_index_002 {
  static std::string desc() {
    return "multi_index: erase by unique index removes object from all indexes";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});
    if (!r1.has_value()) {
      return false;
    }

    idx.erase<0>(1);

    if (r1.value().get().get_optional().has_value()) {
      return false;
    }

    if (!idx.get<0>(1).empty()) {
      return false;
    }

    if (!idx.get<1>(10.0F).empty()) {
      return false;
    }

    auto r2 = idx.add(object{1, 20.0F, "one-again"});
    if (!r2.has_value()) {
      return false;
    }

    return one_live_by_id(idx, 1, 20.0F, "one-again");
  }
};

struct multi_index_003 {
  static std::string desc() {
    return "multi_index: erase by non-unique index removes all records in "
           "equal_range";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 7.0F, "one"});
    auto r2 = idx.add(object{2, 7.0F, "two"});
    auto r3 = idx.add(object{3, 7.0F, "three"});

    if (!r1 || !r2 || !r3) {
      return false;
    }

    idx.erase<1>(7.0F);

    if (r1.value().get().get_optional().has_value()) {
      return false;
    }

    if (r2.value().get().get_optional().has_value()) {
      return false;
    }

    if (r3.value().get().get_optional().has_value()) {
      return false;
    }

    if (!idx.get<1>(7.0F).empty()) {
      return false;
    }

    if (!idx.get<0>(1).empty()) {
      return false;
    }

    if (!idx.get<0>(2).empty()) {
      return false;
    }

    return idx.get<0>(3).empty();
  }
};

struct multi_index_004 {
  static std::string desc() {
    return "multi_index: update unique indexed field to free key updates old "
           "and new indexes";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});
    if (!r1.has_value()) {
      return false;
    }

    idx.update<0>(r1.value(), 10);

    if (!idx.get<0>(1).empty()) {
      return false;
    }

    if (!one_live_by_id(idx, 10, 10.0F, "one")) {
      return false;
    }

    std::vector<record_ref> score_records{idx.get<1>(10.0F)};

    return (score_records.size() == 1) &&
           has_object(score_records, 10, 10.0F, "one");
  }
};

struct multi_index_005 {
  static std::string desc() {
    return "multi_index: update unique indexed field to occupied causes update "
           "to fail";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});
    auto r2 = idx.add(object{2, 20.0F, "two"});

    if (!r1 || !r2) {
      return false;
    }

    const bool _updated{idx.update<0>(r1.value(), 2)};

    if (_updated) {
      return false;
    }

    if (!one_live_by_id(idx, 1, 10.0F, "one")) {
      return false;
    }

    if (!one_live_by_id(idx, 2, 20.0F, "two")) {
      return false;
    }

    const object &obj{r1.value().get().get_optional().value()};

    return (obj.get_id() == 1) && (obj.get_score() == 10.0F) &&
           (obj.get_name() == "one");
  }
};

struct multi_index_006 {
  static std::string desc() {
    return "multi_index: update non-unique indexed field moves record between "
           "score keys";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});
    if (!r1.has_value()) {
      return false;
    }

    idx.update<1>(r1.value(), 30.0F);

    if (!idx.get<1>(10.0F).empty()) {
      return false;
    }

    std::vector<record_ref> records{idx.get<1>(30.0F)};

    return (records.size() == 1) && has_object(records, 1, 30.0F, "one") &&
           one_live_by_id(idx, 1, 30.0F, "one");
  }
};

struct multi_index_007 {
  static std::string desc() {
    return "multi_index: erase by non-indexed field removes matching records "
           "from indexes";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "same"});
    auto r2 = idx.add(object{2, 20.0F, "same"});
    auto r3 = idx.add(object{3, 30.0F, "other"});

    if (!r1) {
      TNCT_LOG_ERR(_logger, fmt("r1 should have been added, but it was not "));
      return false;
    }

    if (!r2) {
      TNCT_LOG_ERR(_logger, fmt("r2 should have been added, but it was not "));
      return false;
    }

    if (!r3) {
      TNCT_LOG_ERR(_logger, fmt("r3 should have been added, but it was not "));
      return false;
    }

    idx.erase<2>(std::string{"same"});

    if (r1.value().get().get_optional().has_value()) {
      TNCT_LOG_ERR(_logger, fmt("there should not be r1, but there is ",
                                r1.value().get().get_optional().value()));
      return false;
    }

    if (r2.value().get().get_optional().has_value()) {
      TNCT_LOG_ERR(_logger, fmt("there should not be r2, but there is ",
                                r2.value().get().get_optional().value()));
      return false;
    }

    if (!r3.value().get().get_optional().has_value()) {
      TNCT_LOG_ERR(_logger, fmt("there should be r3, but there is not"));
      return false;
    }

    if (!idx.get<0>(1).empty()) {
      TNCT_LOG_ERR(_logger, fmt("idx.get<0>(1) should return, but it did not"));
      return false;
    }

    if (!idx.get<0>(2).empty()) {
      TNCT_LOG_ERR(_logger, fmt("idx.get<0>(2) should return, but it did not"));
      return false;
    }

    if (!idx.get<1>(10.0F).empty()) {
      TNCT_LOG_ERR(_logger,
                   fmt("idx.get<1>(10.0F) should return, but it did not"));
      return false;
    }

    if (!idx.get<1>(20.0F).empty()) {
      TNCT_LOG_ERR(_logger,
                   fmt("idx.get<1>(20.0F) should return, but it did not"));
      return false;
    }

    if (!one_live_by_id(idx, 3, 30.0F, "other")) {
      TNCT_LOG_ERR(
          _logger,
          fmt("There should be one {3, 30.0F, \"other\"}, but there is not"));
      return false;
    }

    return true;
  }
};

struct multi_index_008 {
  static std::string desc() {
    return "multi_index: add rollback removes indexes already inserted before "
           "a later unique index fails";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    rollback_index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});
    auto r2 = idx.add(object{2, 20.0F, "two"});

    if (!r1 || !r2) {
      return false;
    }

    // In rollback_index, field 0 is score/multimap and field 1 is id/map.
    // Therefore this insertion first succeeds in score, then fails in id.
    auto dup = idx.add(object{1, 99.0F, "duplicated-id"});

    if (dup.has_value()) {
      return false;
    }

    if (!idx.get<0>(99.0F).empty()) {
      return false;
    }

    std::vector<rollback_record_ref> id_1_records{idx.get<1>(1)};
    std::vector<rollback_record_ref> id_2_records{idx.get<1>(2)};

    if ((id_1_records.size() != 1) ||
        !has_object_in(id_1_records, 1, 10.0F, "one")) {
      return false;
    }

    if ((id_2_records.size() != 1) ||
        !has_object_in(id_2_records, 2, 20.0F, "two")) {
      return false;
    }

    return (idx.get<0>(10.0F).size() == 1) && (idx.get<0>(20.0F).size() == 1);
  }
};

struct multi_index_009 {
  static std::string desc() {
    return "multi_index: erase with non-existing indexed key is a no-op";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});
    auto r2 = idx.add(object{2, 20.0F, "two"});

    if (!r1 || !r2) {
      return false;
    }

    idx.erase<0>(999);

    if (!r1.value().get().get_optional().has_value()) {
      return false;
    }

    if (!r2.value().get().get_optional().has_value()) {
      return false;
    }

    return one_live_by_id(idx, 1, 10.0F, "one") &&
           one_live_by_id(idx, 2, 20.0F, "two");
  }
};

struct multi_index_010 {
  static std::string desc() {
    return "multi_index: erasing the same indexed key twice is safe";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});

    if (!r1) {
      return false;
    }

    idx.erase<0>(1);
    idx.erase<0>(1);

    if (r1.value().get().get_optional().has_value()) {
      return false;
    }

    if (!idx.get<0>(1).empty()) {
      return false;
    }

    if (!idx.get<1>(10.0F).empty()) {
      return false;
    }

    auto r2 = idx.add(object{1, 30.0F, "one-again"});

    return r2.has_value() && one_live_by_id(idx, 1, 30.0F, "one-again");
  }
};

struct multi_index_011 {
  static std::string desc() {
    return "multi_index: update on deleted record returns false and does not "
           "reinsert it";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});

    if (!r1) {
      return false;
    }

    idx.erase<0>(1);

    const bool updated{idx.update<0>(r1.value(), 2)};

    if (updated) {
      return false;
    }

    if (r1.value().get().get_optional().has_value()) {
      return false;
    }

    return idx.get<0>(1).empty() && idx.get<0>(2).empty() &&
           idx.get<1>(10.0F).empty();
  }
};

struct multi_index_012 {
  static std::string desc() {
    return "multi_index: update non-indexed field changes field lookup without "
           "changing indexes";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});

    if (!r1) {
      return false;
    }

    const bool updated{idx.update<2>(r1.value(), std::string{"uno"})};

    if (!updated) {
      return false;
    }

    if (!idx.get<2>(std::string{"one"}).empty()) {
      return false;
    }

    std::vector<record_ref> name_records{idx.get<2>(std::string{"uno"})};

    if ((name_records.size() != 1) ||
        !has_object_in(name_records, 1, 10.0F, "uno")) {
      return false;
    }

    return one_live_by_id(idx, 1, 10.0F, "uno") &&
           (idx.get<1>(10.0F).size() == 1);
  }
};

struct multi_index_013 {
  static std::string desc() {
    return "multi_index: update one record among duplicated multimap keys";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    index idx{_logger};

    auto r1 = idx.add(object{1, 7.0F, "one"});
    auto r2 = idx.add(object{2, 7.0F, "two"});

    if (!r1 || !r2) {
      return false;
    }

    const bool updated{idx.update<1>(r1.value(), 8.0F)};

    if (!updated) {
      return false;
    }

    std::vector<record_ref> score_7_records{idx.get<1>(7.0F)};
    std::vector<record_ref> score_8_records{idx.get<1>(8.0F)};

    if ((score_7_records.size() != 1) ||
        !has_object_in(score_7_records, 2, 7.0F, "two")) {
      return false;
    }

    if ((score_8_records.size() != 1) ||
        !has_object_in(score_8_records, 1, 8.0F, "one")) {
      return false;
    }

    return one_live_by_id(idx, 1, 8.0F, "one") &&
           one_live_by_id(idx, 2, 7.0F, "two");
  }
};

struct multi_index_014 {
  static std::string desc() {
    return "multi_index: calculated index is updated after non-indexed field "
           "update";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    calculated_index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});

    if (!r1) {
      return false;
    }

    if (idx.get<3>(std::string{"1:one"}).size() != 1) {
      return false;
    }

    const bool updated{idx.update<2>(r1.value(), std::string{"uno"})};

    if (!updated) {
      return false;
    }

    if (!idx.get<3>(std::string{"1:one"}).empty()) {
      return false;
    }

    std::vector<calculated_record_ref> records{
        idx.get<3>(std::string{"1:uno"})};

    return (records.size() == 1) && has_object_in(records, 1, 10.0F, "uno");
  }
};

struct multi_index_015 {
  static std::string desc() {
    return "multi_index: failed calculated index update leaves record and "
           "indexes unchanged";
  }

  bool operator()(const program::bus::options &) {
    using namespace std_multimap_index_trait_id;

    logger _logger;
    calculated_name_index idx{_logger};

    auto r1 = idx.add(object{1, 10.0F, "one"});

    auto r2 = idx.add(object{2, 20.0F, "two"});

    if (!r1) {
      TNCT_LOG_ERR(_logger, fmt("r1 should have been added, but it was not "));
      return false;
    }

    if (!r2) {
      TNCT_LOG_ERR(_logger, fmt("r2 should have been added, but it was not "));
      return false;
    }

    const bool updated{idx.update<2>(r2.value(), std::string{"one"})};

    if (!updated) {
      TNCT_LOG_ERR(_logger, fmt("it should not have updated, but it dit"));
      return false;
    }

    const object &obj_2{r2.value().get().get_optional().value()};

    if ((obj_2.get_id() != 2) || (obj_2.get_score() != 20.0F) ||
        (obj_2.get_name() != "one")) {
      TNCT_LOG_ERR(_logger,
                   fmt("object should be {2,20.0,\"one\"}, but it is ", obj_2));
      return false;
    }

    std::vector<calculated_name_record_ref> name_one_records{
        idx.get<3>(std::string{"one"})};

    std::vector<calculated_name_record_ref> name_two_records{
        idx.get<3>(std::string{"two"})};

    const bool _has_object_in_1{
        has_object_in(name_one_records, 1, 10.0F, "one")};

    const bool _has_object_in_2{
        has_object_in(name_two_records, 2, 20.0F, "two")};

    TNCT_LOG_TST(_logger,
                 fmt("name_one_records.size() = ", name_one_records.size(),
                     ", _has_object_in = ", _has_object_in_1,
                     ", name_two_records.size() = ", name_two_records.size(),
                     ", _has_object_in_2 = ", _has_object_in_2));

    return (name_one_records.size() == 2) && _has_object_in_1 &&
           (name_two_records.size() == 0) && !_has_object_in_2;
  }
};

} // namespace tnct::container::tst

#endif

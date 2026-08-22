/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include "tnct/container/dat/multi_index.h"
#include "tnct/container/trt/field_definition.h"
#include "tnct/container/trt/std_map_definition.h"
#include "tnct/container/trt/std_multimap_definition.h"

struct xpto {
  xpto() = default;
  xpto(const xpto &) = default;
  xpto(xpto &&) = default;
  xpto(int p_i, float p_f, std::string_view p_s)
      : m_i(p_i), m_f(p_f), m_s(p_s) {}

  int get_i() const { return m_i; }
  void set_i(int p_i) { m_i = p_i; }

  float get_f() const { return m_f; }
  void set_f(float p_f) { m_f = p_f; }

  std::string get_s() const { return m_s; }
  void set_s(std::string_view p_s) { m_s = p_s; }

  xpto &operator=(const xpto &) = default;
  xpto &operator=(xpto &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out, const xpto &p_xpto) {
    p_out << "{i = " << p_xpto.get_i() << ", " << "f = " << p_xpto.get_f()
          << ", s = " << p_xpto.m_s << "}";
    return p_out;
  }

  constexpr bool operator==(const xpto &p_xpto) const {
    return (m_i == p_xpto.m_i) && (m_f == p_xpto.m_f) && (m_s == p_xpto.m_s);
  }
  constexpr bool operator!=(const xpto &p_xpto) const {
    return !(*this == p_xpto);
  }

  constexpr bool operator<(const xpto &p_xpto) const {
    if (m_i < p_xpto.m_i) {
      return true;
    }
    if (m_i > p_xpto.m_i) {
      return false;
    }
    return (m_f < p_xpto.m_f);
  }

private:
  int m_i{-9};
  float m_f{3.14};
  std::string m_s{"hi"};
};

using tnct::container::trt::attribute_field_definition;
using tnct::container::trt::calculated_index_definition;
using tnct::container::trt::index_field_definition;
using tnct::container::trt::std_map_index_id;
using tnct::container::trt::std_multimap_index_id;

using field_0 = index_field_definition<
    xpto, int,
    decltype([](const xpto &p_xpto) -> int { return p_xpto.get_i(); }),
    decltype([](xpto &p_xpto, int p_i) -> void { p_xpto.set_i(p_i); }),
    std_map_index_id>;

using field_1 = index_field_definition<
    xpto, float,
    decltype([](const xpto &p_xpto) -> float { return p_xpto.get_f(); }),
    decltype([](xpto &p_xpto, float p_f) -> void { p_xpto.set_f(p_f); }),
    std_multimap_index_id>;

using field_2 = attribute_field_definition<
    xpto, std::string,
    decltype([](const xpto &p_xpto) -> std::string { return p_xpto.get_s(); }),
    decltype([](xpto &p_xpto, std::string p_s) -> void { p_xpto.set_s(p_s); })>;

using field_3 =
    calculated_index_definition<xpto, float,
                                decltype([](const xpto &p_xpto) -> float {
                                  return static_cast<float>(p_xpto.get_f() *
                                                            p_xpto.get_i());
                                }),
                                std_multimap_index_id>;

using xpto_indexes =
    tnct::container::dat::multi_index<field_0, field_1, field_2, field_3>;

// using xpto_indexes = tnct::container::dat::multi_index<field_0>;

using rec_opt_ref = xpto_indexes::rec_opt_ref;

int main() {

  xpto_indexes _xpto_indexes;

  std::cout << "\n###### Adding\n";
  _xpto_indexes.add(xpto{-9, 3.14, "hi"});
  _xpto_indexes.add(xpto{138, 0.72, "hello"});
  _xpto_indexes.add(xpto{138, -4.21, "bye"});
  _xpto_indexes.add(xpto{-2, -7.85, "see u"});
  _xpto_indexes.add(xpto{31, 9.012, "ouch"});

  std::cout << _xpto_indexes << std::endl;

  std::cout << "\n###### Searching in index 0 key 138\n";
  std::vector<rec_opt_ref> _rec_opt_refs{_xpto_indexes.get<0>(138)};
  std::for_each(_rec_opt_refs.begin(), _rec_opt_refs.end(),
                [](const rec_opt_ref &p_record) {
                  if (p_record.get().has_value()) {
                    std::cout << "record = " << p_record.get().value()
                              << std::endl;
                  }
                });

  std::cout
      << "\n###### Searching in index 1 key -4.21 that will not be  found "
         "because 'xpto{138, -4.21, \"bye\"'} was not added because "
         "'xpto{138, 0.72, \"hello\"});' was added first  \n";
  _rec_opt_refs = _xpto_indexes.get<1>(-4.21);
  std::for_each(_rec_opt_refs.begin(), _rec_opt_refs.end(),
                [](const rec_opt_ref &p_record) {
                  if (p_record.get().has_value()) {
                    std::cout << "record = " << p_record.get().value()
                              << std::endl;
                  }
                });

  std::cout << "\n###### Searching in non index field 2 field 'see u'\n";
  _rec_opt_refs = _xpto_indexes.get<2>("see u");
  std::for_each(_rec_opt_refs.begin(), _rec_opt_refs.end(),
                [](const rec_opt_ref &p_record) {
                  if (p_record.get().has_value()) {
                    std::cout << "record = " << p_record.get().value()
                              << std::endl;
                  }
                });

  std::cout << "\n###### Deleting from index 1 key 3.14\n";
  _xpto_indexes.erase<1>(float{3.14});
  std::cout << _xpto_indexes << std::endl;

  std::cout << "\n###### Deleting from non index field 2 field 'ouch'\n";
  _xpto_indexes.erase<2>("ouch");
  std::cout << _xpto_indexes << std::endl;

  const xpto _expected{-2, -73.45f, "see u"};
  std::cout << "\n###### Find from index 0 where key is -2, update field 1 "
               "from -7.85 to -73.45 and confirm that the xpto indexed 2 is "
            << _expected << "\n";
  _rec_opt_refs = _xpto_indexes.get<0>(-2);
  if (_rec_opt_refs.size() != 1) {
    std::cout << "ERROR: found " << _rec_opt_refs.size() << std::endl;
    return -1;
  }

  rec_opt_ref _ref{_rec_opt_refs[0]};
  _xpto_indexes.update<1>(_ref, -73.45);

  _rec_opt_refs = _xpto_indexes.get<3>(146.9);
  if (_rec_opt_refs.size() != 1) {
    std::cout << "ERROR: found " << _rec_opt_refs.size() << std::endl;
    return -1;
  }

  if (!_rec_opt_refs[0].get().has_value()) {
    std::cout << "ERROR: no value";
  } else if (_rec_opt_refs[0].get().value().get_object() != _expected) {
    std::cout << "ERROR: xpto should be " << _expected << ", but it is "
              << _rec_opt_refs[0].get().value().get_object() << std::endl;
  }

  std::cout << _xpto_indexes << std::endl;

  std::cout << "\n###### Find from index 1 where key is 0.72 and update field "
               "2 from 'hello' to 'hi!!'\n";
  _rec_opt_refs = _xpto_indexes.get<1>(0.72);
  if (_rec_opt_refs.size() != 1) {
    std::cout << "ERROR: found " << _rec_opt_refs.size() << std::endl;
    return -1;
  }

  _ref = _rec_opt_refs[0];
  _xpto_indexes.update<2>(_ref, "hi!!");

  std::cout << _xpto_indexes << std::endl;
}

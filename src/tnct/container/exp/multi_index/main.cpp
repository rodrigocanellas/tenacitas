/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include "tnct/container/dat/index_definition.h"
#include "tnct/container/dat/multi_index.h"
#include "tnct/container/trt/index_type.h"

struct xpto {
  xpto() = default;
  xpto(const xpto &) = default;
  xpto(xpto &&) = default;
  xpto(int p_i, float p_f, std::string_view p_s)
      : m_i(p_i), m_f(p_f), m_s(p_s) {}

  int get_i() const { return m_i; }
  float get_f() const { return m_f; }
  std::string get_s() const { return m_s; }

  void set_f(float p_f) { m_f = p_f; }

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

using tnct::container::dat::index_definition;
using tnct::container::trt::std_multimap_id;

using xpto_indexes = tnct::container::dat::multi_index_t<
    xpto,
    index_definition<std_multimap_id, xpto, int,
                     decltype([](const xpto &p_xpto) -> int {
                       return p_xpto.get_i();
                     })>,
    index_definition<std_multimap_id, xpto, float,
                     decltype([](const xpto &p_xpto) -> float {
                       return p_xpto.get_f();
                     })>>;

using record_ref = xpto_indexes::record_ref;

// using xpto_ref = typename xpto_indexes::ref;

int main() {
  xpto_indexes _xpto_indexes;

  std::cout << "\n###### Adding\n";
  _xpto_indexes.add(xpto{-9, 3.14, "hi"});
  _xpto_indexes.add(xpto{138, 0.72, "hello"});
  _xpto_indexes.add(xpto{138, -4.21, "bye"});
  _xpto_indexes.add(xpto{-2, -7.85, "see u"});

  std::cout << _xpto_indexes << std::endl;

  std::cout << "\n###### Searching in index 0 key 138\n";
  std::vector<record_ref> _records{_xpto_indexes.get<0>(138)};

  std::for_each(
      _records.begin(), _records.end(), [](const record_ref &p_record) {
        std::cout << "object = " << p_record.get().get_optional().value()
                  << '\n';
      });

  std::cout << "\n###### Searching in index 1 key -4.21\n";
  _records = _xpto_indexes.get<1>(-4.21);

  std::for_each(
      _records.begin(), _records.end(), [](const record_ref &p_record) {
        std::cout << "object = " << p_record.get().get_optional().value()
                  << '\n';
      });

  std::cout << "\n###### Deleting from index 1 key 3.14\n";
  _xpto_indexes.erase<1>(float{3.14});

  std::cout << _xpto_indexes << std::endl;

  std::cout << "\n###### Find from index 0 where key is -2 and update field 1 "
               "from -7.85 to 0.38\n";
  _records = _xpto_indexes.get<0>(-2);
  if (_records.size() != 1) {
    std::cout << "ERROR: found " << _records.size() << std::endl;
    return -1;
  }

  record_ref _ref{_records[0]};
  _xpto_indexes.update(_ref, [](xpto &p_xpto) { p_xpto.set_f(0.38); });
  std::cout << _xpto_indexes << std::endl;
}

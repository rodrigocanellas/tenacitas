/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include "tnct/container/dat/multi_index.h"

struct xpto {
  xpto() = default;
  xpto(const xpto &) = default;
  xpto(xpto &&) = default;
  xpto(int p_i, float p_f, std::string_view p_s)
      : m_i(p_i), m_f(p_f), m_s(p_s) {}

  int get_i() const { return m_i; }
  float get_f() const { return m_f; }
  std::string get_s() const { return m_s; }

  xpto &operator=(const xpto &) = default;
  xpto &operator=(xpto &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out, const xpto &p_xpto) {
    p_out << "{i = " << p_xpto.get_i() << ", " << "f = " << p_xpto.get_f()
          << ", s = " << p_xpto.m_s << "}";
    return p_out;
  }

  constexpr bool operator<(const xpto &p_xpto) const {
    if (m_i < p_xpto.m_i) {
      return true;
    }
    if (m_i > p_xpto.m_i) {
      return false;
    }
    if (m_f < p_xpto.m_f) {
      return true;
    }
    if (m_f > p_xpto.m_f) {
      return true;
    }
    if (m_s < p_xpto.m_s) {
      return true;
    }
    return false;
  }

  constexpr bool operator==(const xpto &p_xpto) const {
    return (m_i == p_xpto.m_i) && (m_f == p_xpto.m_f) && (m_s == p_xpto.m_s);
  }
  constexpr bool operator!=(const xpto &p_xpto) const {
    return !(*this == p_xpto);
  }

private:
  int m_i{-9};
  float m_f{3.14};
  std::string m_s{"hi"};
};

using xpto_indexes =
    tnct::container::dat::multi_index_t<xpto, int, float,
                                        std::pair<float, std::string>>;
using xpto_indexes_iterator = typename xpto_indexes::iterator;

int main() {

  xpto_indexes _xpto_indexes{
      [](const xpto &p_xpto) -> int { return p_xpto.get_i(); },
      [](const xpto &p_xpto) -> float { return p_xpto.get_f(); },
      [](const xpto &p_xpto) -> std::pair<float, std::string> {
        return {p_xpto.get_f(), p_xpto.get_s()};
      }};

  _xpto_indexes.add(xpto{});
  _xpto_indexes.add(xpto{138, 0.72, "hello"});
  _xpto_indexes.add(xpto{138, -4.21, "bye"});
  _xpto_indexes.add(xpto{-2, -4.21, "bye"});

  std::cout << _xpto_indexes << std::endl;

  std::cout << "\nSearch 1\n";
  std::vector<xpto_indexes_iterator> _iterators{_xpto_indexes.get<0>(138)};

  std::for_each(_iterators.begin(), _iterators.end(),
                [](const xpto_indexes_iterator &p_ite) {
                  std::cout << "object = " << *p_ite << '\n';
                });

  std::cout << "\nSearch 2\n";
  _iterators = _xpto_indexes.get<2>({-4.21, "bye"});

  std::for_each(_iterators.begin(), _iterators.end(),
                [](const xpto_indexes_iterator &p_ite) {
                  std::cout << "object = " << *p_ite << '\n';
                });
}

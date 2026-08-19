/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_MULTI_INDEX_CPT_TEST_H
#define TNCT_CONTAINER_TST_MULTI_INDEX_CPT_TEST_H

#include "tnct/container/cpt/multi_index.h"
#include "tnct/container/dat/multi_index.h"
#include "tnct/container/trt/field_definition.h"
#include "tnct/container/trt/std_map_definition.h"
#include "tnct/container/trt/std_multimap_definition.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/program/bus/options.h"
#include "tnct/tester/bus/test.h"

namespace tnct::container::tst {

using tnct::container::trt::attribute_field_definition;
using tnct::container::trt::calculated_index_definition;
using tnct::container::trt::index_field_definition;
using tnct::container::trt::std_map_index_id;
using tnct::container::trt::std_multimap_index_id;

namespace internal {
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

using field_0 =
    index_field_definition<internal::xpto, int,
                     decltype([](const internal::xpto &p_xpto) -> int {
                       return p_xpto.get_i();
                     }),
                     decltype([](internal::xpto &p_xpto, int p_i) -> void {
                       p_xpto.set_i(p_i);
                     }),
                     std_map_index_id>;

using field_1 =
    index_field_definition<internal::xpto, float,
                     decltype([](const internal::xpto &p_xpto) -> float {
                       return p_xpto.get_f();
                     }),
                     decltype([](internal::xpto &p_xpto, float p_f) -> void {
                       p_xpto.set_f(p_f);
                     }),
                     std_multimap_index_trait_id>;

using field_2 = attribute_field_definition<
    internal::xpto, std::string,
    decltype([](const internal::xpto &p_xpto) -> std::string {
      return p_xpto.get_s();
    }),
    decltype([](internal::xpto &p_xpto, std::string p_s) -> void {
      p_xpto.set_s(p_s);
    })>;

using field_3 = calculated_index_definition<
    internal::xpto, float, decltype([](const internal::xpto &p_xpto) -> float {
      return static_cast<float>(p_xpto.get_f() * p_xpto.get_i());
    }),
    std_multimap_index_trait_id>;

using logger = tnct::log::cerr;
using xpto_indexes =
    tnct::container::dat::ref<logger, field_0, field_1, field_2,
                                        field_3>;
} // namespace internal

struct multi_index_cpt_000 {

  static std::string desc() { return "has get method"; }

  bool operator()(const program::bus::options &) {

    static_assert(
        container::cpt::internal::has_get_method<internal::xpto_indexes, 1>);
    static_assert(
        container::cpt::internal::has_get_methods<internal::xpto_indexes>);

    return true;
  }
};

struct multi_index_cpt_001 {

  static std::string desc() { return "does not have get method"; }

  bool operator()(const program::bus::options &) {

    static_assert(!container::cpt::internal::has_get_method<abc, 1>);
    static_assert(!container::cpt::internal::has_get_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using record_ref = float;

    using fields_definitions = std::tuple<char, std::string>;

    template <std::size_t t_field_pos>
    std::vector<record_ref> get(const field_t<t_field_pos> &, char) {
      return {};
    }
  };
};

struct multi_index_cpt_002 {

  static std::string desc() { return "has erase method"; }

  bool operator()(const program::bus::options &) {

    static_assert(
        container::cpt::internal::has_erase_method<internal::xpto_indexes, 1>);
    static_assert(
        container::cpt::internal::has_erase_methods<internal::xpto_indexes>);

    return true;
  }
};

struct multi_index_cpt_003 {

  static std::string desc() { return "does not have erase method"; }

  bool operator()(const program::bus::options &) {

    static_assert(!container::cpt::internal::has_erase_method<abc, 1>);
    static_assert(!container::cpt::internal::has_erase_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using record_ref = float;

    using fields_definitions = std::tuple<char, std::string>;

    template <std::size_t t_field_pos>
    void erase(const field_t<t_field_pos> &, char) {
      return;
    }
  };
};

struct multi_index_cpt_004 {

  static std::string desc() { return "has update method"; }

  bool operator()(const program::bus::options &) {

    static_assert(
        container::cpt::internal::has_update_method<internal::xpto_indexes, 1>);
    static_assert(
        container::cpt::internal::has_update_methods<internal::xpto_indexes>);

    return true;
  }
};

struct multi_index_cpt_005 {

  static std::string desc() { return "does not have update method"; }

  bool operator()(const program::bus::options &) {

    static_assert(!container::cpt::internal::has_update_method<abc, 1>);
    static_assert(!container::cpt::internal::has_update_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using record_ref = float;

    using fields_definitions = std::tuple<char, std::string>;

    template <std::size_t t_field_pos>
    bool update(record_ref, const field_t<t_field_pos> &, char) {
      return true;
    }
  };
};

struct multi_index_cpt_006 {

  static std::string desc() { return "has correct fields_definition type "; }

  bool operator()(const program::bus::options &) {

    static_assert(container::cpt::internal::only_fields_definitions<
                  internal::xpto_indexes>);

    return true;
  }
};

struct multi_index_cpt_007 {

  static std::string desc() { return "is cpt::multi_index"; }

  bool operator()(const program::bus::options &) {

    static_assert(container::cpt::multi_index<internal::xpto_indexes>);

    return true;
  }
};

struct multi_index_cpt_008 {

  static std::string desc() { return "get has wrong return type"; }

  bool operator()(const program::bus::options &) {

    static_assert(!container::cpt::internal::has_get_method<abc, 0>);
    static_assert(!container::cpt::internal::has_get_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using record_ref = float;
    using fields_definitions = std::tuple<char>;

    template <std::size_t t_field_pos> bool get(const field_t<t_field_pos> &) {
      return true;
    }
  };
};

struct multi_index_cpt_009 {

  static std::string desc() { return "does not have all get methods"; }

  bool operator()(const program::bus::options &) {

    static_assert(container::cpt::internal::has_get_method<abc, 0>);
    static_assert(!container::cpt::internal::has_get_method<abc, 1>);

    static_assert(!container::cpt::internal::has_get_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using record_ref = float;
    using fields_definitions = std::tuple<char, std::string>;

    template <std::size_t t_field_pos>
      requires(t_field_pos == 0)
    std::vector<record_ref> get(const field_t<t_field_pos> &) {
      return {};
    }
  };
};

struct multi_index_cpt_010 {

  static std::string desc() { return "erase has wrong return type"; }

  bool operator()(const program::bus::options &) {

    static_assert(!container::cpt::internal::has_erase_method<abc, 0>);
    static_assert(!container::cpt::internal::has_erase_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using fields_definitions = std::tuple<char>;

    template <std::size_t t_field_pos>
    bool erase(const field_t<t_field_pos> &) {
      return true;
    }
  };
};

struct multi_index_cpt_011 {

  static std::string desc() { return "does not have all erase methods"; }

  bool operator()(const program::bus::options &) {

    static_assert(container::cpt::internal::has_erase_method<abc, 0>);
    static_assert(!container::cpt::internal::has_erase_method<abc, 1>);
    static_assert(!container::cpt::internal::has_erase_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using fields_definitions = std::tuple<char, std::string>;

    template <std::size_t t_field_pos>
      requires(t_field_pos == 0)
    void erase(const field_t<t_field_pos> &) {}
  };
};

struct multi_index_cpt_012 {

  static std::string desc() { return "update has wrong return type"; }

  bool operator()(const program::bus::options &) {

    static_assert(!container::cpt::internal::has_update_method<abc, 0>);
    static_assert(!container::cpt::internal::has_update_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using record_ref = float;
    using fields_definitions = std::tuple<char>;

    template <std::size_t t_field_pos>
    int update(record_ref, const field_t<t_field_pos> &) {
      return 0;
    }
  };
};

struct multi_index_cpt_013 {

  static std::string desc() { return "does not have all update methods"; }

  bool operator()(const program::bus::options &) {

    static_assert(container::cpt::internal::has_update_method<abc, 0>);
    static_assert(!container::cpt::internal::has_update_method<abc, 1>);
    static_assert(!container::cpt::internal::has_update_methods<abc>);

    return true;
  }

private:
  struct abc {
    template <std::size_t t_field_pos> using field_t = int;

    using record_ref = float;
    using fields_definitions = std::tuple<char, std::string>;

    template <std::size_t t_field_pos>
      requires(t_field_pos == 0)
    bool update(record_ref, const field_t<t_field_pos> &) {
      return true;
    }
  };
};

struct multi_index_cpt_014 {

  static std::string desc() { return "field types match field definitions"; }

  bool operator()(const program::bus::options &) {

    static_assert(
        container::cpt::internal::field_types_match<internal::xpto_indexes>);

    static_assert(!container::cpt::internal::field_types_match<abc>);

    return true;
  }

private:
  struct abc {
    using fields_definitions = std::tuple<internal::field_0>;

    template <std::size_t t_field_pos> using field_t = float;
  };
};

struct multi_index_cpt_015 {

  static std::string desc() { return "int is not a multi index"; }

  bool operator()(const program::bus::options &) {

    static_assert(!container::cpt::multi_index<int>);

    return true;
  }
};
} // namespace tnct::container::tst

#endif

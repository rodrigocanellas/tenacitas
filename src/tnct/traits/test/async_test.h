/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TRAITS_TEST_ASYNC_TEST_H
#define TNCT_TRAITS_TEST_ASYNC_TEST_H

#include <tuple>

#include "tnct/log/cerr.h"
#include "tnct/program/options.h"
#include "tnct/traits/async/dispatcher.h"
#include "tnct/traits/async/event.h"
#include "tnct/traits/async/publish_method.h"

using namespace tnct;

namespace tnct::traits::test {

struct dispatcher_000 {

  static std::string desc() { return "Basic traitrs::dispatcher test"; }

  bool operator()(const program::options &) {
    tnct::log::cerr _logger;

    my_dispatcher _dispatcher;

    f(_dispatcher);

    return true;
  }

private:
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };

  struct event_b {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &) {
      return p_out;
    }
  };

  struct event_c {
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &) {
      return p_out;
    }
  };

  template <traits::async::event... t_events> struct dispatcher {
    using events = std::tuple<t_events...>;

    dispatcher() = default;
    dispatcher(const dispatcher &) = delete;
    dispatcher(dispatcher &&) = delete;

    dispatcher &operator=(const dispatcher &) = delete;
    dispatcher &operator=(dispatcher &&) = delete;

    void *operator new(size_t) = delete;
  };

  using my_dispatcher = dispatcher<event_a, event_b>;

  template <traits::async::dispatcher<event_a, event_b> t_dispatcher>
  void f(t_dispatcher &) {}
};

struct has_publish_method_000 {
  static std::string desc() { return "A valid has_publish_method"; }

  bool operator()(const program::options &) {
    using my_publisher = publisher<event_a>;
    my_publisher _publisher;
    event_a _event_a;

    result _result{f(_publisher, _event_a)};

    std::cerr << "result = "
              << static_cast<std::underlying_type_t<result>>(_result) << '\n';

    return true;
  }

private:
  enum class result : int { r0 = 1, r1 = -1 };
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };

  template <traits::async::event... t_events> struct publisher {
    template <traits::async::event t_event> result publish(const t_event &) {
      return result::r0;
    }
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_a &p_event_a) {
    return p_publisher.publish(p_event_a);
  };
};

struct has_publish_method_001 {
  static std::string desc() { return "A invalid has_publish_method"; }

  bool operator()(const program::options &) {

    // The code below does not compile because 'publisher' has no 'publish'
    // method

    // using my_publisher = publisher<event_a>;
    // my_publisher _publisher;
    // event_a _event_a;

    // result _result{f(_publisher, _event_a)};

    // std::cerr << "result = "
    //           << static_cast<std::underlying_type_t<result>>(_result) <<
    //           '\n';

    return true;
  }

private:
  enum class result : int { r0 = 1, r1 = -1 };
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };
  template <traits::async::event... t_events> struct publisher {

    // template <traits::async::event t_event> result publish(const t_event &) {
    //   return result::r0;
    // }
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_a &p_event_a) {
    return p_publisher.publish(p_event_a);
  };
};

struct has_publish_method_002 {
  static std::string desc() { return "Checks publihing with multiples events"; }

  bool operator()(const program::options &) {
    static_assert(
        traits::async::has_const_lvalue_publish_method<publisher, result,
                                                       event_a>,
        "publisher should match has_const_lvalue_publish for event_a!");

    publisher _publisher;

    event_b _event_b;
    auto _result{f(_publisher, _event_b)};
    if (_result != result::b) {
      return false;
    }

    event_a _event_a;
    _result = f(_publisher, _event_a);
    if (_result != result::a) {
      return false;
    }

    event_c _event_c;
    _result = f(_publisher, _event_c);
    if (_result != result::c) {
      return false;
    }

    int16_t _i{-9};
    const std::string &_s{"hi"};
    _result = f<publisher>(_publisher, _i, _s);
    if (_result != result::d) {
      return false;
    }

    return true;
  }

private:
  enum class result : int { unknown = -1, a = 1, b = 2, c = 3, d = 4 };
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };

  struct event_b {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &) {
      return p_out;
    }
  };

  struct event_c {
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &) {
      return p_out;
    }
  };

  struct event_d {
    event_d() = default;
    event_d(int16_t p_i, const std::string &p_s) : m_i(p_i), m_s(p_s) {}

    friend std::ostream &operator<<(std::ostream &p_out, const event_d &) {
      return p_out;
    }

    int16_t m_i;
    const std::string m_s;
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_a &p_event_a) {
    return p_publisher.publish(p_event_a);
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_b &p_event_b) {
    return p_publisher.publish(p_event_b);
  };

  template <async::has_const_lvalue_publish_method<result, event_a>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, const event_c &p_event_c) {
    return p_publisher.publish(p_event_c);
  };

  template <async::has_variadic_params_publish_method<result, event_d>
                t_has_publish_method>
  result f(t_has_publish_method &p_publisher, int16_t p_i,
           const std::string &p_s) {
    return p_publisher.template publish<event_d>(p_i, p_s);
  };

  struct publisher {
    template <traits::async::event t_event> result publish(const t_event &) {
      if constexpr (std::is_same_v<t_event, event_a>) {
        return result::a;
      } else if constexpr (std::is_same_v<t_event, event_b>) {
        return result::b;
      } else if constexpr (std::is_same_v<t_event, event_c>) {
        return result::c;
      } else
        return result::d;
    }

    template <async::event t_event, typename... t_params>
    result publish(t_params &&...) {
      if constexpr (std::is_same_v<t_event, event_d>) {
        return result::d;
      }
      return result::unknown;
    }
  };
};

} // namespace tnct::traits::test
#endif

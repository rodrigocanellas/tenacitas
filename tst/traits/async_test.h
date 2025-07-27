/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_TRAITS_ASYNC_H
#define TENACITAS_TST_TRAITS_ASYNC_H

#include <functional>
#include <string>
#include <thread>
// #include <cstdint>
// #include <tuple>

// #include "tenacitas/src/container/circular_queue.h"
// #include "tenacitas/src/log/cerr.h"
// #include "tenacitas/src/log/no_logger.h"
#include "tenacitas/src/program/options.h"
// #include "tenacitas/src/string/fixed_size_string.h"
// #include "tenacitas/src/traits/async/add_handling_method.h"
// #include "tenacitas/src/traits/async/dispatcher.h"
// #include "tenacitas/src/traits/async/event.h"
// #include "tenacitas/src/traits/async/publish_method.h"
// #include "tenacitas/src/traits/async/result.h"

#include "tenacitas/src/traits/async/handling_name.h"

using namespace tenacitas;

// namespace handling_name_types
// {
// struct abc_1
// {
//   abc_1()              = delete;
//   abc_1(const abc_1 &) = default;
//   abc_1(abc_1 &&)      = default;
//   abc_1 &operator=(const abc_1 &)
//   {
//     return *this;
//   }
//   abc_1 &operator=(abc_1 &&)
//   {
//     return *this;
//   }
//   friend std::ostream &operator<<(std::ostream &p_out, const abc_1 &)
//   {
//     return p_out;
//   }
//   std::size_t operator()() const noexcept
//   {
//     return 42;
//   }
// };

// } // namespace handling_name_types

// namespace std
// {
// template <>
// struct hash<handling_name_types::abc_1>
// {
//   std::size_t operator()(const handling_name_types::abc_1 &) const noexcepta
//   {
//     // Since abc_1 has no state, return a fixed value or hash typeid
//     return 42;
//   }
// };
// } // namespace std

namespace tenacitas::tst::traits
{

struct handling_name_000
{
  static std::string desc()
  {
    return "Verifies that a type is 'handling_name' compatible ";
  }

  bool operator()(const src::program::options &)
  {

    using compatible_handling_name = std::thread::id;

    static_assert(src::traits::async::handling_name<compatible_handling_name>,
                  "'std::thread::id' should be a acceptable 'handling_name'");

    return true;
  }
};

struct handling_name_001
{
  static std::string desc()
  {
    return "Verifies that a type is not 'handling_name' compatible because it "
           "is not default constructible";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::traits::async::handling_name<abc>,
                  "'abc' should not be a acceptable "
                  "'handling_name'");

    return true;
  }

private:
  struct abc
  {
    abc()                                       = delete;
    abc(const abc &)                            = default;
    abc(abc &&)                                 = default;
    abc                 &operator=(const abc &) = default;
    abc                 &operator=(abc &&)      = default;
    friend std::ostream &operator<<(std::ostream &p_out, const abc &)
    {
      return p_out;
    }
    std::size_t operator()() const noexcept
    {
      return 42;
    }
  };
};

struct handling_name_002
{
  static std::string desc()
  {
    return "Verifies that a type is not 'handling_name' compatible because it "
           "is not copy constructible";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::traits::async::handling_name<abc>,
                  "'abc' should not be a acceptable "
                  "'handling_name'");

    return true;
  }

private:
  struct abc
  {
    abc()                                       = default;
    abc(const abc &)                            = delete;
    abc(abc &&)                                 = default;
    abc                 &operator=(const abc &) = default;
    abc                 &operator=(abc &&)      = default;
    friend std::ostream &operator<<(std::ostream &p_out, const abc &)
    {
      return p_out;
    }
    std::size_t operator()() const noexcept
    {
      return 42;
    }
  };
};

struct handling_name_003
{
  static std::string desc()
  {
    return "Verifies that a type is not 'handling_name' compatible because it "
           "is not move constructible";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::traits::async::handling_name<abc>,
                  "'abc' should not be a acceptable "
                  "'handling_name'");

    return true;
  }

private:
  struct abc
  {
    abc()                                       = default;
    abc(const abc &)                            = default;
    abc(abc &&)                                 = delete;
    abc                 &operator=(const abc &) = default;
    abc                 &operator=(abc &&)      = default;
    friend std::ostream &operator<<(std::ostream &p_out, const abc &)
    {
      return p_out;
    }
    std::size_t operator()() const noexcept
    {
      return 42;
    }
  };
};

struct handling_name_004
{
  static std::string desc()
  {
    return "Verifies that a type is not 'handling_name' compatible because it "
           "is not copy assignable";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::traits::async::handling_name<abc>,
                  "'abc' should not be a acceptable "
                  "'handling_name'");

    return true;
  }

private:
  struct abc
  {
    abc()                                       = default;
    abc(const abc &)                            = default;
    abc(abc &&)                                 = default;
    abc                 &operator=(const abc &) = delete;
    abc                 &operator=(abc &&)      = default;
    friend std::ostream &operator<<(std::ostream &p_out, const abc &)
    {
      return p_out;
    }
    std::size_t operator()() const noexcept
    {
      return 42;
    }
  };
};

struct handling_name_005
{
  static std::string desc()
  {
    return "Verifies that a type is not 'handling_name' compatible because it "
           "is not move assignable";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::traits::async::handling_name<abc>,
                  "'abc' should not be a acceptable "
                  "'handling_name'");

    return true;
  }

private:
  struct abc
  {
    abc()                                       = default;
    abc(const abc &)                            = default;
    abc(abc &&)                                 = default;
    abc                 &operator=(const abc &) = default;
    abc                 &operator=(abc &&)      = delete;
    friend std::ostream &operator<<(std::ostream &p_out, const abc &)
    {
      return p_out;
    }
    std::size_t operator()() const noexcept
    {
      return 42;
    }
  };
};

struct handling_name_006
{
  static std::string desc()
  {
    return "Verifies that a type is not 'handling_name' compatible because it "
           "does not have output operator";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::traits::async::handling_name<abc>,
                  "'abc' should not be a acceptable "
                  "'handling_name'");

    return true;
  }

private:
  struct abc
  {
    abc()                              = default;
    abc(const abc &)                   = default;
    abc(abc &&)                        = default;
    abc        &operator=(const abc &) = default;
    abc        &operator=(abc &&)      = default;
    std::size_t operator()() const noexcept
    {
      return 42;
    }
  };
};

struct handling_name_007
{
  static std::string desc()
  {
    return "Verifies that a type is not 'handling_name' compatible because it "
           "does not satidfy 'traits::is_hashable";
  }

  bool operator()(const src::program::options &)
  {
    static_assert(!src::traits::async::handling_name<abc>,
                  "'abc' should not be a acceptable "
                  "'handling_name'");

    return true;
  }

private:
  struct abc
  {
    abc()                                       = default;
    abc(const abc &)                            = default;
    abc(abc &&)                                 = default;
    abc                 &operator=(const abc &) = default;
    abc                 &operator=(abc &&)      = default;
    friend std::ostream &operator<<(std::ostream &p_out, const abc &)
    {
      return p_out;
    }
  };
};

// struct dispatcher_000
// {

//   static std::string desc()
//   {
//     return "Basic traitrs::dispatcher test";
//   }

//   bool operator()(const src::program::options &)
//   {
//     src::log::cerr _logger;

//     my_dispatcher _dispatcher;

//     f(_dispatcher);

//     return true;
//   }

// private:
//   struct event_a
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
//     {
//       return p_out;
//     }
//   };

//   struct event_b
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
//     {
//       return p_out;
//     }
//   };

//   struct event_c
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_c &)
//     {
//       return p_out;
//     }
//   };

//   template <src::traits::async::event... t_events>
//   struct dispatcher
//   {
//     using events = std::tuple<t_events...>;

//     dispatcher()                   = default;
//     dispatcher(const dispatcher &) = delete;
//     dispatcher(dispatcher &&)      = delete;

//     dispatcher &operator=(const dispatcher &) = delete;
//     dispatcher &operator=(dispatcher &&)      = delete;

//     void *operator new(size_t) = delete;
//   };

//   using my_dispatcher = dispatcher<event_a, event_b>;

//   template <src::traits::async::dispatcher<event_a, event_b> t_dispatcher>
//   void f(t_dispatcher &)
//   {
//   }
// };

// struct has_publish_method_000
// {
//   static std::string desc()
//   {
//     return "A valid has_publish_method";
//   }

//   bool operator()(const src::program::options &)
//   {
//     using my_publisher = publisher;
//     //<event_a>;
//     my_publisher _publisher;
//     event_a      _event_a;

//     result _result{f(_publisher, _event_a)};

//     std::cerr << "result = "
//               << static_cast<std::underlying_type_t<result>>(_result) <<
//               '\n';

//     return true;
//   }

// private:
//   enum class result : int
//   {
//     r0 = 1,
//     r1 = -1
//   };
//   struct event_a
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
//     {
//       return p_out;
//     }
//   };

//   struct publisher
//   {
//     using events = std::tuple<event_a>;
//     template <src::traits::async::event t_event>
//     result publish(const t_event &)
//     {
//       return result::r0;
//     }
//   };

//   template <src::traits::async::has_const_lvalue_publish_method<result,
//   event_a>
//                 t_has_publish_method>
//   result f(t_has_publish_method &p_publisher, const event_a &p_event_a)
//   {
//     return p_publisher.publish(p_event_a);
//   };
// };

// struct has_publish_method_001
// {
//   static std::string desc()
//   {
//     return "A invalid has_publish_method";
//   }

//   bool operator()(const src::program::options &)
//   {

//     // The code below does not compile because 'publisher' has no 'publish'
//     // method

//     // using my_publisher = publisher<event_a>;
//     // my_publisher _publisher;
//     // event_a _event_a;

//     // result _result{f(_publisher, _event_a)};

//     // std::cerr << "result = "
//     //           << static_cast<std::underlying_type_t<result>>(_result) <<
//     //           '\n';

//     return true;
//   }

// private:
//   enum class result : int
//   {
//     r0 = 1,
//     r1 = -1
//   };
//   struct event_a
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
//     {
//       return p_out;
//     }
//   };
//   template <src::traits::async::event... t_events>
//   struct publisher
//   {
//     // using events = std::tuple<event_a>;
//     // template <src::traits::async::event t_event> result publish(const
//     t_event
//     // &) {
//     //   return result::r0;
//     // }
//   };

//   template <src::traits::async::has_const_lvalue_publish_method<result,
//   event_a>
//                 t_has_publish_method>
//   result f(t_has_publish_method &p_publisher, const event_a &p_event_a)
//   {
//     return p_publisher.publish(p_event_a);
//   };
// };

// struct has_publish_method_002
// {
//   static std::string desc()
//   {
//     return "Checks publihing with multiples events";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         src::traits::async::has_const_lvalue_publish_method<publisher,
//         result,
//                                                             event_a>,
//         "publisher should match has_const_lvalue_publish for event_a!");

//     publisher _publisher;

//     event_b _event_b;
//     auto    _result{f(_publisher, _event_b)};
//     if (_result != result::b)
//     {
//       return false;
//     }

//     event_a _event_a;
//     _result = f(_publisher, _event_a);
//     if (_result != result::a)
//     {
//       return false;
//     }

//     event_c _event_c;
//     _result = f(_publisher, _event_c);
//     if (_result != result::c)
//     {
//       return false;
//     }

//     int16_t            _i{-9};
//     const std::string &_s{"hi"};
//     _result = f<publisher>(_publisher, _i, _s);
//     if (_result != result::d)
//     {
//       return false;
//     }

//     return true;
//   }

// private:
//   enum class result : int
//   {
//     unknown = -1,
//     a       = 1,
//     b       = 2,
//     c       = 3,
//     d       = 4
//   };
//   struct event_a
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
//     {
//       return p_out;
//     }
//   };

//   struct event_b
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
//     {
//       return p_out;
//     }
//   };

//   struct event_c
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_c &)
//     {
//       return p_out;
//     }
//   };

//   struct event_d
//   {
//     event_d() = default;
//     event_d(int16_t p_i, const std::string &p_s) : m_i(p_i), m_s(p_s)
//     {
//     }

//     friend std::ostream &operator<<(std::ostream &p_out, const event_d &)
//     {
//       return p_out;
//     }

//     int16_t           m_i;
//     const std::string m_s;
//   };

//   template <src::traits::async::has_const_lvalue_publish_method<result,
//   event_a>
//                 t_has_publish_method>
//   result f(t_has_publish_method &p_publisher, const event_a &p_event_a)
//   {
//     return p_publisher.publish(p_event_a);
//   };

//   template <src::traits::async::has_const_lvalue_publish_method<result,
//   event_a>
//                 t_has_publish_method>
//   result f(t_has_publish_method &p_publisher, const event_b &p_event_b)
//   {
//     return p_publisher.publish(p_event_b);
//   };

//   template <src::traits::async::has_const_lvalue_publish_method<result,
//   event_a>
//                 t_has_publish_method>
//   result f(t_has_publish_method &p_publisher, const event_c &p_event_c)
//   {
//     return p_publisher.publish(p_event_c);
//   };

//   template <
//       src::traits::async::has_variadic_params_publish_method<result, event_d>
//           t_has_publish_method>
//   result f(t_has_publish_method &p_publisher, int16_t p_i,
//            const std::string &p_s)
//   {
//     return p_publisher.template publish<event_d>(p_i, p_s);
//   };

//   struct publisher
//   {
//     using events = std::tuple<event_a, event_b, event_c, event_d>;
//     template <src::traits::async::event t_event>
//     result publish(const t_event &)
//     {
//       if constexpr (std::is_same_v<t_event, event_a>)
//       {
//         return result::a;
//       }
//       else if constexpr (std::is_same_v<t_event, event_b>)
//       {
//         return result::b;
//       }
//       else if constexpr (std::is_same_v<t_event, event_c>)
//       {
//         return result::c;
//       }
//       else
//         return result::d;
//     }

//     template <src::traits::async::event t_event, typename... t_params>
//     result publish(t_params &&...)
//     {
//       if constexpr (std::is_same_v<t_event, event_d>)
//       {
//         return result::d;
//       }
//       return result::unknown;
//     }
//   };
// };

// enum class result_master : int
// {
//   OK = 0,
//   unknown = -1,
//   a       = 1,
//   b       = 2,
//   c       = 3,
//   d       = 4
// };

// std::ostream &operator<<(std::ostream &p_out, const result_master &p_result)
// {
//   p_out << static_cast<std::underlying_type_t<result_master>>(p_result);
//   return p_out;
// }

// enum class priority_master : uint16_t
// {
//   l = 0,
//   m = 1,
//   h = 2
// };

// std::ostream &operator<<(std::ostream          &p_out,
//                          const priority_master &p_priority_master)
// {
//   p_out << static_cast<std::underlying_type_t<priority_master>>(
//       p_priority_master);
//   return p_out;
// }

// struct has_add_handling_method_000
// {
//   static std::string desc()
//   {
//     return "Check if src::traits::async::has_add_handling_method captures in
//     "
//            "compile time if a class has the 'add_handling' method correctly "
//            "declared";
//   }

//   bool operator()(const src::program::options &)
//   {
//     dummy _dummy;
//     foo(_dummy);

//     return true;
//   }

// private:
//   using logger = src::log::no_logger;

//   struct handling_a
//   {
//     struct event
//     {
//       friend std::ostream &operator<<(std::ostream &p_out, const event &)
//       {
//         return p_out;
//       }
//     };

//     using priority = priority_master;

//     struct handler
//     {
//       void operator()(event &&)
//       {
//       }
//     };

//     using queue = src::container::circular_queue<logger, event, 10>;

//     using id = src::string::fixed_size_string<5>;

//     const id &get_id() const
//     {
//       return m_id;
//     }
//     handler get_handler() const
//     {
//       return handler{};
//     }
//     queue get_queue()
//     {
//       return queue{m_logger};
//     }
//     std::size_t get_num_handlers() const
//     {
//       return 3;
//     }
//     priority get_priority() const
//     {
//       return priority::h;
//     }

//   private:
//     logger m_logger;
//     id     m_id{"01234"};
//   };

//   template <
//       src::traits::async::has_add_handling_method<result_master, handling_a>
//           t_add_handling_method>
//   void foo(t_add_handling_method &p_add_handling_method)
//   {
//     p_add_handling_method.template add_handling<handling_a>(
//         std::move(handling_a{}));
//   }

//   struct dummy
//   {
//     using events = std::tuple<handling_a::event>;
//     template <src::traits::async::handling_definition t_handling>
//     result_master add_handling(t_handling &&)
//     {
//       return result_master::a;
//     }
//   };
// };

// struct has_add_handling_method_001
// {
//   static std::string desc()
//   {
//     return "Check if src::traits::async::has_add_handling_method captures in
//     "
//            "compile time an error if a class does not have the 'add_handling'
//            " "method correctly declared because the the handling_id does not
//            " "conform to src::string::fixed_size_string";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(!src::traits::string::fixed_size_string<handling_a::id>);
//     static_assert(!src::traits::async::handling_definition<handling_a>);
//     static_assert(
//         !src::traits::async::has_add_handling_method<dummy, result_master,
//                                                      handling_a>);

//     // the following lines cause a compiler error, as expected, because
//     // handling_a::id does not conform to
//     // 'traits::src::string::fixed_size_string'
//     // dummy _dummy;
//     // foo(_dummy);

//     return true;
//   }

// private:
//   using logger = src::log::no_logger;

//   struct handling_a
//   {
//     struct event
//     {
//       friend std::ostream &operator<<(std::ostream &p_out, const event &)
//       {
//         return p_out;
//       }
//     };

//     struct handler
//     {
//       void operator()(event &&)
//       {
//       }
//     };

//     using queue    = src::container::circular_queue<logger, event, 10>;
//     using priority = priority_master;
//     using id       = int *;

//     const id &get_id() const
//     {
//       return m_id;
//     }
//     handler get_handler() const
//     {
//       return handler{};
//     }
//     queue get_queue()
//     {
//       return queue{m_logger};
//     }
//     std::size_t get_num_handlers() const
//     {
//       return 3;
//     }
//     priority get_priority() const
//     {
//       return priority::h;
//     }

//   private:
//     logger m_logger;
//     id     m_id{nullptr};
//   };

//   // template <src::traits::async::has_add_handling_method<result_master,
//   // handling_a>
//   //               t_add_handling_method>
//   // void foo(t_add_handling_method p_add_handling_method) {
//   //   static_assert(src::traits::async::handling<handling_a>,
//   //                 "handling_a does not satisfy
//   //                 src::traits::async::handling");
//   //   p_add_handling_method.template add_handling<handling_a>(
//   //       std::move(handling_a{}));
//   // }

//   struct dummy
//   {
//     template <typename t_event>
//     result_master add_handling(handling_a &&)
//     {
//       return result_master::a;
//     }
//   };
// };

// struct has_add_handling_method_002
// {
//   static std::string desc()
//   {
//     return "Check if src::traits::async::has_add_handling_method captures in
//     "
//            "compile time an error if a class does not have the 'add_handling'
//            " "method handler conform to src::async::handler";
//   }

//   bool operator()(const src::program::options &)
//   {

//     static_assert(!src::traits::async::handling_method<handling_a::handler,
//                                                        handling_a::event>);
//     static_assert(!src::traits::async::handling_definition<handling_a>);

//     // the following lines cause a compiler error, as expected, because
//     // 'handling_a::handler' declared below is not compatible with
//     // 'src::traits::async::handler'
//     // dummy _dummy;
//     // foo(_dummy);

//     return true;
//   }

// private:
//   using logger = src::log::no_logger;

//   struct handling_a
//   {
//     struct event
//     {
//       friend std::ostream &operator<<(std::ostream &p_out, const event &)
//       {
//         return p_out;
//       }
//     };

//     struct handler
//     {
//       void operator()(event)
//       {
//       }
//     };

//     using queue    = src::container::circular_queue<logger, event, 10>;
//     using priority = priority_master;
//     using id       = src::string::fixed_size_string<5>;

//     const id &get_id() const
//     {
//       return m_id;
//     }
//     handler get_handler() const
//     {
//       return handler{};
//     }
//     queue get_queue()
//     {
//       return queue{m_logger};
//     }
//     std::size_t get_num_handlers() const
//     {
//       return 3;
//     }
//     priority get_priority() const
//     {
//       return priority::h;
//     }

//   private:
//     logger m_logger;
//     id     m_id{"01234"};
//   };

//   template <
//       src::traits::async::has_add_handling_method<result_master, handling_a>
//           t_add_handling_method>
//   void foo(t_add_handling_method p_add_handling_method)
//   {
//     p_add_handling_method.template add_handling<handling_a>(
//         std::move(handling_a{}));
//   }

//   struct dummy
//   {
//     template <typename t_event>
//     result_master add_handling(handling_a &&)
//     {
//       return result_master::a;
//     }
//   };
// };

// struct has_add_handling_method_003
// {
//   static std::string desc()
//   {
//     return "Check if src::traits::async::has_add_handling_method captures in
//     "
//            "compile time an error if a class does not have the 'add_handling'
//            " "method correctly declared because the event informed does not "
//            "conform to 'src::traits::async::event";
//   }

//   bool operator()(const src::program::options &)
//   {

//     static_assert(!src::traits::async::event<handling_a::event>);
//     static_assert(!src::traits::async::handling_definition<handling_a>);

//     // the following lines cause a compiler error, as expected, because
//     // 'handling_a::event' declared below is not compatible with
//     // 'src::traits::async::event'
//     // dummy _dummy;
//     // foo(_dummy);

//     return true;
//   }

// private:
//   using logger = src::log::no_logger;

//   struct handling_a
//   {
//     struct event
//     {
//     };

//     struct handler
//     {
//       void operator()(event &&)
//       {
//       }
//     };

//     // the following lines cause a compiler error, as expected, because
//     // 'handling_a::event' declared below is not compatible with
//     // 'src::traits::async::event'

//     // using queue = src::container::circular_queue<logger, event, 10>;
//     using priority = priority_master;
//     using id       = src::string::fixed_size_string<5>;

//     const id &get_id() const
//     {
//       return m_id;
//     }
//     handler get_handler() const
//     {
//       return handler{};
//     }
//     //     queue get_queue() { return queue{m_logger}; }
//     std::size_t get_num_handlers() const
//     {
//       return 3;
//     }
//     priority get_priority() const
//     {
//       return priority::h;
//     }

//   private:
//     // logger m_logger;
//     id m_id{"01234"};
//   };

//   template <
//       src::traits::async::has_add_handling_method<result_master, handling_a>
//           t_add_handling_method>
//   void foo(t_add_handling_method p_add_handling_method)
//   {
//     p_add_handling_method.template add_handling<handling_a>(
//         std::move(handling_a{}));
//   }

//   struct dummy
//   {
//     template <typename t_event>
//     result_master add_handling(handling_a &&)
//     {
//       return result_master::a;
//     }
//   };
// };

// struct has_add_handling_method_004
// {
//   static std::string desc()
//   {
//     return "Check if src::traits::async::has_add_handling_method captures in
//     "
//            "compile time an error if a class does not have the 'add_handling'
//            " "method correctly declared because the queue informed does not "
//            "conform to 'src::traits::container::queue";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::traits::container::queue<handling_a::queue,
//         handling_a::event>);
//     static_assert(!src::traits::async::handling_definition<handling_a>);
//     // dummy _dummy;
//     // foo(_dummy);

//     return true;
//   }

// private:
//   using logger = src::log::no_logger;

//   struct handling_a
//   {
//     struct event
//     {
//       friend std::ostream &operator<<(std::ostream &p_out, const event &)
//       {
//         return p_out;
//       }
//     };

//     struct handler
//     {
//       void operator()(event &&)
//       {
//       }
//     };

//     using queue = int;

//     enum class priority : char
//     {
//       l = 'l',
//       m = 'm',
//       h = 'h'
//     };
//     using id = src::string::fixed_size_string<5>;

//     const id &get_id() const
//     {
//       return m_id;
//     }
//     handler get_handler() const
//     {
//       return handler{};
//     }
//     queue get_queue()
//     {
//       return queue{};
//     }
//     std::size_t get_num_handlers() const
//     {
//       return 3;
//     }
//     priority get_priority() const
//     {
//       return priority::h;
//     }

//   private:
//     // logger m_logger;
//     id m_id{"01234"};
//   };

//   template <
//       src::traits::async::has_add_handling_method<result_master, handling_a>
//           t_add_handling_method>
//   void foo(t_add_handling_method p_add_handling_method)
//   {
//     p_add_handling_method.template add_handling<handling_a>(
//         std::move(handling_a{}));
//   }

//   struct dummy
//   {
//     template <typename t_event>
//     result_master add_handling(handling_a &&)
//     {
//       return result_master::a;
//     }
//   };
// };

// struct has_add_handling_method_006
// {
//   static std::string desc()
//   {
//     return "Check if src::traits::async::has_add_handling_method captures in
//     "
//            "compile time an error if a class does not have the 'add_handling'
//            " "method correctly declared because the result informed " "does
//            not conform to 'src::traits::async::result";
//   }

//   bool operator()(const src::program::options &)
//   {

//     static_assert(!src::traits::async::result<result>);
//     static_assert(!src::traits::async::has_add_handling_method<dummy, result,
//                                                                handling_a>);

//     // the following lines cause a compiler error, as expected, because
//     // 'result' declared below is not compatible with
//     // 'src::traits::async::result'

//     // dummy _dummy;
//     // foo(_dummy);

//     return true;
//   }

// private:
//   using logger = src::log::no_logger;

//   using result = int;

//   struct handling_a
//   {
//     struct event
//     {
//       friend std::ostream &operator<<(std::ostream &p_out, const event &)
//       {
//         return p_out;
//       }
//     };

//     struct handler
//     {
//       void operator()(event &&)
//       {
//       }
//     };

//     using queue = src::container::circular_queue<logger, event, 10>;

//     using priority = priority_master;
//     using id       = src::string::fixed_size_string<5>;

//     const id &get_id() const
//     {
//       return m_id;
//     }
//     handler get_handler() const
//     {
//       return handler{};
//     }
//     queue get_queue()
//     {
//       return queue{m_logger};
//     }
//     std::size_t get_num_handlers() const
//     {
//       return 3;
//     }
//     priority get_priority() const
//     {
//       return priority::h;
//     }

//   private:
//     logger m_logger;
//     id     m_id{"01234"};
//   };

//   template <src::traits::async::has_add_handling_method<result, handling_a>
//                 t_add_handling_method>
//   void foo(t_add_handling_method p_add_handling_method)
//   {
//     p_add_handling_method.template add_handling<handling_a>(
//         std::move(handling_a{}));
//   }

//   struct dummy
//   {
//     template <typename t_event>
//     result_master add_handling(handling_a &&)
//     {
//       return result_master::a;
//     }
//   };
// };

// struct handling_priority_000
// {
//   static std::string desc()
//   {
//     return "a 'handling_priority' which is not conformant to "
//            "src::traits::async::handling_priority because it does not "
//            "conformant to traits::has_output_operator";
//   }

//   bool operator()(const src::program::options &)
//   {
//     // enum class priority_000 : int { a = -1, b = 0, c = 1 };
//     // static_assert(src::traits::async::handling_priority<priority_000>,
//     //               "priority_000 is not
//     src::traits::async::handling_priority
//     //               " "conformant");

//     /*
// async_test.h:660:19: Static assertion failed: priority_000 is not
// src::traits::async::handling_priority conformant

// async_test.h:660:19: because 'priority_000' does not satisfy
// 'handling_priority'

// handling_priority.h:15:45: because 'priority_000' does not satisfy
// 'has_output_operator'

// has_output_operator.h:18:8: because 'os << value' would be invalid: invalid
// operands to binary expression ('std::basic_ostream<char, char_traits<char>>'
// and 'const priority_000')
// */
//     return true;
//   }
// };

// struct handling_priority_001
// {
//   static std::string desc()
//   {
//     return "a 'handling_priority' which is not conformant to "
//            "src::traits::async::handling_priority because it does not "
//            "conformant to traits::enum_like";
//   }

//   bool operator()(const src::program::options &)
//   {
//     // using priority_001 = uint16_t;
//     // static_assert(src::traits::async::handling_priority<priority_001>,
//     //               "priority_000 is not
//     src::traits::async::handling_priority
//     //               " "conformant");
//     /*
// async_test.h:692:19: Static assertion failed: priority_000 is not
// src::traits::async::handling_priority conformant

// async_test.h:692:19: because 'priority_001' (aka 'unsigned short') does not
// satisfy 'handling_priority'

// handling_priority.h:15:29: because 'unsigned short' does not satisfy
// 'enum_like'

// enum.h:14:21: because 'std::is_enum_v<unsigned short>' evaluated to false
// */

//     return true;
//   }
// };

// enum class priority_002 : int
// {
//   a = -1,
//   b = 0,
//   c = 1
// };
// std::ostream &operator<<(std::ostream &p_out, const priority_002 &)
// {
//   return p_out;
// }

// struct handling_priority_002
// {
//   static std::string desc()
//   {
//     return "a 'handling_priority' which is conformant to "
//            "src::traits::async::handling_priority";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(src::traits::async::handling_priority<priority_002>,
//                   "priority_000 is not conformant to "
//                   "src::traits::async::handling_priority ");

//     return true;
//   }
// };

// struct handler_000
// {
//   static std::string desc()
//   {
//     return "A lambda function that is conformant to "
//            "src::traits::async::handler";
//   }
//   bool operator()(const src::program::options &)
//   {
//     auto _handler = [](event &&) mutable {};

//     static_assert(
//         src::traits::async::handling_method<decltype(_handler), event>,
//         "'auto _handle = [](event &&) {}' is conformant to "
//         "src::traits::async::handler");

//     return true;
//   }

// private:
//   struct event
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };
// };

// struct handler_001
// {
//   static std::string desc()
//   {
//     return "A lambda function that is not src::traits::async::handler "
//            "conformance";
//   }
//   bool operator()(const src::program::options &)
//   {
//     auto _handle = []() {};

//     static_assert(
//         !src::traits::async::handling_method<decltype(_handle), event>,
//         "'auto _handle = []() {}' is not conformance to "
//         "src::traits::async::handler");

//     return true;
//   }

// private:
//   struct event
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };
// };

// struct handler_002
// {
//   static std::string desc()
//   {
//     return "A 'handle' class that is src::traits::async::handler
//     conformance";
//   }
//   bool operator()(const src::program::options &)
//   {

//     static_assert(
//         src::traits::async::handling_method<handle, event>,
//         "'handle class is conformance to src::traits::async::handler");

//     return true;
//   }

// private:
//   struct event
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };

//   struct handle
//   {
//     void operator()(event &&)
//     {
//     }
//   };
// };

// struct handler_003
// {
//   static std::string desc()
//   {
//     return "A 'handle' class that is not conformant to "
//            "src::traits::async::handler ";
//   }
//   bool operator()(const src::program::options &)
//   {

//     static_assert(
//         !src::traits::async::handling_method<handle, event>,
//         "'handle class is not conformant to src::traits::async::handler");

//     return true;
//   }

// private:
//   struct event
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };

//   struct handle
//   {
//     void operator()()
//     {
//     }
//   };
// };

// struct handler_004
// {
//   static std::string desc()
//   {
//     return "Another 'handle' class that is not conformant to "
//            "src::traits::async::handler";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::traits::async::handling_method<handle, event>,
//         "'handle' class is not conformant to src::traits::async::handler");
//     return true;
//   }

// private:
//   struct event
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };

//   struct handle
//   {
//     void operator()(event)
//     {
//     }
//   };
// };

// struct handler_005
// {
//   static std::string desc()
//   {
//     return "Yet another 'handle' class that is not conformant to "
//            "src::traits::async::handler";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::traits::async::handling_method<handle, event>,
//         "'handle' class is not conformant to src::traits::async::handler");
//     return true;
//   }

// private:
//   struct event
//   {
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };

//   struct handle
//   {
//     void operator()(const event &)
//     {
//     }
//   };
// };

// struct event_000
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::traits::async::event'
//     "
//            "because it does not have output operator";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::traits::async::event<event>,
//         "'event' should not be  conformant to 'src::traits::async::event' "
//         "because it does not output operator");

//     return true;
//   }

// private:
//   struct event
//   {
//   };
// };

// struct event_001
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::traits::async::event'
//     "
//            "because its default constructor is deleted";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::traits::async::event<event>,
//         "'event' should not be  conformant to 'src::traits::async::event' "
//         "because its default constructor is deleted");

//     return true;
//   }

// private:
//   struct event
//   {
//     event() = delete;
//   };
// };

// struct event_002
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::traits::async::event'
//     "
//            "because its copy constructor is deleted";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::traits::async::event<event>,
//         "'event' should not be  conformant to 'src::traits::async::event' "
//         "because its copy constructor is deleted");

//     return true;
//   }

// private:
//   struct event
//   {
//     event(const event &) = delete;
//   };
// };

// struct event_003
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'src::traits::async::event'
//     "
//            "because its move constructor is deleted";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         !src::traits::async::event<event>,
//         "'event' should not be  conformant to 'src::traits::async::event' "
//         "because its move constructor is deleted");

//     return true;
//   }

// private:
//   struct event
//   {
//     event(event &&) = delete;
//   };
// };

// struct event_004
// {
//   static std::string desc()
//   {
//     return "A 'event' class is conformant to 'src::traits::async::event'";
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(
//         src::traits::async::event<event>,
//         "'event' should be  conformant to 'src::traits::async::event' ");

//     return true;
//   }

// private:
//   struct event
//   {
//     event()              = default;
//     event(event &&)      = default;
//     event(const event &) = default;
//     friend std::ostream &operator<<(std::ostream &p_out, const event &)
//     {
//       return p_out;
//     }
//   };
// };

// enum class result_like : std::int8_t
// {
//   OK      = 0,
//   ERROR_1 = -1,
//   ERROR_2 = -2
// };

// std::ostream &operator<<(std::ostream &p_out, result_like)
// {

//   return p_out;
// }

// struct result_000
// {

//   static std::string desc()
//   {
//     return {"A 'enum' compatible with 'result'"};
//   }

//   bool operator()(const src::program::options &)
//   {
//     static_assert(src::traits::async::result<result_like>, "This compiles");
//     return true;
//   }
// };

} // namespace tenacitas::tst::traits
#endif

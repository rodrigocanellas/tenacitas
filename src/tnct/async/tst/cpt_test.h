/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_ASYNC_TST_CPT_TEST_H
#define TNCT_ASYNC_TST_CPT_TEST_H

#include <string>

#include "tnct/async/bus/dispatcher.h"
#include "tnct/async/cpt/has_add_handling_method.h"
#include "tnct/async/cpt/has_publish_method.h"
#include "tnct/async/cpt/is_dispatcher.h"
#include "tnct/async/cpt/is_event.h"
#include "tnct/async/cpt/is_handler.h"
#include "tnct/async/dat/handling_priority.h"
#include "tnct/async/dat/result.h"
#include "tnct/container/dat/circular_queue.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using namespace tnct;

namespace tnct::async::tst
{

struct cpt_event_000
{
  static std::string desc()
  {
    return "Verifies that a class that is not default contructible is not "
           "compatible with 'event";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_event<event_a>,
        "'event_a' is not compatible with async::cpt::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a() = delete;
  };
};

struct cpt_event_001
{
  static std::string desc()
  {
    return "Verifies that a class that is not copy contructible is not "
           "compatible with 'event";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_event<event_a>,
        "'event_a' is not compatible with async::cpt::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a()                = default;
    event_a(const event_a &) = delete;
  };
};

struct cpt_event_002
{
  static std::string desc()
  {
    return "Verifies that a class that is not move contructible is not "
           "compatible with 'event";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_event<event_a>,
        "'event_a' is not compatible with async::cpt::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a()                = default;
    event_a(const event_a &) = default;
    event_a(event_a &&)      = delete;
  };
};

struct cpt_event_003
{
  static std::string desc()
  {
    return "Verifies that a class that does not have the output operator is "
           "not compatible with 'event";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_event<event_a>,
        "'event_a' is not compatible with async::cpt::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    event_a()                = default;
    event_a(const event_a &) = default;
    event_a(event_a &&)      = default;
  };
};

enum class event_004_event_a : char
{
  start  = 's',
  finish = 'f'
};

std::ostream &operator<<(std::ostream &p_out, event_004_event_a)
{
  return p_out;
}

struct cpt_event_004
{
  static std::string desc()
  {
    return "Verifies that an 'enum' not compatible with 'event";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_event<event_004_event_a>,
        "'event_a' is not compatible with async::cpt::event<event_a>");

    return true;
  }
};

struct cpt_event_005
{
  static std::string desc()
  {
    return "Verifies that a class is default,copy and move constructible and "
           "that implements output operator is compatible with 'event";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(async::cpt::is_event<event_a>,
                  "'event_a' is compatible with async::cpt::event<event_a>");

    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, event_a)
    {
      return p_out;
    }
  };
};

struct add_handler_tests_event
{
  friend std::ostream &operator<<(std::ostream &p_out, add_handler_tests_event)
  {
    return p_out;
  }
};

struct cpt_add_handler_000
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'async::cpt::handler' because it does not "
           "implement call operator that receives a 'event' rvalue";
  }
  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_handler<cpt_add_handler_000, add_handler_tests_event>,
        "'handler_000' is not compatible with "
        "async::cpt::handler ");

    return true;
  }
};

struct cpt_add_handler_001
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'async::cpt::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it returns bool";
  }
  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_handler<cpt_add_handler_001, add_handler_tests_event>,
        "'handler_000' is not compatible with "
        "async::cpt::handler ");

    return true;
  }

  bool operator()(add_handler_tests_event &&)
  {
    return false;
  }
};

struct cpt_add_handler_002
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'async::cpt::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it also "
           "implements a call operator that receives a constant 'event' lvalue "
           "reference";
  }
  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_handler<cpt_add_handler_002, add_handler_tests_event>,
        "'handler_000' is not compatible with "
        "async::cpt::handler ");

    return true;
  }

  void operator()(add_handler_tests_event &&)
  {
  }

  void operator()(const add_handler_tests_event &)
  {
  }
};

struct cpt_add_handler_003
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'async::cpt::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it also "
           "implements a call operator that receives a constant 'event' "
           "lvalue ";
  }
  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_handler<cpt_add_handler_003, add_handler_tests_event>,
        "'handler_000' is not compatible with "
        "async::cpt::handler ");

    return true;
  }

  void operator()(add_handler_tests_event)
  {
  }
};

struct cpt_add_handler_004
{
  static std::string desc()
  {
    return "A struct not compatible with "
           "'async::cpt::handler' because it implements a "
           "call operator that receives a 'event' rvalue, but it also "
           "implements a call operator that receives a 'event' lvalue "
           "reference ";
  }
  bool operator()(const program::bus::options &)
  {
    static_assert(
        !async::cpt::is_handler<cpt_add_handler_004, add_handler_tests_event>,
        "'handler_000' is not compatible with "
        "async::cpt::handler ");

    return true;
  }

  void operator()(add_handler_tests_event &)
  {
  }
};

struct cpt_add_handler_005
{
  static std::string desc()
  {
    return "A struct compatible with 'async::cpt::handler' ";
  }
  bool operator()(const program::bus::options &)
  {
    static_assert(
        async::cpt::is_handler<cpt_add_handler_005, add_handler_tests_event>,
        "'handler_000' is not compatible with "
        "async::cpt::handler ");

    return true;
  }

  void operator()(add_handler_tests_event &&)
  {
  }

  void handle(add_handler_tests_event &&)
  {
  }
};

struct cpt_dispatcher_000
{

  static std::string desc()
  {
    return "Basic traits dispatcher test";
  }

  bool operator()(const program::bus::options &)
  {
    log::cerr _logger;

    using my_dispatcher = dispatcher<event_a, event_b, event_c>;

    static_assert(async::cpt::is_dispatcher<my_dispatcher, event_a, event_c>,
                  "'my_dispatcher should be compatible with async::cpt");

    my_dispatcher _dispatcher;

    f(_dispatcher);

    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct event_b
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
    {
      return p_out;
    }
  };

  struct event_c
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &)
    {
      return p_out;
    }
  };

  template <async::cpt::is_event... t_events>
  struct dispatcher
  {
    using events = std::tuple<t_events...>;

    dispatcher()                   = default;
    dispatcher(const dispatcher &) = delete;
    dispatcher(dispatcher &&)      = delete;

    dispatcher &operator=(const dispatcher &) = delete;
    dispatcher &operator=(dispatcher &&)      = delete;

    void *operator new(size_t) = delete;
  };

  using my_dispatcher = dispatcher<event_a, event_b>;

  template <async::cpt::is_dispatcher<event_a, event_b> t_dispatcher>
  void f(t_dispatcher &)
  {
  }
};

struct cpt_has_add_handling_method_000
{
  static std::string desc()
  {
    return "Checks for async::cpt::has_add_handling_method";
  }

  bool operator()(const program::bus::options &)
  {

    logger _logger;

    using dispatcher = async::bus::dispatcher<logger, event_a>;

    dispatcher _dispatcher{_logger};

    abc<dispatcher> _abc(_logger, _dispatcher);

    return true;
  }

private:
  using logger = log::cerr;

  struct event_a
  {
    friend std ::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  template <typename t_dispatcher>
  struct abc
  {

    using queue_a   = container::dat::circular_queue<logger, event_a, 100>;
    using handler_a = std::function<void(event_a &&)>;

    static_assert(async::cpt::has_add_handling_method<t_dispatcher, event_a,
                                                      queue_a, handler_a>,
                  "t_dispatcher must have add_handling<event_a>()");

    abc(logger &p_logger, t_dispatcher &p_dispatcher)
        : m_dispatcher(p_dispatcher)
    {
      create_handling_1(p_logger);
      create_handling_2(p_logger);
    }

  private:
    void handler_1(event_a &&)
    {
    }

    void handler_2(event_a &&)
    {
    }

    void create_handling_1(logger &p_logger)
    {

      async::dat::result _result{m_dispatcher.template add_handling<event_a>(
          "handling 1", queue_a{p_logger},
          [this](event_a &&evt) { handler_1(std::move(evt)); },
          async::dat::handling_priority::medium, 2)};

      if (_result != async::dat::result::OK)
      {
        TNCT_LOG_ERR(p_logger, format::bus::fmt(_result));
      }
      else
      {
        TNCT_LOG_INF(p_logger, "add handling 1 OK");
      }
    }

    void create_handling_2(logger &p_logger)
    {

      async::dat::result _result{m_dispatcher.template add_handling<event_a>(
          "handling 2", queue_a{p_logger},
          [this](event_a &&evt) { handler_2(std::move(evt)); },
          async::dat::handling_priority::medium, 4)};

      if (_result != async::dat::result::OK)
      {
        TNCT_LOG_ERR(p_logger, format::bus::fmt(_result));
      }
      else
      {
        TNCT_LOG_INF(p_logger, "add handling 2 OK");
      }
    }

  private:
    t_dispatcher &m_dispatcher;
  };
};

struct cpt_has_add_handling_method_001
{
  static std::string desc()
  {
    return "Checks for async::cpt::has_add_handling_method";
  }

  bool operator()(const program::bus::options &)
  {

    logger _logger;

    using dispatcher = invalid_dispatcher;

    dispatcher _dispatcher;

    abc<dispatcher> _abc(_dispatcher);

    return true;
  }

private:
  using logger = log::cerr;

  struct invalid_dispatcher
  {
  };

  struct event_a
  {
    friend std ::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  template <typename t_dispatcher>
  struct abc
  {

    using queue_a   = container::dat::circular_queue<logger, event_a, 100>;
    using handler_a = std::function<void(event_a &&)>;

    static_assert(!async::cpt::has_add_handling_method<t_dispatcher, event_a,
                                                       handler_a, queue_a>,
                  "t_dispatcher must have add_handling<event_a>()");

    abc(t_dispatcher &p_dispatcher) : m_dispatcher(p_dispatcher)
    {
    }

    void handler_1(event_a &&)
    {
    }

    void handler_2(event_a &&)
    {
    }

  private:
    t_dispatcher &m_dispatcher;
  };
};

struct cpt_has_publish_method_000
{
  static std::string desc()
  {
    return "A valid has_publish_method";
  }

  bool operator()(const program::bus::options &)
  {

    static_assert(async::cpt::has_publish_method<publisher, event_a>,
                  "'publisher' should be compatible with "
                  "'async::cpt::has_publish_method'");
    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct publisher
  {

    template <async::cpt::is_event t_event>
    async::dat::result publish(const t_event &)
    {
      return async::dat::result::OK;
    }
  };
};

struct cpt_has_publish_method_001
{
  static std::string desc()
  {
    return "A invalid has_publish_method";
  }

  bool operator()(const program::bus::options &)
  {

    static_assert(!async::cpt::has_publish_method<publisher, event_b>,
                  "'publisher' should not be compatible with "
                  "'async::cpt::has_publish_method'");
    return true;
  }

private:
  struct event_b
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
    {
      return p_out;
    }
  };

  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct publisher
  {
    template <async::cpt::is_event t_event>
    async::dat::result publish(t_event &&)
    {
      return async::dat::result::OK;
    }
  };
};

struct cpt_has_publish_method_002
{
  static std::string desc()
  {
    return "A valid has_publish_method";
  }

  bool operator()(const program::bus::options &)
  {

    static_assert(async::cpt::has_publish_method<publisher, event_a, event_b>,
                  "'publisher' should be compatible with "
                  "'async::cpt::has_publish_method'");
    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct event_b
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
    {
      return p_out;
    }
  };

  struct publisher
  {

    template <async::cpt::is_event t_event, typename... t_params>
    async::dat::result publish(t_params &&...)
    {
      return async::dat::result::OK;
    }
  };
};

struct cpt_handler_001
{
  static std::string desc()
  {
    return "A lambda function that is not "
           "async::cpt::is_handler conformance";
  }
  bool operator()(const program::bus::options &)
  {
    auto _handle = []() {};

    static_assert(!async::cpt::is_handler<decltype(_handle), event>,
                  "'auto _handle = []() {}' is not conformance to "
                  "async::cpt::is_handler");

    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };
};

struct cpt_handler_002
{
  static std::string desc()
  {
    return "A 'handle' class that is async::cpt::is_handler "
           "conformance";
  }
  bool operator()(const program::bus::options &)
  {

    static_assert(async::cpt::is_handler<handle, event>,
                  "handle class is conformance to "
                  "async::cpt::is_handler");

    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()(event &&)
    {
    }
  };
};

struct cpt_handler_003
{
  static std::string desc()
  {
    return "A 'handle' class that is not conformant to "
           "async::cpt::is_handler ";
  }
  bool operator()(const program::bus::options &)
  {

    static_assert(!async::cpt::is_handler<handle, event>,
                  "'handle class is not conformant to "
                  "async::cpt::is_handler");

    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()()
    {
    }
  };
};

struct cpt_handler_004
{
  static std::string desc()
  {
    return "Another 'handle' class that is not conformant to "
           "async::cpt::is_handler";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(!async::cpt::is_handler<handle, event>,
                  "'handle' class is not conformant to "
                  "async::cpt::is_handler");
    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()(event)
    {
    }
  };
};

struct cpt_handler_005
{
  static std::string desc()
  {
    return "Yet another 'handle' class that is not conformant to "
           "async::cpt::is_handler";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(!async::cpt::is_handler<handle, event>,
                  "'handle' class is not conformant to "
                  "async::cpt::is_handler");
    return true;
  }

private:
  struct event
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event &)
    {
      return p_out;
    }
  };

  struct handle
  {
    void operator()(const event &)
    {
    }
  };
};

} // namespace tnct::async::tst
#endif

// struct event_000
// {
//   static std::string desc()
//   {
//     return "A 'event' class is not conformant to 'async::cpt::event'
//     "
//            "because it does not have output operator";
//   }

//   bool operator()(const program::bus::options &)
//   {
//     static_assert(
//         !async::cpt::event<event>,
//         "'event' should not be  conformant to 'async::cpt::event' "
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
//     return "A 'event' class is not conformant to 'async::cpt::event'
//     "
//            "because its default constructor is deleted";
//   }

//   bool operator()(const program::bus::options &)
//   {
//     static_assert(
//         !async::cpt::event<event>,
//         "'event' should not be  conformant to 'async::cpt::event' "
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
//     return "A 'event' class is not conformant to 'async::cpt::event'
//     "
//            "because its copy constructor is deleted";
//   }

//   bool operator()(const program::bus::options &)
//   {
//     static_assert(
//         !async::cpt::event<event>,
//         "'event' should not be  conformant to 'async::cpt::event' "
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
//     return "A 'event' class is not conformant to 'async::cpt::event'
//     "
//            "because its move constructor is deleted";
//   }

//   bool operator()(const program::bus::options &)
//   {
//     static_assert(
//         !async::cpt::event<event>,
//         "'event' should not be  conformant to 'async::cpt::event' "
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
//     return "A 'event' class is conformant to 'async::cpt::event'";
//   }

//   bool operator()(const program::bus::options &)
//   {
//     static_assert(
//         async::cpt::event<event>,
//         "'event' should be  conformant to 'async::cpt::event' ");

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
//     return {"A 'enum' compatible with 'dat::result'"};
//   }

//   bool operator()(const program::bus::options &)
//   {
//     static_assert(async::cpt::dat::result<result_like>, "This compiles");
//     return true;
//   }
// };

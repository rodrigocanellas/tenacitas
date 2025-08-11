/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <optional>
#include <string>

#include "tnct/container/circular_queue.h"
#include "tnct/container/traits/queue.h"
#include "tnct/log/cerr.h"
#include "tnct/program/options.h"

using namespace tnct;

using logger = log::cerr;

namespace tnct::container::tst
{

struct traits_test_000
{
  static std::string desc()
  {
    return "Checking if 'container::circular_queue' complies to "
           "'container::traits::queue'";
  }

  bool operator()(const program::options &)
  {
    using my_queue = container::circular_queue<logger, std::uint16_t, 100>;

    static_assert(
        container::traits::queue<my_queue, std::uint16_t>,
        "'my_queue' should be compliant to 'container::traits::queue'");
    return true;
  }
};

struct traits_test_001
{
  static std::string desc()
  {
    return "Checking an invalid 'container::traits::queue'";
  }

  bool operator()(const program::options &)
  {
    static_assert(!container::traits::queue<invalid_queue<char>, char>,
                  "'invalid_queue' should not ne compatible with "
                  "'container::traits::queue'");
    return true;
  }

private:
  template <typename t_data>
  struct invalid_queue
  {
    invalid_queue() = default;

    void add(t_data &&)
    {
    }

    void add(const t_data &)
    {
    }

    std::optional<t_data> get()
    {
      return {};
    }
  };
};

struct traits_test_002
{
  static std::string desc()
  {
    return "Checking a valid container::traits::queue";
  }

  bool operator()(const program::options &)
  {
    static_assert(container::traits::queue<valid_queue<char>, char>,
                  "'valid_queue' should not ne compatible with "
                  "'container::traits::queue'");

    return true;
  }

private:
  template <typename t_data>
  struct valid_queue
  {
    using data = t_data;
    void push(t_data &&p_data)
    {
      m_data = std::move(p_data);
    }

    std::optional<t_data> pop()
    {
      return {m_data};
    }

    void push(const t_data &p_data)
    {
      m_data = p_data;
    }

    bool full() const
    {
      return false;
    }

    bool empty() const
    {
      return false;
    }

    std::size_t capacity() const
    {
      return 0;
    }

    std::size_t occupied() const
    {
      return 0;
    }

    void clear()
    {
    }

  private:
    t_data m_data;
  };
};
} // namespace tnct::container::tst

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TST_CPT_TEST_H
#define TNCT_CONTAINER_TST_CPT_TEST_H

#include <cstdint>
#include <optional>
#include <string>

#include "tnct/container/dat/circular_queue.h"
#include "tnct/container/cpt/queue.h"
#include "tnct/log/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/options.h"

using namespace tnct;

using logger = log::cerr;

namespace tnct::container::tst
{

struct cpt_test_000
{
  static std::string desc()
  {
    return "Checking if 'container::dat::circular_queue' complies to "
           "'container::cpt::queue'";
  }

  bool operator()(const program::options &)
  {
    using my_queue = container::dat::circular_queue<logger, std::uint16_t, 100>;

    static_assert(container::cpt::queue<my_queue, std::uint16_t>,
                  "'my_queue' should be compliant to 'container::cpt::queue'");
    return true;
  }
};

struct cpt_test_001
{
  static std::string desc()
  {
    return "Checking an invalid 'container::cpt::queue'";
  }

  bool operator()(const program::options &)
  {
    static_assert(!container::cpt::queue<invalid_queue<char>, char>,
                  "'invalid_queue' should not ne compatible with "
                  "'container::cpt::queue'");
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

struct cpt_test_002
{
  static std::string desc()
  {
    return "Checking a valid container::cpt::queue";
  }

  bool operator()(const program::options &)
  {
    static_assert(container::cpt::queue<valid_queue<char>, char>,
                  "'valid_queue' should not ne compatible with "
                  "'container::cpt::queue'");

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

#endif

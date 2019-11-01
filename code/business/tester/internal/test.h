#ifndef TENACITAS_BUSINESS_TESTER_TEST_H
#define TENACITAS_BUSINESS_TESTER_TEST_H

#include <functional>
#include <string>

#include <business/logger/log.h>

namespace tenacitas {
namespace business {
namespace tester {

using namespace tenacitas::business;

struct test
{
  template<typename t_function>
  bool operator()(const std::string& p_name, t_function&& p_function)
  {

    bool result = false;
    try {
      logger::log::test("############ ", __LINE__, p_name);
      result = p_function();
    } catch (std::exception& _ex) {
      logger::log::fatal("ERROR ", p_name, ": '", _ex.what(), "'");
      result = false;
    }
    logger::log::test(p_name, __LINE__, "############");
    return result;
  }
};

} // namespace tester
} // namespace business
} // namespace tenacitas

#endif // TEST_H

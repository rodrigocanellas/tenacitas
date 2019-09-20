#ifndef TENACITAS_TESTER_BUS_TEST_H
#define TENACITAS_TESTER_BUS_TEST_H

#include <functional>
#include <string>

#include <logger/business/log.h>

namespace tenacitas {
namespace tester {
namespace business {

struct test
{
    template<typename t_function>
    bool operator()(const std::string& p_name, t_function&& p_function)
    {
        using namespace tenacitas::logger::business;
        bool _result = false;
        try {
            log::test("############ ", __LINE__, p_name);
            _result = p_function();
        } catch (std::exception& _ex) {
            log::fatal("ERROR ", p_name, ": '", _ex.what(), "'");
            _result = false;
        }
        log::test(p_name, __LINE__, "############");
        return _result;
    }
};

} // namespace business
} // namespace tester
} // namespace tenacitas

#endif // TEST_H

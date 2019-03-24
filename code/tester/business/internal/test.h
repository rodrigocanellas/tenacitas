#ifndef TENACITAS_TESTER_BUS_TEST_H
#define TENACITAS_TESTER_BUS_TEST_H

#include <functional>
#include <string>

#include <logger/business/cerr.h>

namespace tenacitas {
namespace tester {
namespace business {

struct test
{
    template<typename t_function>
    bool operator()(const std::string& p_name, t_function && p_function)
    {
        bool _result = false;
        try {
            cerr_test("############ ", p_name);
            _result = p_function();
        } catch (std::exception& _ex) {
            cerr_fatal("ERROR ", p_name, ": '", _ex.what(), "'");
            _result = false;
        }
        cerr_test(p_name, " ############");
        return _result;
    }
};

} // namespace business
} // namespace tester
} // namespace tenacitas

#endif // TEST_H

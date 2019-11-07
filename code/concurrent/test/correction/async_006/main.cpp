
#include <chrono>
#include <cstdint>
#include <ctime>
#include <functional>
#include <sstream>
#include <string>

#include <concurrent/async.h>
#include <concurrent/business/thread.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#define DESCRIPTION                                                            \
    "No time out of a lambda function with a string and float parameteres, "   \
    "and returning int16_t."

typedef tenacitas::concurrent::business::async<int16_t> async_t;

struct work1
{

    int16_t operator()(const std::string& p_str, float p_f)
    {
        cerr_test("going to sleep");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cerr_test("woke up");
        cerr_test("str = '", p_str, "', f = ", p_f);
        return 3185;
    }
};

struct async_006
{
    bool operator()()
    {

        std::string _str("hello, world!");
        float _f(3.14f);

        m_async([&_str, &_f]() -> int16_t { return work1()(_str, _f); },
                std::chrono::milliseconds(501));

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        std::pair<bool, int16_t> _result = m_async.timeout();

        if (!_result.first) {
            cerr_test("result = ", _result.second);
        }
        return !_result.first;
    }

  private:
    async_t m_async;
};

int
main(int argc, char** argv)
{
    run_test(async_006, argc, argv, DESCRIPTION);
}

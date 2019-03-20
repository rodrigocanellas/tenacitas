
#include <chrono>
#include <cstdint>
#include <ctime>
#include <functional>
#include <sstream>
#include <string>

#include <concurrent/async.h>
#include <concurrent/bus/thread.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

#define DESCRIPTION                                                            \
    "No time out of a lambda function with no parameters, and returning "      \
    "int16_t."

typedef tenacitas::concurrent::async<int16_t> async_t;

struct work1
{

    int16_t operator()()
    {
        cerr_test("going to sleep");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cerr_test("woke up");
        return 3185;
    }
};

struct async_005
{
    bool operator()()
    {
        std::function<int16_t()> _function = []() { return work1()(); };

        m_async(_function, std::chrono::milliseconds(501));

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
    run_test(async_005, argc, argv, DESCRIPTION);
}

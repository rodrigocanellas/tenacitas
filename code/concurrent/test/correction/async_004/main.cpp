
#include <chrono>
#include <cstdint>
#include <ctime>
#include <functional>
#include <sstream>
#include <string>

#include <concurrent/business/thread.h>
#include <concurrent/async.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

#define DESCRIPTION                                                            \
    "Time out of a function object with no parameters, and returning "         \
    "int16_t."

typedef tenacitas::concurrent::business::async<int16_t> async_t;

struct work1
{

    int16_t operator()()
    {
        cerr_test("going to sleep");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cerr_test("woke up");
        return -15;
    }
};

struct async_004
{
    bool operator()()
    {
        work1 _work;

        std::function<int16_t()> _function = _work;

        m_async(_function, std::chrono::milliseconds(499));

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        std::pair<bool, int16_t> _result =m_async.timeout();

        if (!_result.first) {
            cerr_test("result = ", _result.second);
        }
        return _result.first;
    }

  private:
    async_t m_async;
};

int
main(int argc, char** argv)
{
    run_test(async_004, argc, argv, DESCRIPTION);
}

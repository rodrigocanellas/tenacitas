
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
    "Time out of a function object with a string and float parameteres, "      \
    "with no return."

typedef tenacitas::concurrent::async<void> async_t;

struct work1
{

    void operator()(const std::string& p_str, float p_f)
    {
        cerr_test("going to sleep");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cerr_test("woke up");
        cerr_test("str = '", p_str, "', f = ", p_f);
    }
};

struct async_007
{
    bool operator()()
    {

        std::string _str("hello, world!");
        float _f(3.14f);

        std::function<void(std::string&&, float&&)> _function =
          std::bind(work1(), std::placeholders::_1, std::placeholders::_2);

        m_async([&_str, &_f]() -> void { work1()(_str, _f); },
                std::chrono::milliseconds(499));

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        bool timeout = m_async.timeout();

        if (timeout) {
            cerr_test("TIME OUT");
        } else {
            cerr_test("NO TIME OUT");
        }
        return timeout;
    }

  private:
    async_t m_async;
};

int
main(int argc, char** argv)
{
    run_test(async_007, argc, argv, DESCRIPTION);
}

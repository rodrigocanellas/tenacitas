
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
    "Time out of a function object with no parameters, and returning void."

typedef tenacitas::concurrent::business::async<void> async_t;

struct work1
{

    void run()
    {
        cerr_test("going to sleep");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cerr_test("woke up");
    }
};

struct async_002
{
    bool operator()()
    {

        std::function<void()> _function = std::bind(&work1::run, work1());
        m_async(_function, std::chrono::milliseconds(499));

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        return m_async.timeout();
    }

  private:
    async_t m_async;
};

int
main(int argc, char** argv)
{
    run_test(async_002, argc, argv, DESCRIPTION);
}

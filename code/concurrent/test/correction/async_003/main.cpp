
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
    "No time out of a function object with no parameters, and returning "      \
    "int16_t."

typedef tenacitas::concurrent::business::async<int16_t> async_t;

struct work1
{

    int16_t run()
    {
        cerr_test("going to sleep");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cerr_test("woke up");
        return -15;
    }
};

struct async_003
{
    bool operator()()
    {
        work1 _work;

        std::function<int16_t()> _function =
          std::bind(&work1::run, std::ref(_work));

        m_async(_function, std::chrono::milliseconds(501));

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        std::pair<bool, int16_t> _result =m_async.timeout();

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
    run_test(async_003, argc, argv, DESCRIPTION);
}

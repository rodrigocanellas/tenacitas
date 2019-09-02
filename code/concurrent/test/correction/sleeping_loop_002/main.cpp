
/// \example
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <calendar/business/epoch.h>
#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

typedef tenacitas::concurrent::business::sleeping_loop_t<void> loop_t;

struct work1
{
    bool operator()()
    {
        ++counter;
        cerr_test(counter);
        return true;
    }
    uint64_t counter = 0;
};

struct sleeping_loop_002
{

    bool operator()()
    {
        work1 _work;
        loop_t _loop(std::chrono::milliseconds(1000),
                     [&_work]() { return _work(); },
                     std::chrono::milliseconds(500));

        _loop.run();
        std::this_thread::sleep_for(std::chrono::seconds(10));
        _loop.stop();

        cerr_test("data = ", _work.counter);
        if (_work.counter != 10) {
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(sleeping_loop_002,
             argc,
             argv,
             "\n'sleeping_loop' with interval of 1000 ms, work time out of 500 "
             "ms, increments a counter, and just prints using 'cerr_test', so "
             "there will be no time out."

             "\nThe main function will sleep for 10 secs, and the "
             "'sleeping_loop' will stop by calling 'stop'."

             "\nWork counter must be 10");
}


#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <calendar/bus/epoch.h>
#include <concurrent/bus/sleeping_loop.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

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

typedef tenacitas::concurrent::bus::sleeping_loop<void> loop_t;

struct sleeping_loop_001
{

    bool operator()()
    {
        work1 _work;
        loop_t _loop(std::chrono::milliseconds(1000),
                     [&_work]() { return _work(); },
                     std::chrono::milliseconds(500));
        _loop.run();
        std::this_thread::sleep_for(std::chrono::seconds(10));

        cerr_test("data = ", _work.counter);
        if (_work.counter != 10) {
            return false;
        }

        return true;
    }

  private:
};

int
main(int argc, char** argv)
{
    run_test(sleeping_loop_001,
             argc,
             argv,
             "\n'sleeping_loop' with interval of 1000 ms, work time out of 500 "
             "ms, increments a counter, and just prints using 'cerr_test', so "
             "there will be no time out."

             "\nThe main function will sleep for 10 secs, and the "
             "'sleeping_loop' will stop in the destructor."

             "\nCounter should be 11");
}

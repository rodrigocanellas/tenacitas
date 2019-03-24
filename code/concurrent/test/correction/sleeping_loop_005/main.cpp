
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

typedef tenacitas::concurrent::business::sleeping_loop<void> loop_t;

struct sleeping_loop_005
{
    bool operator()()
    {
        work1 _work;
        loop_t _loop_1(std::chrono::milliseconds(1000),
                       [&_work]() { return _work(); },
                       std::chrono::milliseconds(500));

        _loop_1.run();
        std::this_thread::sleep_for(std::chrono::seconds(10));

        cerr_test("data = ", _work.counter);
        if (_work.counter != 10) {
            return false;
        }
        _loop_1.stop();

        loop_t _loop_2(std::chrono::milliseconds(1000),
                       [&_work]() { return _work(); },
                       std::chrono::milliseconds(500));

        _loop_2.run();
        std::this_thread::sleep_for(std::chrono::seconds(3));

        cerr_test("data = ", _work.counter);
        if (_work.counter != 13) {
            return false;
        }
        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(
      sleeping_loop_005,
      argc,
      argv,
      "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
      "increments a counter, and just prints using 'cerr_test', so there will "
      "be no time out."

      "\nA 'sleeping_loop' will be started, the main thread will sleep "
      "for 10 secs, another 'sleeping_loop' will be created using the same "
      "parameters as the first one, the main thread will sleep for 3 secs, the "
      "new 'sleeping_loop' will stop by its destructor."

      "\nCounter should be 13.");
}


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

struct sleeping_loop_007
{

    bool operator()()
    {
        work1 _work;
        loop_t _loop_1(std::chrono::milliseconds(1000),
                       [&_work]() { return _work(); },
                       std::chrono::milliseconds(500));
        _loop_1.run();
        std::this_thread::sleep_for(std::chrono::seconds(10));
        _loop_1.stop();

        cerr_test("data = ", _work.counter);
        if (_work.counter != 10) {
            return false;
        }

        _loop_1.stop();

        loop_t _loop_2(
          _loop_1.get_interval(), _loop_1.get_work(), _loop_1.get_timeout());

        std::this_thread::sleep_for(std::chrono::seconds(1));

        _loop_2.run();
        std::this_thread::sleep_for(std::chrono::seconds(5));

        cerr_test("data = ", _work.counter);
        if (_work.counter != 15) {
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(
      sleeping_loop_007,
      argc,
      argv,
      "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
      "increments a counter, and just prints using 'cerr_test', so there will "
      "be no time out."

      "\nA 'sleeping_loop' will be started, the main thread will sleep for 10 "
      "secs, the 'sleeping_loop' will be stopped, another 'sleeping_loop' will "
      "be created using the same parameters as the first one, the new "
      "'sleeping_loop' will be started, the main thread will sleep for 5 secs, "
      "the new 'sleeping_loop' will stop in its destructor."

      "\nCounter should be 15.");
}

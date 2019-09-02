
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

struct work2
{
    bool operator()()
    {
        counter += 100;
        cerr_test(counter);
        return true;
    }
    uint64_t counter = 0;
};

struct work3
{
    bool operator()()
    {
        counter += 1000;
        cerr_test(counter);
        return true;
    }
    uint64_t counter = 0;
};

struct sleeping_loop_010
{

    bool operator()()
    {
        work1 _work_1;
        loop_t _loop_1(std::chrono::milliseconds(1000),
                       [&_work_1]() { return _work_1(); },
                       std::chrono::milliseconds(100));

        work2 _work_2;
        loop_t _loop_2(std::chrono::milliseconds(1000),
                       [&_work_2]() { return _work_2(); },
                       std::chrono::milliseconds(100));

        work3 _work_3;
        loop_t _loop_3(std::chrono::milliseconds(1000),
                       [&_work_3]() { return _work_3(); },
                       std::chrono::milliseconds(100));

        _loop_1.run();
        _loop_2.run();
        _loop_3.run();

        std::this_thread::sleep_for(std::chrono::seconds(6));

        cerr_test("stopping 2");
        _loop_2.stop();

        std::this_thread::sleep_for(std::chrono::seconds(3));

        cerr_test("data 1 = ", _work_1.counter);
        if (_work_1.counter != 9) {
            return false;
        }

        cerr_test("data 2 = ", _work_2.counter);
        if (_work_2.counter != 600) {
            return false;
        }

        cerr_test("data 3 = ", _work_3.counter);
        if (_work_3.counter != 9000) {
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(
      sleeping_loop_010,
      argc,
      argv,
      "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of 500 "
      "ms."

      "\nThe first one increments a counter by 1; the second by 100, and the "
      "third by 1000."

      "\nAll will be started, the main thread will sleep for 6 secs, the "
      "second 'sleeping_loop' will be stopped, the main thread will sleep for "
      "seconds all 'sleeping_loop' will stopped by theirs destructors."

      "\nCounter 1 should be 6, counter 2 should be 900, and counter 3 should "
      "be 6000.");
}

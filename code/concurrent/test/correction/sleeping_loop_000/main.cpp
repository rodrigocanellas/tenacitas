
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

struct sleeping_loop_000
{
    bool operator()()
    {
        typedef tenacitas::concurrent::business::sleeping_loop<void> loop_t;

        loop_t _loop(std::chrono::milliseconds(100),
                     [] {
                         cerr_test("loop1");
                         return true;
                     },
                     std::chrono::milliseconds(100));

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(sleeping_loop_000, argc, argv, "'sleeping_loop' creation test");
}

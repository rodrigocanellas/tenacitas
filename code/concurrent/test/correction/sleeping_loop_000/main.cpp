
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
#include <logger/business/log.h>
#include <tester/business/run.h>

#include <concurrent/test/log.h>

struct sleeping_loop_000
{

    bool operator()()
    {
        using namespace tenacitas::logger::business;
        configure_cerr_log();

        typedef tenacitas::concurrent::business::sleeping_loop_t<void, log>
          loop;

        loop _loop(std::chrono::milliseconds(100),
                   [] {
                       log_test("loop1");
                       return true;
                   },
                   std::chrono::milliseconds(100));

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(sleeping_loop_000, argc, argv, "'sleeping_loop' creation test")
}

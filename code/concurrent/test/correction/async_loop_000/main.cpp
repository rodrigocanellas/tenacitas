
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/bus/thread.h>
#include <concurrent/bus/internal//async_loop.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

struct work1
{

    bool operator()()
    {
        if (counter > 100) {
            return false;
        }
        cerr_test(counter);
        ++counter;
        return true;
    }

    uint16_t counter = 0;
};

typedef tenacitas::concurrent::bus::async_loop<void> async_loop_t;

struct async_loop_000
{
    bool operator()()
    {
        bool _result = true;
        try {
            work1 _work;
            async_loop_t _async_loop([&_work]() -> bool { return _work(); },
                                     std::chrono::milliseconds(100),
                                     []() -> bool { return false; });

            _async_loop.run();

            std::this_thread::sleep_for(std::chrono::seconds(4));

            cerr_test("counter = ", _work.counter);
            _result = (_work.counter == 101);

        } catch (std::exception& _ex) {
            cerr_fatal("ERROR async_loop_000: '", _ex.what(), "'");
            return false;
        }
        return _result;
    }
};

int
main(int argc, char** argv)
{
    run_test(
      async_loop_000,
      argc,
      argv,
      "\nWork in 'async_loop' will stop when a internal counter reaches 100."

      "\nMain thread will sleep for 4 secs."

      "\nCounter should be 101.");
}

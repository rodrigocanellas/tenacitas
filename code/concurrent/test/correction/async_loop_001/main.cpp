
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/business/thread.h>
#include <concurrent/business/internal//async_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct work1
{

    bool operator()()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        cerr_test(counter);
        ++counter;
        return true;
    }

    uint64_t counter = 0;
};

typedef tenacitas::concurrent::business::async_loop_t<void> async_loop_t;

struct async_loop_001
{
    bool operator()()
    {
        try {
            work1 _work;
            async_loop_t _async_loop([&_work]() -> bool { return _work(); },
                                     std::chrono::milliseconds(1000),
                                     []() -> bool { return false; });
            _async_loop.run();

            std::this_thread::sleep_for(std::chrono::seconds(1));

            cerr_test("counter = ", _work.counter);
            if (_work.counter != 4) {
                cerr_error("wrong value for data, it should be 4");
                return false;
            }
        } catch (std::exception& _ex) {
            cerr_error(_ex.what());
            return false;
        }

        return true;
    }
};

int
main(int argc, char** argv)
{
    run_test(
      async_loop_001,
      argc,
      argv,
      "\nWork in 'async_loop' will increment an internal counter indefinetly, "
      "at each 200 ms."

      "\nMain thread will stop for 1 sec."

      "\nCounter should be 4.");
}

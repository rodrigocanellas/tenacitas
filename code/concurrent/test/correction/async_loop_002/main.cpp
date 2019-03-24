
#include <chrono>
#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>

#include <concurrent/business/internal//async_loop.h>
#include <concurrent/business/thread.h>
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

typedef tenacitas::concurrent::business::async_loop<void> async_loop_t;

struct async_loop_002
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

            _async_loop.stop();

            std::this_thread::sleep_for(std::chrono::seconds(1));

            _async_loop.run();

            std::this_thread::sleep_for(std::chrono::seconds(2));

            cerr_test("counter = ", _work.counter);
            if (_work.counter != 14) {
                cerr_error("wrong value for data, it should be 14");
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
      async_loop_002,
      argc,
      argv,
      "\nWork in 'async_loop' will increment an internal counter indefinetly, "
      "with a 200ms sleep."

      "\nMain thread will stop for 1 secs; counter should be 4; 'async_loop' "
      "will stop; main thread will sleep for 2 secs; 'async_loop' will run "
      "again; counter should be 14");
}

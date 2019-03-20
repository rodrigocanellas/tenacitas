
#include <chrono>
#include <cstdint>
#include <ctime>
#include <functional>
#include <sstream>
#include <string>

#include <concurrent/async.h>
#include <concurrent/bus/thread.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

#define DESCRIPTION "Creation of a 'async' object."

typedef tenacitas::concurrent::async<void> async;

struct work1
{

    bool run()
    {
        cerr_test("going to sleep");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cerr_test("woke up");
        return true;
    }
};

struct timeout_000
{
    bool operator()() { return true; }

  private:
    async m_async;
};

int
main(int argc, char** argv)
{
    run_test(timeout_000, argc, argv, DESCRIPTION);
}

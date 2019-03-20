#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

struct thread_pool_019
{
    bool operator()()
    {
        thread_pool_tester<7, 10, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_019,
             argc,
             argv,
             "7 consumers, 10 msgs, 2800 work sleep");
}

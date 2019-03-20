#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

struct thread_pool_021
{
    bool operator()()
    {
        thread_pool_tester<7, 100, 300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_021,
             argc,
             argv,
             "7 consumers, 100 msgs, 300 work sleep");
}

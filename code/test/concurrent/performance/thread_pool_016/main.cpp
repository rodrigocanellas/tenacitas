#include <business/concurrent/thread_pool.h>
#include <test/concurrent/performance/thread_pool_tester.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

struct thread_pool_016
{
    bool operator()()
    {
        thread_pool_tester<7, 1, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_016,
             argc,
             argv,
             "7 consumers, 1 msgs, 2800 work sleep");
}

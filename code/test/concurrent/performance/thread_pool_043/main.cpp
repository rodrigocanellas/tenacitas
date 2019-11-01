#include <business/concurrent/thread_pool.h>
#include <test/concurrent/performance/thread_pool_tester.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

struct thread_pool_043
{
    bool operator()()
    {
        thread_pool_tester<12, 10000, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_043,
             argc,
             argv,
             "12 consumers, 10000 msgs, 2800 work sleep");
}

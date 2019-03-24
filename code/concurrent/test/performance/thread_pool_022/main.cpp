#include <concurrent/business/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct thread_pool_022
{
    bool operator()()
    {
        thread_pool_tester<7, 100, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_022,
             argc,
             argv,
             "7 consumers, 100 msgs, 2800 work sleep");
}

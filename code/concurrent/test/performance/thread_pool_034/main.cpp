#include <concurrent/business/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct thread_pool_034
{
    bool operator()()
    {
        thread_pool_tester<12, 10, 2800>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_034,
             argc,
             argv,
             "12 consumers, 10 msgs, 2800 work sleep");
}

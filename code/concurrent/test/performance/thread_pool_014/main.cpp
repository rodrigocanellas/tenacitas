#include <concurrent/business/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct thread_pool_014
{
    bool operator()()
    {
        thread_pool_tester<2, 10000, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_014,
             argc,
             argv,
             "2 consumers, 10000 msgs, 5300 work sleep");
}

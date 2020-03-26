#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_038
{
    bool operator()()
    {
        thread_pool_tester<12, 100, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_038,
             argc,
             argv,
             "12 consumers, 100 msgs, 5300 work sleep");
}

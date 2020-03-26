#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_024
{
    bool operator()()
    {
        thread_pool_tester<7, 1000, 300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_024,
             argc,
             argv,
             "7 consumers, 1000 msgs, 300 work sleep");
}

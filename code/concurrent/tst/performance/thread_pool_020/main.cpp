#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_020
{
    bool operator()()
    {
        thread_pool_tester<7, 10, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_020,
             argc,
             argv,
             "7 consumers, 10 msgs, 5300 work sleep");
}

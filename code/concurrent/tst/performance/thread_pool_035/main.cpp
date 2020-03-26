#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_035
{
    bool operator()()
    {
        thread_pool_tester<12, 10, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_035,
             argc,
             argv,
             "12 consumers, 10 msgs, 5300 work sleep");
}

#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/performance/thread_pool_tester.h>
#include <logger/cerr/log.h>


struct thread_pool_008
{
    bool operator()()
    {
        thread_pool_tester<2, 100, 5300>()();
        return true;
    }
};
int
main(int argc, char** argv)
{
    run_test(thread_pool_008,
             argc,
             argv,
             "2 consumers, 100 msgs, 5300 work sleep");
}

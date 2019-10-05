#include <iostream>
#include <utility>

#include <crosswords/business/internal/log.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/business/internal/matrix.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::crosswords::business;
using namespace tenacitas::crosswords::entities;
using namespace tenacitas::logger::business;

struct matrix_000
{
        bool operator()()
        {
            typedef coordinate::x x;
            typedef coordinate::y y;

            try {
                matrix<tenacitas::logger::business::log> _m(coordinate::x(3)
                                                            , coordinate::y(2));

                _m.set(x(0), y(0), 'w');
                _m.set(x(2), y(1), 'u');

                _m.print();

                if (_m.get(x(0), y(0)) != 'w') {
                    return false;
                }

                if (_m.get(x(2), y(1)) != 'u') {
                    return false;
                }




            } catch (std::exception & _ex) {
                crosswords_log_error(log, _ex.what());
                return false;
            }

            return true;

        }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(matrix_000, argc, argv, "matrix printing");
    return 0;
}

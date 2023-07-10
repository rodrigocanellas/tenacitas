#ifndef TENACITAS_TST_ASYNC_DISPATCHER_DISPATCHER_H
#define TENACITAS_TST_ASYNC_DISPATCHER_DISPATCHER_H

#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/tst/async/dispatcher/evt.h>

namespace bus {

using dispatcher =
    async::alg::dispatcher<evt::pressure_generated, evt::pressure_sent,
                           evt::pressure_handled>;

}
#endif

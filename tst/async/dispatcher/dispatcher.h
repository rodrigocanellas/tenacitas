#ifndef TENACITAS_TST_ASYNC_DISPATCHER_DISPATCHER_H
#define TENACITAS_TST_ASYNC_DISPATCHER_DISPATCHER_H

#include "evt.h"
#include <tenacitas/async/alg/dispatcher.h>

namespace bus {

using dispatcher =
    async::alg::dispatcher<evt::pressure_generated, evt::pressure_sent,
                           evt::pressure_handled>;

}
#endif

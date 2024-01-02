#ifndef TENACITAS_TST_ASYNC_DISPATCHER_DISPATCHER_H
#define TENACITAS_TST_ASYNC_DISPATCHER_DISPATCHER_H

#include "evt.h"
#include <tenacitas.h>

namespace alg {

using dispatcher = tnctl::dispatcher_a<evt::pressure_generated,
                                     evt::pressure_sent, evt::pressure_handled>;

}
#endif

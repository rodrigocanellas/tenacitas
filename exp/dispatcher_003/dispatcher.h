#ifndef TNCT_TST_ASYNC_DISPATCHER_DISPATCHER_H
#define TNCT_TST_ASYNC_DISPATCHER_DISPATCHER_H

#include "evt.h"
#include <tenacitas.lib/src/alg/dispatcher.h>

namespace alg {

using dispatcher = tla::dispatcher<evt::pressure_generated, evt::pressure_sent,
                                   evt::pressure_handled>;

}
#endif

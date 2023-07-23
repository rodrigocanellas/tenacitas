TEMPLATE=aux

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS += \
        $$LIB_SRC_DIR/tenacitas.lib/async/alg/dispatcher.h \
        $$LIB_SRC_DIR/tenacitas.lib/async/alg/dispatcher.h \
        $$LIB_SRC_DIR/tenacitas.lib/async/alg/execute.h \
        $$LIB_SRC_DIR/tenacitas.lib/async/alg/sleeping_loop.h

HEADERS += \
        $$LIB_SRC_DIR/tenacitas.lib/async/cpt/event.h \
        $$LIB_SRC_DIR/tenacitas.lib/async/cpt/dispatcher.h

HEADERS += \
        $$LIB_SRC_DIR/tenacitas.lib/async/internal/typ/event_queue.h

HEADERS += \
        $$LIB_SRC_DIR/tenacitas.lib/async/typ/priority.h \
        $$LIB_SRC_DIR/tenacitas.lib/async/typ/queue_id.h \
        $$LIB_SRC_DIR/tenacitas.lib/async/typ/subscriber.h \

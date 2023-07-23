TEMPLATE=aux

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS += \
        $$LIB_SRC_DIR/tenacitas/async/alg/dispatcher.h \
        $$LIB_SRC_DIR/tenacitas/async/alg/dispatcher.h \
        $$LIB_SRC_DIR/tenacitas/async/alg/execute.h \
        $$LIB_SRC_DIR/tenacitas/async/alg/sleeping_loop.h

HEADERS += \
        $$LIB_SRC_DIR/tenacitas/async/cpt/event.h \
        $$LIB_SRC_DIR/tenacitas/async/cpt/dispatcher.h

HEADERS += \
        $$LIB_SRC_DIR/tenacitas/async/internal/typ/event_queue.h

HEADERS += \
        $$LIB_SRC_DIR/tenacitas/async/typ/priority.h \
        $$LIB_SRC_DIR/tenacitas/async/typ/queue_id.h \
        $$LIB_SRC_DIR/tenacitas/async/typ/subscriber.h \

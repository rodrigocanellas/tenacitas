TEMPLATE=aux

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS += \
        $$SRC_DIR/tenacitas/async/alg/dispatcher.h \
        $$SRC_DIR/tenacitas/async/alg/dispatcher.h \
        $$SRC_DIR/tenacitas/async/alg/execute.h \
        $$SRC_DIR/tenacitas/async/alg/sleeping_loop.h

HEADERS += \
        $$SRC_DIR/tenacitas/async/cpt/event.h \
        $$SRC_DIR/tenacitas/async/cpt/dispatcher.h

HEADERS += \
        $$SRC_DIR/tenacitas/async/internal/typ/event_queue.h

HEADERS += \
        $$SRC_DIR/tenacitas/async/typ/priority.h \
        $$SRC_DIR/tenacitas/async/typ/queue_id.h \
        $$SRC_DIR/tenacitas/async/typ/subscriber.h \

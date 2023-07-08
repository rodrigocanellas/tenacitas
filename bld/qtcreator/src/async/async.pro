TEMPLATE=aux

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/async/alg/dispatcher.h \
        $$BASE_DIR/tenacitas.lib/src/async/alg/dispatcher.h \
        $$BASE_DIR/tenacitas.lib/src/async/alg/execute.h \
        $$BASE_DIR/tenacitas.lib/src/async/alg/sleeping_loop.h

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/async/cpt/event.h \
        $$BASE_DIR/tenacitas.lib/src/async/cpt/dispatcher.h

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/async/internal/typ/queue.h

HEADERS += \
        $$BASE_DIR/tenacitas.lib/src/async/typ/priority.h \
        $$BASE_DIR/tenacitas.lib/src/async/typ/queue_id.h \
        $$BASE_DIR/tenacitas.lib/src/async/typ/subscriber.h \

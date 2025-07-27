include (../../common.pri)

TEMPLATE=aux

PRJ_DIR=$$BASE_DIR/tenacitas/src/async

HEADERS += \
        $$PRJ_DIR/sleeping_loop.h \
        $$PRJ_DIR/exec_sync.h \
        $$PRJ_DIR/dispatcher.h \
        $$PRJ_DIR/handling_priority.h \
        $$PRJ_DIR/handling_name.h \
        $$PRJ_DIR/result.h \
        $$PRJ_DIR/internal/handling.h \
        $$PRJ_DIR/internal/handler_id.h \
        $$PRJ_DIR/internal/handling_id.h



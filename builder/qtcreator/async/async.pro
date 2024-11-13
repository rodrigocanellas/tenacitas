include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=\
     test
     # \
    # example

HEADERS += \
        $$BASE_DIR/tenacitas.lib/async/sleeping_loop.h \
        $$BASE_DIR/tenacitas.lib/async/dispatcher.h \
        $$BASE_DIR/tenacitas.lib/async/exec_sync.h \
        $$BASE_DIR/tenacitas.lib/async/handling_priority.h \
        $$BASE_DIR/tenacitas.lib/async/handling_id.h \
        $$BASE_DIR/tenacitas.lib/async/result.h \
        $$BASE_DIR/tenacitas.lib/async/internal/handling.h \
        $$BASE_DIR/tenacitas.lib/async/internal/handlings.h \
        $$BASE_DIR/tenacitas.lib/async/handler.h



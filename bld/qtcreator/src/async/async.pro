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
        $$PRJ_DIR/internal/handling_id.h \
        $$PRJ_DIR/traits/is_dispatcher.h  \
        $$PRJ_DIR/traits/is_event.h  \
        $$PRJ_DIR/traits/is_handler.h  \
        $$PRJ_DIR/traits/has_add_handling_method.h  \
        $$PRJ_DIR/traits/has_events_handled.h  \
        $$PRJ_DIR/traits/has_events_published.h  \
        $$PRJ_DIR/traits/has_publish_method.h  \
        $$PRJ_DIR/traits/tuple_contains_only_events.h


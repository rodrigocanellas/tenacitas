include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst \
          exp

PRJ_DIR=$$BASE_DIR/tnct/async

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
        $$PRJ_DIR/cpt/is_dispatcher.h  \
        $$PRJ_DIR/cpt/is_event.h  \
        $$PRJ_DIR/cpt/is_handler.h  \
        $$PRJ_DIR/cpt/has_add_handling_method.h  \
        $$PRJ_DIR/cpt/has_events_handled.h  \
        $$PRJ_DIR/cpt/has_events_published.h  \
        $$PRJ_DIR/cpt/has_publish_method.h  \
        $$PRJ_DIR/cpt/tuple_contains_only_events.h


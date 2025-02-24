include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=test

PRJ_DIR=$$BASE_DIR/tnct/traits

HEADERS += \
        $$PRJ_DIR/is_pair.h \
        $$PRJ_DIR/is_smart_ptr.h \
        $$PRJ_DIR/event.h \
        $$PRJ_DIR/publisher.h \
        $$PRJ_DIR/subscriber.h \
        $$PRJ_DIR/dispatcher.h \
        $$PRJ_DIR/chrono_convertible.h \
        $$PRJ_DIR/has_new_operator.h \
        $$PRJ_DIR/has_output_operator.h \
        $$PRJ_DIR/queue.h \
        $$PRJ_DIR/log_writer.h \
        $$PRJ_DIR/logger.h \
        $$PRJ_DIR/i_queue.h \
        $$PRJ_DIR/handler.h \
        $$PRJ_DIR/handling.h \
        $$PRJ_DIR/handlings.h \
        $$PRJ_DIR/is_tuple.h \
        $$PRJ_DIR/tuple_like.h \
        $$PRJ_DIR/pair_like.h \
        $$PRJ_DIR/tuple_contains_tuple.h \
        $$PRJ_DIR/is_type_in_tuple.h \
        $$PRJ_DIR/tuple_find.h \
        $$PRJ_DIR/visit_tuple.h \
        $$PRJ_DIR/chrono_convertible.h \
        $$PRJ_DIR/tuple_contains_only_events.h \
        $$PRJ_DIR/tuple_size_greather_or_equal.h \
        $$PRJ_DIR/is_pair.h \



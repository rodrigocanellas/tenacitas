include (../../common.pri)

TEMPLATE=aux

PRJ_DIR=$$BASE_DIR/tenacitas/src/traits

HEADERS += \
        $$PRJ_DIR/is_smart_ptr.h \
        $$PRJ_DIR/is_hashable.h \
        $$PRJ_DIR/chrono_convertible.h \
        $$PRJ_DIR/has_new_operator.h \
        $$PRJ_DIR/has_output_operator.h \
        $$PRJ_DIR/scoped_enum.h \
        $$PRJ_DIR/pair/is.h \
        $$PRJ_DIR/pair/like.h \
        $$PRJ_DIR/container/queue.h \
        $$PRJ_DIR/log/writer.h \
        $$PRJ_DIR/log/logger.h \
        $$PRJ_DIR/tuple/is.h \
        $$PRJ_DIR/tuple/is_tuple.h \
        $$PRJ_DIR/tuple/contains_tuple.h \
        $$PRJ_DIR/tuple/contains_type.h \
        $$PRJ_DIR/tuple/find.h \
        $$PRJ_DIR/tuple/visit.h \
        $$PRJ_DIR/tuple/traverse.h \
        $$PRJ_DIR/tuple/size_greather_or_equal.h \
        $$PRJ_DIR/string/fixed_size_string.h



# $$PRJ_DIR/async/event.h \
# $$PRJ_DIR/async/has_events_published.h \
# $$PRJ_DIR/async/has_events_handled.h \
# $$PRJ_DIR/async/dispatcher.h \
# $$PRJ_DIR/async/tuple_contains_only_events.h \
# $$PRJ_DIR/async/publish_method.h \
# $$PRJ_DIR/async/has_add_handling_method.h \
# $$PRJ_DIR/async/handler.h \

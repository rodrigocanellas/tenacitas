include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=test


HEADERS += \
        $$BASE_DIR/tenacitas.lib/traits/is_pair.h \
        $$BASE_DIR/tenacitas.lib/traits/is_smart_ptr.h \
        $$BASE_DIR/tenacitas.lib/traits/event.h \
        $$BASE_DIR/tenacitas.lib/traits/publisher.h \
        $$BASE_DIR/tenacitas.lib/traits/subscriber.h \
        $$BASE_DIR/tenacitas.lib/traits/dispatcher.h \
        $$BASE_DIR/tenacitas.lib/traits/chrono_convertible.h \
        $$BASE_DIR/tenacitas.lib/traits/has_new_operator.h \
        $$BASE_DIR/tenacitas.lib/traits/has_output_operator.h \
        $$BASE_DIR/tenacitas.lib/traits/queue.h \
        $$BASE_DIR/tenacitas.lib/traits/log_writer.h \
        $$BASE_DIR/tenacitas.lib/traits/logger.h \
        $$BASE_DIR/tenacitas.lib/traits/i_queue.h \
        $$BASE_DIR/tenacitas.lib/traits/handler.h \
        $$BASE_DIR/tenacitas.lib/traits/handling.h \
        $$BASE_DIR/tenacitas.lib/traits/handlings.h \
        $$BASE_DIR/tenacitas.lib/traits/is_tuple.h \
        $$BASE_DIR/tenacitas.lib/traits/tuple_like.h \
        $$BASE_DIR/tenacitas.lib/traits/pair_like.h \
        $$BASE_DIR/tenacitas.lib/traits/tuple_contains_tuple.h \
        $$BASE_DIR/tenacitas.lib/traits/is_type_in_tuple.h \
        $$BASE_DIR/tenacitas.lib/traits/tuple_find.h \
        $$BASE_DIR/tenacitas.lib/traits/visit_value_in_tuple.h \
        $$BASE_DIR/tenacitas.lib/traits/visit_type_in_tuple.h \
        $$BASE_DIR/tenacitas.lib/traits/chrono_convertible.h




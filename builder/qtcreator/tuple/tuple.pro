include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
    $$BASE_DIR/tenacitas.lib/tuple/is_tuple.h  \
    $$BASE_DIR/tenacitas.lib/tuple/is_type_in_tuple.h  \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_contains_tuple.h  \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_find.h  \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_like.h  \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_transform.h  \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_traverse.h \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_output.h \

include (../../common.pri)

TEMPLATE=aux

PRJ_DIR=$$BASE_DIR/tenacitas/src/tuple

HEADERS += \
    $$PRJ_DIR/tuple_transform.h  \
    $$PRJ_DIR/tuple_traverse.h \
    $$PRJ_DIR/traits/is.h \
    $$PRJ_DIR/traits/is_tuple.h \
    $$PRJ_DIR/traits/contains_tuple.h \
    $$PRJ_DIR/traits/contains_type.h \
    $$PRJ_DIR/traits/find.h \
    $$PRJ_DIR/traits/visit.h \
    $$PRJ_DIR/traits/traverse.h \
    $$PRJ_DIR/traits/size_greather_or_equal.h

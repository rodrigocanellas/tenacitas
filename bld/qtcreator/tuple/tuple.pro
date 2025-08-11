include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst

PRJ_DIR=$$BASE_DIR/tnct/tuple

HEADERS += \
    $$PRJ_DIR/cpt/is_tuple.h \
    $$PRJ_DIR/cpt/contains_tuple.h \
    $$PRJ_DIR/cpt/visit.h \
    $$PRJ_DIR/cpt/size_greather_or_equal.h \
    $$PRJ_DIR/transform.h  \
    $$PRJ_DIR/traverse.h \
    $$PRJ_DIR/contains_type.h \
    $$PRJ_DIR/get_type_index.h \
    $$PRJ_DIR/internal/is_tuple_helper.h \
    $$PRJ_DIR/internal/contains_tuple_helper.h


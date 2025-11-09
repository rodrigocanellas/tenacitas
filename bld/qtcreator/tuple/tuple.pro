include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst

PRJ_DIR=$$BASE_DIR/tnct/tuple

HEADERS += \
    $$PRJ_DIR/cpt/is_tuple.h \
    $$PRJ_DIR/cpt/contains_tuple.h \
    $$PRJ_DIR/cpt/visit.h \
    $$PRJ_DIR/cpt/size_greather_or_equal.h \
    $$PRJ_DIR/bus/transform.h  \
    $$PRJ_DIR/bus/traverse.h \
    $$PRJ_DIR/bus/contains_type.h \
    $$PRJ_DIR/bus/get_type_index.h \
    $$PRJ_DIR/output.h \
    $$PRJ_DIR/internal/bus/is_tuple_helper.h \
    $$PRJ_DIR/internal/bus/contains_tuple_helper.h


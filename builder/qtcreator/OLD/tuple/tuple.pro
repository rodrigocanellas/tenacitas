include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
    $$BASE_DIR/tnct/tuple/tuple_transform.h  \
    $$BASE_DIR/tnct/tuple/tuple_traverse.h

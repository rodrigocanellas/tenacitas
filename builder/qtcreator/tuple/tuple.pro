include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=test

HEADERS += \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_transform.h  \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_traverse.h \
    $$BASE_DIR/tenacitas.lib/tuple/tuple_output.h

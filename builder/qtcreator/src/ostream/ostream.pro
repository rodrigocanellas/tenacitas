include (../../common.pri)

TEMPLATE=subdirs

SUBDIRS=\
     test

PRJ_DIR=$$BASE_SRC_DIR/tnct/ostream

HEADERS += \
        $$PRJ_DIR/tuple.h \
        $$PRJ_DIR/pair.h



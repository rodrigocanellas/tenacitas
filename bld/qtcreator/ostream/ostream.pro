include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


PRJ_DIR=$$BASE_DIR/tnct/ostream

HEADERS += \
        $$PRJ_DIR/tuple.h \
        $$PRJ_DIR/pair.h \
        $$PRJ_DIR/traits/has_output_operator.h



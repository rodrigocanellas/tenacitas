include (../../common.pri)

TEMPLATE=subdirs

SUBDIRS = test


PRJ_DIR=$$BASE_DIR/tenacitas/src/ostream

HEADERS += \
        $$PRJ_DIR/tuple.h \
        $$PRJ_DIR/pair.h \
        $$PRJ_DIR/traits/has_output_operator.h



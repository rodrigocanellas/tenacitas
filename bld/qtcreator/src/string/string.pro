include (../../common.pri)

TEMPLATE=subdirs

SUBDIRS = test


PRJ_DIR=$$BASE_DIR/tenacitas/src/string

HEADERS += \
    $$PRJ_DIR/fixed_size_string.h \
    $$PRJ_DIR/traits/fixed_size_string.h



include (../../common.pri)

TEMPLATE=subdirs

SUBDIRS = test


PRJ_DIR=$$BASE_DIR/tenacitas/src/traits

HEADERS += \
        $$PRJ_DIR/is_hashable.h \
        $$PRJ_DIR/scoped_enum.h \
        $$PRJ_DIR/is_pair.h


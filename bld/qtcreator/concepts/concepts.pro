include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


PRJ_DIR=$$BASE_DIR/tnct/concepts

HEADERS += \
        $$PRJ_DIR/is_hashable.h \
        $$PRJ_DIR/scoped_enum.h


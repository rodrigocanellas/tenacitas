include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


PRJ_DIR=$$BASE_DIR/tnct/string

HEADERS += \
    $$PRJ_DIR/fixed_size_string.h \
    $$PRJ_DIR/cpt/fixed_size_string.h



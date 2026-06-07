include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


PRJ_DIR=$$BASE_DIR/tnct/string

HEADERS += \
    $$PRJ_DIR/dat/*.h \
    $$PRJ_DIR/cpt/*.h



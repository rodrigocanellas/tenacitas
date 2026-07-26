include (../common.pri)

TEMPLATE=subdirs

PRJ_DIR=$$BASE_DIR/tnct/supplier


SUBDIRS = exp

HEADERS += \
        $$PRJ_DIR/cpt/*.h \
        $$PRJ_DIR/cpt/internal/*.h

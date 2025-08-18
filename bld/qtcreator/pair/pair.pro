include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst

PRJ_DIR=$$BASE_DIR/tnct/pair

HEADERS += \
    $$PRJ_DIR/cpt/is_pair.h \
    $$PRJ_DIR/output.h


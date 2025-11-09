include (../common.pri)

TEMPLATE=subdirs

SUBDIRS=tst

PRJ_DIR=$$BASE_DIR/tnct/io


HEADERS += \
    $$PRJ_DIR/cpt/reader.h \
    $$PRJ_DIR/cpt/writer.h

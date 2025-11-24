include (../common.pri)


TEMPLATE=subdirs

SUBDIRS = tst

HEADERS += \
        $$BASE_DIR/tnct/generic/dat/id.h \
        $$BASE_DIR/tnct/generic/cpt/is_hashable.h \
        $$BASE_DIR/tnct/generic/cpt/creator.h \
        $$BASE_DIR/tnct/generic/bus/creator.h



include (../common.pri)


TEMPLATE=subdirs

SUBDIRS = tst

HEADERS += \
        $$BASE_DIR/tnct/generic/dat/*.h \
        $$BASE_DIR/tnct/generic/cpt/*.h \
        $$BASE_DIR/tnct/generic/bus/*.h



include (../common.pri)


TEMPLATE=subdirs

SUBDIRS = tst \
          cpt

HEADERS += \
        $$BASE_DIR/tnct/generic/dat/*.h \
        $$BASE_DIR/tnct/generic/bus/*.h



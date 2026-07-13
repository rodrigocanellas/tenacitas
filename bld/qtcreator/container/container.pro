include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst \
          exp


HEADERS += \
        $$BASE_DIR/tnct/container/cpt/*.h \
        $$BASE_DIR/tnct/container/trt/*.h \
        $$BASE_DIR/tnct/container/dat/*.h \
        $$BASE_DIR/tnct/container/bus/*.h \
        $$BASE_DIR/tnct/container/internal/bus/*.h \
        $$BASE_DIR/tnct/container/internal/dat/*.h \
        $$BASE_DIR/tnct/container/internal/trt/*.h \
        $$BASE_DIR/tnct/container/internal/evt/*.h




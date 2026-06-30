include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst

PRJ_DIR=$$BASE_DIR/tnct/tuple

HEADERS += \
    $$PRJ_DIR/cpt/*.h           \
    $$PRJ_DIR/bus/*             \
    $$PRJ_DIR/*.h               \
    $$PRJ_DIR/internal/bus/*.h



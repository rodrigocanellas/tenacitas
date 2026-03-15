include (../common.pri)

TEMPLATE=subdirs

SUBDIRS = tst


PRJ_DIR=$$BASE_DIR/tnct/math

HEADERS += \
        $$PRJ_DIR/bus/factorial.h \
        $$PRJ_DIR/bus/moving_average.h \
        $$PRJ_DIR/bus/sum.h \
        $$PRJ_DIR/bus/sub.h \
        $$PRJ_DIR/bus/mul.h \
        $$PRJ_DIR/bus/internal/sum.h \
        $$PRJ_DIR/bus/internal/sub.h \
        $$PRJ_DIR/bus/internal/mul.h \
        $$PRJ_DIR/bus/internal/fits.h \
        $$PRJ_DIR/cpt/number.h \
        $$PRJ_DIR/bus/revert.h

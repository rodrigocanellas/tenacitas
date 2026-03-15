QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.math.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/math/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS=$$PRJ_DIR/sum.h \
        $$PRJ_DIR/sub.h \
        $$PRJ_DIR/mul.h \
        $$PRJ_DIR/moving_average.h

QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.pair.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/pair/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
    $$PRJ_DIR/output_test.h





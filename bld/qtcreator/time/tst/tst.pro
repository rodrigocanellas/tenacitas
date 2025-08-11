QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/time/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/chrono_output_test.h





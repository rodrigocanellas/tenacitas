QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tester.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/tester/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/test_test.h





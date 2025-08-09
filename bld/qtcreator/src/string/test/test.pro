QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.string.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/string/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/fixed_size_string_test.h





QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.ostream.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../../common.pri)

PRJ_DIR=$$BASE_SRC_DIR/tnct/ostream


SOURCES = $$PRJ_DIR/test/main.cpp

HEADERS = \
          $$PRJ_DIR/test/tuple_output_test.h \
          $$PRJ_DIR/test/pair_output_test.h





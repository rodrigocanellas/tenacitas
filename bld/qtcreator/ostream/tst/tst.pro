QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.ostream.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/ostream/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/tuple_output_test.h \
          $$PRJ_DIR/pair_output_test.h





QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tuple.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/tuple/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/tuple_transform_test.h  \
          $$PRJ_DIR/tuple_traverse_test.h





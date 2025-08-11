QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tuple.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/tuple/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/contains_type_test.h \
          $$PRJ_DIR/get_type_index_test.h \
          $$PRJ_DIR/tuple_transform_test.h  \
          $$PRJ_DIR/transform_test.h  \
          $$PRJ_DIR/traverse_test.h \
          $$PRJ_DIR/is_tuple_test.h \
          $$PRJ_DIR/contains_tuple_test.h




QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.string.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/string/tst


SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/fixed_size_string_test.h \
          $$PRJ_DIR/cpt_test.h \
          $$PRJ_DIR/cpt/fixed_size_string.h





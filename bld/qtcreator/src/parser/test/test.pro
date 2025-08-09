QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.parser.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/parser/tst


SOURCES = $$PRJ_DIR/main.cpp

DISTFILES = $$PRJ_DIR/test_000.ini






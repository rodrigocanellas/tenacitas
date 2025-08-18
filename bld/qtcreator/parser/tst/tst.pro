QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.parser.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/parser/tst


SOURCES = $$PRJ_DIR/main.cpp

DISTFILES = $$PRJ_DIR/test_000.ini






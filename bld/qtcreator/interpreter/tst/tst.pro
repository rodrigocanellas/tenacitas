QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.interpreter.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/interpreter/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
        $$PRJ_DIR/*.h

SOURCES = \
        $$PRJ_DIR/*.cpp






QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.concepts.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/concepts/tst



SOURCES = $$PRJ_DIR/main.cpp






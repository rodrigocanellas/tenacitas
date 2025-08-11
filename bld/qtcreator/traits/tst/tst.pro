QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.traits.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/traits/tst



SOURCES = $$PRJ_DIR/main.cpp






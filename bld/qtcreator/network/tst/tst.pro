QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.network.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/network/tst

# SOURCES = $$PRJ_DIR/main.cpp

# HEADERS = \
#          $$PRJ_DIR/cpt_test.h



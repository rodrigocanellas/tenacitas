QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.async.tst

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/async/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
         $$PRJ_DIR/dispatcher_test.h \
         $$PRJ_DIR/traits_test.h \
         $$PRJ_DIR/sleeping_loop_test.h \
         $$PRJ_DIR/handling_test.h


DISTFILES += $$PRJ_DIR/cfg_000.ini



QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tst.dispatcher

include (../../common.pri)
# QMAKE_CXXFLAGS += -DTENACITAS_LOG
SOURCES = $$BASE_DIR/tenacitas.lib/test/dispatcher/main.cpp



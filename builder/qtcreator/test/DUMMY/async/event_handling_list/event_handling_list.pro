QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tst.event_handling_list

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas.lib/test/event_handling_list/main.cpp



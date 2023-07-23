QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tst.async.sleeping_loop

include (../../../../../../tenacitas.bld/qtcreator/common.pri)


SOURCES = $$TST_DIR/async/sleeping_loop/main.cpp



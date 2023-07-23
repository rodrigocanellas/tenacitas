QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tst.async.dispatcher

include (../../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS = \
            $$LIB_TST_DIR/async/dispatcher/bus.h \
            $$LIB_TST_DIR/async/dispatcher/dispatcher.h \
            $$LIB_TST_DIR/async/dispatcher/cfg.h \
            $$LIB_TST_DIR/async/dispatcher/evt.h \
            $$LIB_TST_DIR/async/dispatcher/per.h \
            $$LIB_TST_DIR/async/dispatcher/sto.h \
            $$LIB_TST_DIR/async/dispatcher/typ.h \
            $$LIB_TST_DIR/async/dispatcher/uix.h

SOURCES = $$LIB_TST_DIR/async/dispatcher/main.cpp

LIBS+=-lsqlite3

QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tst.async.dispatcher

include (../../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS = \
            $$TST_DIR/async/dispatcher/bus.h \
            $$TST_DIR/async/dispatcher/dispatcher.h \
            $$TST_DIR/async/dispatcher/cfg.h \
            $$TST_DIR/async/dispatcher/evt.h \
            $$TST_DIR/async/dispatcher/per.h \
            $$TST_DIR/async/dispatcher/sto.h \
            $$TST_DIR/async/dispatcher/typ.h \
            $$TST_DIR/async/dispatcher/uix.h

SOURCES = $$TST_DIR/async/dispatcher/main.cpp

LIBS+=-lsqlite3

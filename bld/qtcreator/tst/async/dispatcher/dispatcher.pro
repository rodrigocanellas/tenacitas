QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.tst.async.dispatcher

include (../../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS = \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/bus.h \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/dispatcher.h \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/cfg.h \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/evt.h \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/per.h \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/sto.h \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/typ.h \
            $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/uix.h

SOURCES = $$BASE_DIR/tenacitas.lib/tst/async/dispatcher/main.cpp

LIBS+=-lsqlite3

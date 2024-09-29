QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.async.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas.lib/async/test/main.cpp

HEADERS = \
          $$BASE_DIR/tenacitas.lib/async/test/handling_test.h \
          $$BASE_DIR/tenacitas.lib/async/test/sleeping_loop_test.h \
          $$BASE_DIR/tenacitas.lib/async/test/handling_id_test.h \
          $$BASE_DIR/tenacitas.lib/async/test/handlings_test.h \
          $$BASE_DIR/tenacitas.lib/async/test/dispatcher_test.h \


DISTFILES += \
    $$BASE_DIR/tenacitas.lib/async/test/cfg_000.ini



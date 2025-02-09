QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.async.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)


SOURCES = $$BASE_DIR/tnct/async/test/main.cpp

HEADERS = \
          $$BASE_DIR/tnct/async/test/sleeping_loop_test.h \
          $$BASE_DIR/tnct/async/test/handling_id_test.h \
          $$BASE_DIR/tnct/async/test/handling_test.h \
          $$BASE_DIR/tnct/async/test/dispatcher_test.h

          # $$BASE_DIR/tnct/async/test/handlings_test.h \



DISTFILES += \
    $$BASE_DIR/tnct/async/test/cfg_000.ini



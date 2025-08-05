QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.async

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas/tst/async/main.cpp

HEADERS = \
         $$BASE_DIR/tenacitas/tst/async/dispatcher_test.h \
         $$BASE_DIR/tenacitas/tst/async/traits_test.h

          # $$BASE_DIR/tenacitas/tst/async/sleeping_loop_test.h \
          # $$BASE_DIR/tenacitas/tst/async/handling_test.h \





DISTFILES += \
    $$BASE_DIR/tenacitas/tst/async/cfg_000.ini



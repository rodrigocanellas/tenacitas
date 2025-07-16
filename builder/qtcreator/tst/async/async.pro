QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.async.test

QMAKE_CXXFLAGS += -DTENACITAS_LOG
include (../../common.pri)


SOURCES = $$BASE_DIR_TST/tnct/async/main.cpp

HEADERS = \
          $$BASE_DIR_TST/tnct/async/sleeping_loop_test.h \
          $$BASE_DIR_TST/tnct/async/handling_test.h \
          $$BASE_DIR_TST/tnct/async/dispatcher_test.h





DISTFILES += \
    $$BASE_DIR_TST/tnct/async/cfg_000.ini



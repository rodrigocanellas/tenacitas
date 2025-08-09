QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.container.test

include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/container/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = $$PRJ_DIR/matrix_test.h \
          $$PRJ_DIR/circular_queue_test.h

DISTFILES += \
    $$PRJ_DIR/circular_queue_tests_000.ini



QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.container.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas.lib/container/test/main.cpp

HEADERS = $$BASE_DIR/tenacitas.lib/container/test/matrix_test.h \
          $$BASE_DIR/tenacitas.lib/container/test/circular_queue_test.h

DISTFILES += \
    $$BASE_DIR/tenacitas.lib/container/test/circular_queue_tests_000.ini



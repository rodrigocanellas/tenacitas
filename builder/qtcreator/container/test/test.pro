QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.container.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas.lib/container/test/main.cpp

HEADERS = $$BASE_DIR/tenacitas.lib/container/test/matrix.h \
          $$BASE_DIR/tenacitas.lib/container/test/test_circular_queue.h

DISTFILES += \
    $$BASE_DIR/tenacitas.lib/container/test/circular_queue_tests_000.ini



QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tst.container

include (../../common.pri)


SOURCES = $$BASE_DIR/tenacitas/tst/container/main.cpp

HEADERS = $$BASE_DIR/tenacitas/tst/container/matrix_test.h \
          $$BASE_DIR/tenacitas/tst/container/circular_queue_test.h

DISTFILES += \
    $$BASE_DIR/tenacitas/tst/container/circular_queue_tests_000.ini



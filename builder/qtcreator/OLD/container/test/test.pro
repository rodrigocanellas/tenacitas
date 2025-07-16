QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.container.test

include (../../common.pri)


SOURCES = $$BASE_DIR/tnct/container/test/main.cpp

HEADERS = $$BASE_DIR/tnct/container/test/matrix_test.h \
          $$BASE_DIR/tnct/container/test/circular_queue_test.h

DISTFILES += \
    $$BASE_DIR/tnct/container/test/circular_queue_tests_000.ini



QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.container.test

include (../../../common.pri)


SOURCES = $$BASE_SRC_DIR/tnct/container/test/main.cpp

HEADERS = $$BASE_SRC_DIR/tnct/container/test/matrix_test.h \
          $$BASE_SRC_DIR/tnct/container/test/circular_queue_test.h

DISTFILES += \
    $$BASE_SRC_DIR/tnct/container/test/circular_queue_tests_000.ini



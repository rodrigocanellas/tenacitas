QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.container.test

include (../../common.pri)


SOURCES = $$BASE_DIR_TST/tnct/container/main.cpp

HEADERS = $$BASE_DIR_TST/tnct/container/matrix_test.h \
          $$BASE_DIR_TST/tnct/container/circular_queue_test.h

DISTFILES += \
    $$BASE_DIR_TST/tnct/container/circular_queue_tests_000.ini



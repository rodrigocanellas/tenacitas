QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.container.tst

include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/container/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
          $$PRJ_DIR/circular_queue_test.h \
          $$PRJ_DIR/cpt_test.h \
          $$PRJ_DIR/matrix_test.h \
          $$PRJ_DIR/multiply_matrix_test.h \
          $$PRJ_DIR/multiply_matrix_row_test.h

DISTFILES += \
    $$PRJ_DIR/circular_queue_tests_000.ini



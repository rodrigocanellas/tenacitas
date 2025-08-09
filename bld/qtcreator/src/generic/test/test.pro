QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.generic.test

include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/generic/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
        $$PRJ_DIR/tester_test.h \
        $$PRJ_DIR/id_test.h \
        $$PRJ_DIR/format_number_test.h

QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.format.test

include (../../../common.pri)

PRJ_DIR=$$BASE_DIR/tenacitas/src/format/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
        $$PRJ_DIR/format_number_test.h

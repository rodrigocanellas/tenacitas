QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.format.tst

include (../../common.pri)

PRJ_DIR=$$BASE_DIR/tnct/format/tst

SOURCES = $$PRJ_DIR/main.cpp

HEADERS = \
        $$PRJ_DIR/format_number_test.h

QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.format.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tnct/format/test/main.cpp

HEADERS = \
        $$BASE_DIR/tnct/format/test/format_number_test.h

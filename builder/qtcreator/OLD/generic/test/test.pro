QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.generic.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tnct/generic/test/main.cpp

HEADERS = \
        $$BASE_DIR/tnct/generic/test/tester_test.h \
        $$BASE_DIR/tnct/generic/test/id_test.h \
        $$BASE_DIR/tnct/generic/test/format_number_test.h

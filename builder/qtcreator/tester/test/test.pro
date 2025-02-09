QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tnct.tester.test

include (../../common.pri)

SOURCES = $$BASE_DIR/tnct/tester/test/main.cpp

HEADERS = \
        $$BASE_DIR/tnct/tester/test/test_test.h
